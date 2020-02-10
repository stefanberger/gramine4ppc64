#ifndef _SHIM_TCB_H_
#define _SHIM_TCB_H_

#include <assert.h>
#include <atomic.h>
#include <pal.h>

#include "shim_tcb-arch.h"

#define SHIM_TCB_CANARY 0xdeadbeef

struct shim_context {
    struct shim_regs *      regs;
    uint64_t                fs_base;
    struct shim_context *   next;
    struct atomic_int       preempt;
};

struct debug_buf;

typedef struct shim_tcb shim_tcb_t;
struct shim_tcb {
    uint64_t            canary;
    shim_tcb_t*         self;
    struct shim_thread* tp;
    struct shim_context context;
    unsigned int        tid;
    int                 pal_errno;
    struct debug_buf*   debug_buf;
    void*               vma_cache;

    /* This record is for testing the memory of user inputs.
     * If a segfault occurs with the range [start, end],
     * the code addr is set to cont_addr to alert the caller. */
    struct {
        void * start, * end;
        void * cont_addr;
        bool has_fault;
    } test_range;
};

#if defined(__i386__) || defined(__x86_64__)

static inline uint64_t shim_context_get_sp(struct shim_context *context) {
    return context->regs->rsp;
}

static inline void shim_context_set_sp(struct shim_context *context,
                                       uint64_t rsp) {
    context->regs->rsp = rsp;
}

#define SHIM_TCB_GET(member)                                            \
    ({                                                                  \
        shim_tcb_t* tcb;                                                \
        __typeof__(tcb->member) ret;                                    \
        static_assert(sizeof(ret) == 8 ||                               \
                      sizeof(ret) == 4 ||                               \
                      sizeof(ret) == 2 ||                               \
                      sizeof(ret) == 1,                                 \
                      "SHIM_TCB_GET can be used only for "              \
                      "8, 4, 2, or 1-byte(s) members");                 \
        switch (sizeof(ret)) {                                          \
        case 8:                                                         \
            __asm__("movq %%gs:%c1, %0\n"                               \
                    : "=r"(ret)                                         \
                    : "i" (offsetof(PAL_TCB, libos_tcb) +               \
                           offsetof(shim_tcb_t, member)));              \
            break;                                                      \
        case 4:                                                         \
            __asm__("movl %%gs:%c1, %0\n"                               \
                    : "=r"(ret)                                         \
                    : "i" (offsetof(PAL_TCB, libos_tcb) +               \
                           offsetof(shim_tcb_t, member)));              \
            break;                                                      \
        case 2:                                                         \
            __asm__("movw %%gs:%c1, %0\n"                               \
                    : "=r"(ret)                                         \
                    : "i" (offsetof(PAL_TCB, libos_tcb) +               \
                           offsetof(shim_tcb_t, member)));              \
            break;                                                      \
        case 1:                                                         \
            __asm__("movb %%gs:%c1, %0\n"                               \
                    : "=r"(ret)                                         \
                    : "i" (offsetof(PAL_TCB, libos_tcb) +               \
                           offsetof(shim_tcb_t, member)));              \
            break;                                                      \
        default:                                                        \
            __abort();                                                  \
        }                                                               \
        ret;                                                            \
    })

#define SHIM_TCB_SET(member, value)                                     \
    do {                                                                \
        shim_tcb_t* tcb;                                                \
        static_assert(sizeof(tcb->member) == 8 ||                       \
                      sizeof(tcb->member) == 4 ||                       \
                      sizeof(tcb->member) == 2 ||                       \
                      sizeof(tcb->member) == 1,                         \
                      "SHIM_TCB_SET can be used only for "              \
                      "8, 4, 2, or 1-byte(s) members");                 \
        switch (sizeof(tcb->member)) {                                  \
        case 8:                                                         \
            __asm__("movq %0, %%gs:%c1\n"                               \
                    :: "ir"(value),                                     \
                     "i"(offsetof(PAL_TCB, libos_tcb) +                 \
                         offsetof(shim_tcb_t, member)));                \
            break;                                                      \
        case 4:                                                         \
            __asm__("movl %0, %%gs:%c1\n"                               \
                    :: "ir"(value),                                     \
                     "i"(offsetof(PAL_TCB, libos_tcb) +                 \
                         offsetof(shim_tcb_t, member)));                \
            break;                                                      \
        case 2:                                                         \
            __asm__("movw %0, %%gs:%c1\n"                               \
                    :: "ir"(value),                                     \
                     "i"(offsetof(PAL_TCB, libos_tcb) +                 \
                         offsetof(shim_tcb_t, member)));                \
            break;                                                      \
        case 1:                                                         \
            __asm__("movb %0, %%gs:%c1\n"                               \
                    :: "ir"(value),                                     \
                     "i"(offsetof(PAL_TCB, libos_tcb) +                 \
                         offsetof(shim_tcb_t, member)));                \
            break;                                                      \
        default:                                                        \
            __abort();                                                  \
        }                                                               \
    } while (0)

#elif defined(__powerpc64__)

static inline uint64_t shim_context_get_sp(struct shim_context *context) {
    return context->regs->gpr[1];
}

static inline void shim_context_set_sp(struct shim_context *context,
                                       uint64_t rsp) {
    context->regs->gpr[1] = rsp;
}

