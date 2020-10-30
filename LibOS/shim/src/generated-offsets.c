#include <stddef.h>
#include <assert.h>

#include "generated-offsets-build.h"
#include "shim_internal.h"
#include "shim_tcb.h"

__attribute__((__used__)) static void dummy(void) {
    OFFSET_T(SHIM_TCB_OFFSET, PAL_TCB, libos_tcb);
#if defined(__i386__) || defined(__x86_64__)
    OFFSET_T(TCB_REGS, shim_tcb_t, context.regs);
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
    OFFSET(SHIM_REGS_VSR0 , shim_regs, xt_regs.vsr[0]);
    OFFSET(SHIM_REGS_VSR1 , shim_regs, xt_regs.vsr[1]);
    OFFSET(SHIM_REGS_VSR2 , shim_regs, xt_regs.vsr[2]);
    OFFSET(SHIM_REGS_VSR3 , shim_regs, xt_regs.vsr[3]);
    OFFSET(SHIM_REGS_VSR4 , shim_regs, xt_regs.vsr[4]);
    OFFSET(SHIM_REGS_VSR5 , shim_regs, xt_regs.vsr[5]);
    OFFSET(SHIM_REGS_VSR6 , shim_regs, xt_regs.vsr[6]);
    OFFSET(SHIM_REGS_VSR7 , shim_regs, xt_regs.vsr[7]);
    OFFSET(SHIM_REGS_VSR8 , shim_regs, xt_regs.vsr[8]);
    OFFSET(SHIM_REGS_VSR9 , shim_regs, xt_regs.vsr[9]);
    OFFSET(SHIM_REGS_VSR10, shim_regs, xt_regs.vsr[10]);
    OFFSET(SHIM_REGS_VSR11, shim_regs, xt_regs.vsr[11]);
    OFFSET(SHIM_REGS_VSR12, shim_regs, xt_regs.vsr[12]);
    OFFSET(SHIM_REGS_VSR13, shim_regs, xt_regs.vsr[13]);
    OFFSET(SHIM_REGS_VSR14, shim_regs, xt_regs.vsr[14]);
    OFFSET(SHIM_REGS_VSR15, shim_regs, xt_regs.vsr[15]);
    OFFSET(SHIM_REGS_VSR16, shim_regs, xt_regs.vsr[16]);
    OFFSET(SHIM_REGS_VSR17, shim_regs, xt_regs.vsr[17]);
    OFFSET(SHIM_REGS_VSR18, shim_regs, xt_regs.vsr[18]);
    OFFSET(SHIM_REGS_VSR19, shim_regs, xt_regs.vsr[19]);
    OFFSET(SHIM_REGS_VSR20, shim_regs, xt_regs.vsr[20]);
    OFFSET(SHIM_REGS_VSR21, shim_regs, xt_regs.vsr[21]);
    OFFSET(SHIM_REGS_VSR22, shim_regs, xt_regs.vsr[22]);
    OFFSET(SHIM_REGS_VSR23, shim_regs, xt_regs.vsr[23]);
    OFFSET(SHIM_REGS_VSR24, shim_regs, xt_regs.vsr[24]);
    OFFSET(SHIM_REGS_VSR25, shim_regs, xt_regs.vsr[25]);
    OFFSET(SHIM_REGS_VSR26, shim_regs, xt_regs.vsr[26]);
    OFFSET(SHIM_REGS_VSR27, shim_regs, xt_regs.vsr[27]);
    OFFSET(SHIM_REGS_VSR28, shim_regs, xt_regs.vsr[28]);
    OFFSET(SHIM_REGS_VSR29, shim_regs, xt_regs.vsr[29]);
    OFFSET(SHIM_REGS_VSR30, shim_regs, xt_regs.vsr[30]);
    OFFSET(SHIM_REGS_VSR31, shim_regs, xt_regs.vsr[31]);
    OFFSET(SHIM_REGS_VSR32, shim_regs, xt_regs.vsr[32]);
    OFFSET(SHIM_REGS_VSR33, shim_regs, xt_regs.vsr[33]);
    OFFSET(SHIM_REGS_VSR34, shim_regs, xt_regs.vsr[34]);
    OFFSET(SHIM_REGS_VSR35, shim_regs, xt_regs.vsr[35]);
    OFFSET(SHIM_REGS_VSR36, shim_regs, xt_regs.vsr[36]);
    OFFSET(SHIM_REGS_VSR37, shim_regs, xt_regs.vsr[37]);
    OFFSET(SHIM_REGS_VSR38, shim_regs, xt_regs.vsr[38]);
    OFFSET(SHIM_REGS_VSR39, shim_regs, xt_regs.vsr[39]);
    OFFSET(SHIM_REGS_VSR40, shim_regs, xt_regs.vsr[40]);
    OFFSET(SHIM_REGS_VSR41, shim_regs, xt_regs.vsr[41]);
    OFFSET(SHIM_REGS_VSR42, shim_regs, xt_regs.vsr[42]);
    OFFSET(SHIM_REGS_VSR43, shim_regs, xt_regs.vsr[43]);
    OFFSET(SHIM_REGS_VSR44, shim_regs, xt_regs.vsr[44]);
    OFFSET(SHIM_REGS_VSR45, shim_regs, xt_regs.vsr[45]);
    OFFSET(SHIM_REGS_VSR46, shim_regs, xt_regs.vsr[46]);
    OFFSET(SHIM_REGS_VSR47, shim_regs, xt_regs.vsr[47]);
    OFFSET(SHIM_REGS_VSR48, shim_regs, xt_regs.vsr[48]);
    OFFSET(SHIM_REGS_VSR49, shim_regs, xt_regs.vsr[49]);
    OFFSET(SHIM_REGS_VSR50, shim_regs, xt_regs.vsr[50]);
    OFFSET(SHIM_REGS_VSR51, shim_regs, xt_regs.vsr[51]);
    OFFSET(SHIM_REGS_VSR52, shim_regs, xt_regs.vsr[52]);
    OFFSET(SHIM_REGS_VSR53, shim_regs, xt_regs.vsr[53]);
    OFFSET(SHIM_REGS_VSR54, shim_regs, xt_regs.vsr[54]);
    OFFSET(SHIM_REGS_VSR55, shim_regs, xt_regs.vsr[55]);
    OFFSET(SHIM_REGS_VSR56, shim_regs, xt_regs.vsr[56]);
    OFFSET(SHIM_REGS_VSR57, shim_regs, xt_regs.vsr[57]);
    OFFSET(SHIM_REGS_VSR58, shim_regs, xt_regs.vsr[58]);
    OFFSET(SHIM_REGS_VSR59, shim_regs, xt_regs.vsr[59]);
    OFFSET(SHIM_REGS_VSR60, shim_regs, xt_regs.vsr[60]);
    OFFSET(SHIM_REGS_VSR61, shim_regs, xt_regs.vsr[61]);
    OFFSET(SHIM_REGS_VSR62, shim_regs, xt_regs.vsr[62]);
    OFFSET(SHIM_REGS_VSR63, shim_regs, xt_regs.vsr[63]);
    OFFSET(SHIM_REGS_VRSAVE, shim_regs, xt_regs.vrsave);
    OFFSET(SHIM_REGS_FPSCR, shim_regs, xt_regs.fpscr);
    OFFSET(SHIM_REGS_VSCR,  shim_regs, xt_regs.vscr);
    /* shim_regs's size must be a multiple of 16 byte */
    assert((sizeof(struct shim_regs) & 0xf) == 0);
    DEFINE(SHIM_REGS_SIZE, sizeof(struct shim_regs));
#endif
}
