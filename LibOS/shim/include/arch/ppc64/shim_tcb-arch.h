/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2020 IBM Corporation */

#ifndef _SHIM_TCB_ARCH_H_
#define _SHIM_TCB_ARCH_H_

#include <stdint.h>

#include "api.h"
#include "pal.h"

struct shim_pt_regs {
    /* same layout as pt_regs and same layout as ucontext's gp_regs:
     * https://elixir.bootlin.com/linux/v5.6.13/source/arch/powerpc/include/uapi/asm/ptrace.h#L68
     */
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
    /* needs to be 16-byte aligned when on stack */
} __attribute__((aligned(16)));

struct shim_regs {
    struct shim_pt_regs pt_regs; // general purpose registers
} __attribute__((aligned(16)));

static inline uint64_t shim_regs_get_sp(struct shim_regs* sr) {
    return sr->pt_regs.gpr[1];
}

static inline void shim_regs_set_sp(struct shim_regs* sr, uint64_t sp) {
    sr->pt_regs.gpr[1] = sp;
}

static inline uint64_t shim_regs_get_ip(struct shim_regs* sr) {
    return sr->pt_regs.nip;
}

static inline uint64_t shim_regs_get_syscallnr(struct shim_regs* sr) {
    return sr->pt_regs.gpr[0]; // FIXME: check this
}

static inline void shim_regs_set_syscallnr(struct shim_regs* sr, uint64_t sc_num) {
    sr->pt_regs.gpr[0] = sc_num; // FIXME: check this
}

struct shim_tcb;
typedef struct shim_tcb shim_tcb_t;

static inline PAL_TCB* shim_get_pal_tcb(void) {
    /* Do NOT use pal_get_tcb(). it will not work */
    register uint64_t r13 __asm__("r13");

    if (r13)
        return r13_to_pal_tcb(r13);
    return NULL;
}

/* Set r13 given the PAL_TCB */
static inline void shim_set_r13(PAL_TCB* ptcb) {
    pal_set_r13(ptcb);
}

/* Given a register r13, update the LibOS_TCB the PAL_TCB points to */
static inline void shim_r13_set_tcb(uint64_t r13, shim_tcb_t* tcb) {
    assert(r13);
    assert(tcb);
    PAL_TCB* ptcb = r13_to_pal_tcb(r13);
    ptcb->glibc_tcb.LibOS_TCB = container_of(tcb, PAL_TCB, libos_tcb);
}

// r13 == 0: switch to the shim_tcb if there is a glibc TCB set
// r13 != 0: assume tls_base is from glibc, so we set it and
//           connect the shim_tcb to it via its LibOS_TCB pointer
static inline void* shim_tcb_set_r13(unsigned long r13, shim_tcb_t* tcb) {
    PAL_TCB* orig_ptcb = shim_get_pal_tcb();

    if (r13 == 0) {
        PAL_TCB *ptcb = NULL;

        if (orig_ptcb)
            ptcb = orig_ptcb->glibc_tcb.LibOS_TCB;

        /* If shim's and active TCB are different, switch to shim's TCB */
        if (ptcb != orig_ptcb)
            shim_set_r13(ptcb);
    } else {
        tcbhead_t *glibc_tcb = r13_to_tcbhead_t(r13);
        /* given a shim_tcb, get the PAL_TCB it is embedded in */
        PAL_TCB* ptcb = (void*)tcb - offsetof(PAL_TCB, libos_tcb);
        glibc_tcb->LibOS_TCB = ptcb;
        __asm__("addi %%r13, %0, 0\n\t"
                :
                : "r" (r13)
                : "r13"
        );
    }
    return orig_ptcb;
}


static inline uint64_t shim_tls_base_to_r13(uint64_t tls_base) {
    return pal_tcb_to_r13((PAL_TCB*)tls_base);
}

#define SHIM_TCB_GET(member)                                            \
    ({                                                                  \
        shim_tcb_t* tcb;                                                \
        PAL_TCB* ptcb;							\
        __typeof__(tcb->member) ret;                                    \
        static_assert(sizeof(ret) == 8 ||                               \
                      sizeof(ret) == 4 ||                               \
                      sizeof(ret) == 2 ||                               \
                      sizeof(ret) == 1,                                 \
                      "SHIM_TCB_GET can be used only for "              \
                      "8, 4, 2, or 1-byte(s) members");                 \
        ptcb = shim_get_pal_tcb();                                      \
        tcb = (shim_tcb_t*)&ptcb->glibc_tcb.LibOS_TCB->libos_tcb[0];    \
        ret = tcb->member;                                              \
        ret;                                                            \
    })

#define SHIM_TCB_SET(member, value)                                     \
    do {                                                                \
        shim_tcb_t* tcb;                                                \
        PAL_TCB* ptcb;							\
        static_assert(sizeof(tcb->member) == 8 ||                       \
                      sizeof(tcb->member) == 4 ||                       \
                      sizeof(tcb->member) == 2 ||                       \
                      sizeof(tcb->member) == 1,                         \
                      "SHIM_TCB_SET can be used only for "              \
                      "8, 4, 2, or 1-byte(s) members");                 \
        ptcb = shim_get_pal_tcb();                                      \
        tcb = (shim_tcb_t*)&ptcb->glibc_tcb.LibOS_TCB->libos_tcb[0];    \
        tcb->member = value;                                            \
    } while (0)

static inline void shim_arch_update_tls_base(unsigned long r13, shim_tcb_t *shim_tcb) {
    /* r13 contains the value to load into r13 !! */
    shim_tcb_set_r13(r13, shim_tcb);
}

/* On ppc64 the tls_base (PAL_TCB*) needs to be offset adjusted from plain tls, which is prepared
 * to be loaded into r13.
 */
static inline unsigned long tls_to_tls_base(unsigned long tls) {
    if (tls)
        return (unsigned long)r13_to_pal_tcb(tls);
    return 0;
}

/* extended context */
struct shim_ext_context {
};

#endif /* _SHIM_TCB_ARCH_H_ */
