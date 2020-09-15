#include <stddef.h>
#include <assert.h>

#include "generated-offsets-build.h"
#include "shim_internal.h"
#include "shim_tcb.h"

__attribute__((__used__)) static void dummy(void) {
    OFFSET_T(SHIM_TCB_OFFSET, PAL_TCB, libos_tcb);
#if defined(__i386__) || defined(__x86_64__)
    OFFSET_T(TCB_REGS, shim_tcb_t, context.regs);
    OFFSET_T(TCB_FPCW, shim_tcb_t, context.ext_ctx.fpcw);
    OFFSET_T(TCB_MXCSR, shim_tcb_t, context.ext_ctx.mxcsr);
    OFFSET(SHIM_REGS_RSP, shim_regs, rsp);
    OFFSET(SHIM_REGS_R15, shim_regs, r15);
    OFFSET(SHIM_REGS_RIP, shim_regs, rip);
    DEFINE(SHIM_REGS_SIZE, sizeof(struct shim_regs));

    /* definitions */
    DEFINE(RED_ZONE_SIZE, RED_ZONE_SIZE);
#elif defined(__powerpc64__)
    OFFSET(SHIM_REGS_GPR0, shim_regs, pt_regs.gpr[0]);
    OFFSET(SHIM_REGS_GPR1, shim_regs, pt_regs.gpr[1]);
    OFFSET(SHIM_REGS_GPR2, shim_regs, pt_regs.gpr[2]);
    OFFSET(SHIM_REGS_GPR3, shim_regs, pt_regs.gpr[3]);
    OFFSET(SHIM_REGS_GPR4, shim_regs, pt_regs.gpr[4]);
    OFFSET(SHIM_REGS_GPR5, shim_regs, pt_regs.gpr[5]);
    OFFSET(SHIM_REGS_GPR6, shim_regs, pt_regs.gpr[6]);
    OFFSET(SHIM_REGS_GPR7, shim_regs, pt_regs.gpr[7]);
    OFFSET(SHIM_REGS_GPR8, shim_regs, pt_regs.gpr[8]);
    OFFSET(SHIM_REGS_GPR9, shim_regs, pt_regs.gpr[9]);
    OFFSET(SHIM_REGS_GPR10, shim_regs, pt_regs.gpr[10]);
    OFFSET(SHIM_REGS_GPR11, shim_regs, pt_regs.gpr[11]);
    OFFSET(SHIM_REGS_GPR12, shim_regs, pt_regs.gpr[12]);
    OFFSET(SHIM_REGS_GPR13, shim_regs, pt_regs.gpr[13]);
    OFFSET(SHIM_REGS_GPR14, shim_regs, pt_regs.gpr[14]);
    OFFSET(SHIM_REGS_GPR15, shim_regs, pt_regs.gpr[15]);
    OFFSET(SHIM_REGS_GPR16, shim_regs, pt_regs.gpr[16]);
    OFFSET(SHIM_REGS_GPR17, shim_regs, pt_regs.gpr[17]);
    OFFSET(SHIM_REGS_GPR18, shim_regs, pt_regs.gpr[18]);
    OFFSET(SHIM_REGS_GPR19, shim_regs, pt_regs.gpr[19]);
    OFFSET(SHIM_REGS_GPR20, shim_regs, pt_regs.gpr[20]);
    OFFSET(SHIM_REGS_GPR21, shim_regs, pt_regs.gpr[21]);
    OFFSET(SHIM_REGS_GPR22, shim_regs, pt_regs.gpr[22]);
    OFFSET(SHIM_REGS_GPR23, shim_regs, pt_regs.gpr[23]);
    OFFSET(SHIM_REGS_GPR24, shim_regs, pt_regs.gpr[24]);
    OFFSET(SHIM_REGS_GPR25, shim_regs, pt_regs.gpr[25]);
    OFFSET(SHIM_REGS_GPR26, shim_regs, pt_regs.gpr[26]);
    OFFSET(SHIM_REGS_GPR27, shim_regs, pt_regs.gpr[27]);
    OFFSET(SHIM_REGS_GPR28, shim_regs, pt_regs.gpr[28]);
    OFFSET(SHIM_REGS_GPR29, shim_regs, pt_regs.gpr[29]);
    OFFSET(SHIM_REGS_GPR30, shim_regs, pt_regs.gpr[30]);
    OFFSET(SHIM_REGS_GPR31, shim_regs, pt_regs.gpr[31]);
    OFFSET(SHIM_REGS_NIP  , shim_regs, pt_regs.nip);
    OFFSET(SHIM_REGS_CTR  , shim_regs, pt_regs.ctr);
    OFFSET(SHIM_REGS_LINK , shim_regs, pt_regs.link);
    OFFSET(SHIM_REGS_XER  , shim_regs, pt_regs.xer);
    OFFSET(SHIM_REGS_CCR  , shim_regs, pt_regs.ccr);
    /* shim_regs's size must be a multiple of 16 byte */
    assert((sizeof(struct shim_regs) & 0xf) == 0);
    DEFINE(SHIM_REGS_SIZE, sizeof(struct shim_regs));
#endif
}