#define TLS_TCB_OFFSET 0x7000

#define TLS_PAL_TCB_OFFSET (sizeof(PAL_TCB) + TLS_TCB_OFFSET)

static inline PAL_TCB *_shim_get_pal_tcb(uint64_t r13) {
    return (void *)r13 - TLS_TCB_OFFSET - sizeof(PAL_TCB);
}

static inline PAL_TCB *shim_get_pal_tcb(void) {
    uint64_t r13;

    __asm__ ("mr %0, %%r13\n\t"
             : "=r" (r13)
    );
    if (r13)
        return _shim_get_pal_tcb(r13);
    return NULL;
}

static inline PAL_TCB *_shim_set_r13(PAL_TCB *ptcb) {
    PAL_TCB *orig_tcb = shim_get_pal_tcb();

    if (ptcb) {
        __asm__("addi %%r13, %0, %1\n\t"
                :
                : "r" (ptcb),
                  "i" (sizeof(PAL_TCB) + TLS_TCB_OFFSET)
                : "r13"
        );
    } else {
        __asm__("li %%r13, 0\n\t"
                :
                :
                : "r13"
        );
    }
    return orig_tcb;
}

// Given a register r13, update the LibOS_TCB the PAL_TCB points to
static inline void _shim_set_LibOS_TCB(uint64_t r13, shim_tcb_t *tcb)
{
    assert(r13);
    assert(tcb);
    PAL_TCB* ptcb = _shim_get_pal_tcb(r13);
    ptcb->glibc_tcb.LibOS_TCB = container_of(tcb, PAL_TCB, libos_tcb);
}

#define SHIM_TCB_GET(member)                                            \
    ({                                                                  \
        shim_tcb_t* tcb;                                                \
        PAL_TCB *ptcb;							\
        __typeof__(tcb->member) ret;                                    \
        static_assert(sizeof(ret) == 8 ||                               \
                      sizeof(ret) == 4 ||                               \
                      sizeof(ret) == 2 ||                               \
                      sizeof(ret) == 1,                                 \
                      "SHIM_TCB_GET can be used only for "              \
                      "8, 4, 2, or 1-byte(s) members");                 \
        ptcb = shim_get_pal_tcb();                                      \
        tcb = (shim_tcb_t *)&ptcb->glibc_tcb.LibOS_TCB->libos_tcb[0];   \
        ret = tcb->member;                                              \
        ret;                                                            \
    })

#define SHIM_TCB_SET(member, value)                                     \
    do {                                                                \
        shim_tcb_t* tcb;                                                \
        PAL_TCB *ptcb;							\
        static_assert(sizeof(tcb->member) == 8 ||                       \
                      sizeof(tcb->member) == 4 ||                       \
                      sizeof(tcb->member) == 2 ||                       \
                      sizeof(tcb->member) == 1,                         \
                      "SHIM_TCB_SET can be used only for "              \
                      "8, 4, 2, or 1-byte(s) members");                 \
        ptcb = shim_get_pal_tcb();                                      \
        tcb = (shim_tcb_t *)&ptcb->glibc_tcb.LibOS_TCB->libos_tcb[0];   \
        tcb->member = value;                                            \
    } while (0)

// fs_base == NULL: switch to the shim_tcb if there is a glibc TCB set
// fs_base != NULL: assume fs_base is from glibc, so we set it and
//                  connect the shim_tcb to it via its LibOS_TCB pointer
static inline void *shim_tcb_set_fs_base(unsigned long fs_base, shim_tcb_t *tcb) {
    PAL_TCB *orig_ptcb = shim_get_pal_tcb();

    if (fs_base == 0) {
        PAL_TCB *ptcb = NULL;

        if (orig_ptcb)
            ptcb = orig_ptcb->glibc_tcb.LibOS_TCB;

        /* If shim's and active TCB are different, switch to shim's TCB */
        if (ptcb != orig_ptcb)
            _shim_set_r13(ptcb);
    } else {
        PAL_TCB *r13_ptcb = (PAL_TCB *)fs_base;
        PAL_TCB *ptcb = (void *)tcb - offsetof(PAL_TCB, libos_tcb);
        r13_ptcb->glibc_tcb.LibOS_TCB = ptcb;
        _shim_set_r13(r13_ptcb);
    }
    return orig_ptcb;
}
#endif

static inline void __shim_tcb_init(shim_tcb_t* shim_tcb) {
    shim_tcb->canary = SHIM_TCB_CANARY;
    shim_tcb->self = shim_tcb;
    shim_tcb->vma_cache = NULL;
}

/* Call this function at the beginning of thread execution. */
static inline void shim_tcb_init(void) {
    PAL_TCB* tcb = pal_get_tcb();
    shim_tcb_t* shim_tcb = (shim_tcb_t*)tcb->libos_tcb;
    memset(shim_tcb, 0, sizeof(*shim_tcb));
    __shim_tcb_init(shim_tcb);
}

static inline shim_tcb_t* shim_get_tcb(void) {
    return SHIM_TCB_GET(self);
}

static inline bool shim_tcb_check_canary(void) {
    return SHIM_TCB_GET(canary) == SHIM_TCB_CANARY;
}

#endif /* _SHIM_H_ */
