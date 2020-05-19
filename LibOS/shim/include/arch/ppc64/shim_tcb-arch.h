#ifndef _SHIM_TCB_ARCH_H_
#define _SHIM_TCB_ARCH_H_

#include <stdint.h>

struct shim_regs {
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
};

static inline uint64_t shim_regs_get_sp(struct shim_regs* sr) {
    return sr->gpr[1];
}

static inline void shim_regs_set_sp(struct shim_regs* sr, uint64_t sp) {
    sr->gpr[1] = sp;
}

static inline uint64_t shim_regs_get_syscallnr(struct shim_regs* sr) {
    return sr->gpr[0]; // FIXME: check this
}

static inline void shim_regs_set_syscallnr(struct shim_regs* sr, uint64_t sc_num) {
    sr->gpr[0] = sc_num; // FIXME: check this
}

#endif /* _SHIM_TCB_ARCH_H_ */
