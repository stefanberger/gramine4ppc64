#ifndef _SHIM_TCB_H_
#define _SHIM_TCB_H_

#include <assert.h>
#include <atomic.h>
#include <pal.h>

#define SHIM_TCB_CANARY 0xdeadbeef

struct shim_regs {
#if defined(__i386__) || defined(__x86_64__)
    uint64_t    orig_rax;
    uint64_t    rsp;
    uint64_t    r15;
    uint64_t    r14;
    uint64_t    r13;
    uint64_t    r12;
    uint64_t    r11;
    uint64_t    r10;
    uint64_t    r9;
    uint64_t    r8;
    uint64_t    rcx;
    uint64_t    rdx;
    uint64_t    rsi;
    uint64_t    rdi;
    uint64_t    rbx;
    uint64_t    rbp;
    uint64_t    rflags;
    uint64_t    rip;
#elif defined(__powerpc64__)
    /* same layout as pt_regs */
    uint64_t    gpr[32];
    uint64_t    nip;
    uint64_t    msr;
    uint64_t    orig_gpr3;
    uint64_t    ctr;
    uint64_t    link;
    uint64_t    xer;
    uint64_t    ccr;
    uint64_t    softe;
    uint64_t    trap;
    uint64_t    dar;
    uint64_t    dsisr;
    uint64_t    result;
#endif
};

struct shim_context {
    struct shim_regs *      regs;
    uint64_t                fs_base;
    struct shim_context *   next;
    uint64_t                enter_time;
    struct atomic_int       preempt;
};

struct debug_buf;

typedef struct shim_tcb shim_tcb_t;
struct shim_tcb {
    uint64_t                canary;
    shim_tcb_t *            self;
    struct shim_thread *    tp;
    struct shim_context     context;
    unsigned int            tid;
    int                     pal_errno;
    struct debug_buf *      debug_buf;

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

#define TLS_TCB_OFFSET 0x7000

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
        __asm__ ("addi %0, %%r13, %1\n\t"                               \
            : "=r" (ptcb)                                               \
            : "i" (-TLS_TCB_OFFSET - sizeof(PAL_TCB))                   \
        );                                                              \
        tcb = (shim_tcb_t *)&ptcb->glibc_tcb.LibOS_TCB->libos_tcb[0];   \
        ret = tcb->member;                                              \
        ret;                                                            \
    })

#define SHIM_TCB_SET(member, value)                                     \
    ({                                                                  \
        shim_tcb_t* tcb;                                                \
        PAL_TCB *ptcb;							\
        static_assert(sizeof(tcb->member) == 8 ||                       \
                      sizeof(tcb->member) == 4 ||                       \
                      sizeof(tcb->member) == 2 ||                       \
                      sizeof(tcb->member) == 1,                         \
                      "SHIM_TCB_SET can be used only for "              \
                      "8, 4, 2, or 1-byte(s) members");                 \
        __asm__ ("addi %0, %%r13, %1\n\t"                               \
            : "=r" (ptcb)                                               \
            : "i" (-TLS_TCB_OFFSET - sizeof(PAL_TCB))                   \
        );                                                              \
        tcb = (shim_tcb_t *)&ptcb->glibc_tcb.LibOS_TCB->libos_tcb[0];   \
        tcb->member = value;                                            \
    } while (0)

#endif

static inline void __shim_tcb_init(shim_tcb_t* shim_tcb) {
    shim_tcb->canary = SHIM_TCB_CANARY;
    shim_tcb->self = shim_tcb;
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

#ifdef __powerpc64__
// get rid of the TCB set by glibc, if there is one
static inline void *shim_tcb_clear(void) {
    PAL_TCB *r13_ptcb;
    PAL_TCB *ptcb;
    __asm__ ("addi %0, %%r13, %1\n\t"
        : "=r" (r13_ptcb)
        : "i" (-TLS_TCB_OFFSET - sizeof(PAL_TCB))
    );

    ptcb = r13_ptcb->glibc_tcb.LibOS_TCB;
    if (ptcb != r13_ptcb) {
        __asm__ ("addi %%r13, %0, %1\n\t"
            :
            : "r" (ptcb), "i" (sizeof(PAL_TCB) + TLS_TCB_OFFSET)
            : "r13"
        );
    }
    return r13_ptcb - TLS_TCB_OFFSET - sizeof(PAL_TCB);
}
#endif

#endif /* _SHIM_H_ */
