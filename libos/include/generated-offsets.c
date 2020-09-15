#include "generated_offsets_build.h"
#include "shim_internal.h"
#include "shim_tcb.h"

const char* generated_offsets_name = "LIBOS";

const struct generated_offset generated_offsets[] = {
#if defined(__x86_64__)
    OFFSET_T(LIBOS_TCB_OFF, PAL_TCB, libos_tcb),
    OFFSET_T(LIBOS_TCB_LIBOS_STACK_OFF, libos_tcb_t, libos_stack_bottom),
    OFFSET_T(LIBOS_TCB_SCRATCH_PC_OFF, libos_tcb_t, syscall_scratch_pc),

    OFFSET_T(PAL_CONTEXT_FPREGS_OFF, struct PAL_CONTEXT, fpregs),
    OFFSET_T(PAL_CONTEXT_MXCSR_OFF, struct PAL_CONTEXT, mxcsr),
    OFFSET_T(PAL_CONTEXT_FPCW_OFF, struct PAL_CONTEXT, fpcw),
    OFFSET_T(PAL_CONTEXT_FPREGS_USED_OFF, struct PAL_CONTEXT, is_fpregs_used),

    DEFINE(LIBOS_XSTATE_ALIGN, LIBOS_XSTATE_ALIGN),
    DEFINE(RED_ZONE_SIZE, RED_ZONE_SIZE),
#elif defined(__powerpc64__)
    OFFSET_T(SHIM_REGS_GPR0, PAL_CONTEXT, gpregs.gpr[0]),
    OFFSET_T(SHIM_REGS_GPR1, PAL_CONTEXT, gpregs.gpr[1]),
    OFFSET_T(SHIM_REGS_GPR2, PAL_CONTEXT, gpregs.gpr[2]),
    OFFSET_T(SHIM_REGS_GPR3, PAL_CONTEXT, gpregs.gpr[3]),
    OFFSET_T(SHIM_REGS_GPR4, PAL_CONTEXT, gpregs.gpr[4]),
    OFFSET_T(SHIM_REGS_GPR5, PAL_CONTEXT, gpregs.gpr[5]),
    OFFSET_T(SHIM_REGS_GPR6, PAL_CONTEXT, gpregs.gpr[6]),
    OFFSET_T(SHIM_REGS_GPR7, PAL_CONTEXT, gpregs.gpr[7]),
    OFFSET_T(SHIM_REGS_GPR8, PAL_CONTEXT, gpregs.gpr[8]),
    OFFSET_T(SHIM_REGS_GPR9, PAL_CONTEXT, gpregs.gpr[9]),
    OFFSET_T(SHIM_REGS_GPR10, PAL_CONTEXT, gpregs.gpr[10]),
    OFFSET_T(SHIM_REGS_GPR11, PAL_CONTEXT, gpregs.gpr[11]),
    OFFSET_T(SHIM_REGS_GPR12, PAL_CONTEXT, gpregs.gpr[12]),
    OFFSET_T(SHIM_REGS_GPR13, PAL_CONTEXT, gpregs.gpr[13]),
    OFFSET_T(SHIM_REGS_GPR14, PAL_CONTEXT, gpregs.gpr[14]),
    OFFSET_T(SHIM_REGS_GPR15, PAL_CONTEXT, gpregs.gpr[15]),
    OFFSET_T(SHIM_REGS_GPR16, PAL_CONTEXT, gpregs.gpr[16]),
    OFFSET_T(SHIM_REGS_GPR17, PAL_CONTEXT, gpregs.gpr[17]),
    OFFSET_T(SHIM_REGS_GPR18, PAL_CONTEXT, gpregs.gpr[18]),
    OFFSET_T(SHIM_REGS_GPR19, PAL_CONTEXT, gpregs.gpr[19]),
    OFFSET_T(SHIM_REGS_GPR20, PAL_CONTEXT, gpregs.gpr[20]),
    OFFSET_T(SHIM_REGS_GPR21, PAL_CONTEXT, gpregs.gpr[21]),
    OFFSET_T(SHIM_REGS_GPR22, PAL_CONTEXT, gpregs.gpr[22]),
    OFFSET_T(SHIM_REGS_GPR23, PAL_CONTEXT, gpregs.gpr[23]),
    OFFSET_T(SHIM_REGS_GPR24, PAL_CONTEXT, gpregs.gpr[24]),
    OFFSET_T(SHIM_REGS_GPR25, PAL_CONTEXT, gpregs.gpr[25]),
    OFFSET_T(SHIM_REGS_GPR26, PAL_CONTEXT, gpregs.gpr[26]),
    OFFSET_T(SHIM_REGS_GPR27, PAL_CONTEXT, gpregs.gpr[27]),
    OFFSET_T(SHIM_REGS_GPR28, PAL_CONTEXT, gpregs.gpr[28]),
    OFFSET_T(SHIM_REGS_GPR29, PAL_CONTEXT, gpregs.gpr[29]),
    OFFSET_T(SHIM_REGS_GPR30, PAL_CONTEXT, gpregs.gpr[30]),
    OFFSET_T(SHIM_REGS_GPR31, PAL_CONTEXT, gpregs.gpr[31]),
    OFFSET_T(SHIM_REGS_ORIG_GPR3, PAL_CONTEXT, gpregs.orig_gpr3),
    OFFSET_T(SHIM_REGS_NIP  , PAL_CONTEXT, gpregs.nip),
    OFFSET_T(SHIM_REGS_MSR  , PAL_CONTEXT, gpregs.msr),
    OFFSET_T(SHIM_REGS_CTR  , PAL_CONTEXT, gpregs.ctr),
    OFFSET_T(SHIM_REGS_LINK , PAL_CONTEXT, gpregs.link),
    OFFSET_T(SHIM_REGS_XER  , PAL_CONTEXT, gpregs.xer),
    OFFSET_T(SHIM_REGS_CCR  , PAL_CONTEXT, gpregs.ccr),
    OFFSET_T(SHIM_REGS_SOFTE, PAL_CONTEXT, gpregs.softe),
    OFFSET_T(SHIM_REGS_TRAP , PAL_CONTEXT, gpregs.trap),
    OFFSET_T(SHIM_REGS_DAR  , PAL_CONTEXT, gpregs.dar),
    OFFSET_T(SHIM_REGS_DSISR, PAL_CONTEXT, gpregs.dsisr),
    OFFSET_T(SHIM_REGS_VSR0 , PAL_CONTEXT, xtregs.vsr[0]),
    OFFSET_T(SHIM_REGS_VSR1 , PAL_CONTEXT, xtregs.vsr[1]),
    OFFSET_T(SHIM_REGS_VSR2 , PAL_CONTEXT, xtregs.vsr[2]),
    OFFSET_T(SHIM_REGS_VSR3 , PAL_CONTEXT, xtregs.vsr[3]),
    OFFSET_T(SHIM_REGS_VSR4 , PAL_CONTEXT, xtregs.vsr[4]),
    OFFSET_T(SHIM_REGS_VSR5 , PAL_CONTEXT, xtregs.vsr[5]),
    OFFSET_T(SHIM_REGS_VSR6 , PAL_CONTEXT, xtregs.vsr[6]),
    OFFSET_T(SHIM_REGS_VSR7 , PAL_CONTEXT, xtregs.vsr[7]),
    OFFSET_T(SHIM_REGS_VSR8 , PAL_CONTEXT, xtregs.vsr[8]),
    OFFSET_T(SHIM_REGS_VSR9 , PAL_CONTEXT, xtregs.vsr[9]),
    OFFSET_T(SHIM_REGS_VSR10, PAL_CONTEXT, xtregs.vsr[10]),
    OFFSET_T(SHIM_REGS_VSR11, PAL_CONTEXT, xtregs.vsr[11]),
    OFFSET_T(SHIM_REGS_VSR12, PAL_CONTEXT, xtregs.vsr[12]),
    OFFSET_T(SHIM_REGS_VSR13, PAL_CONTEXT, xtregs.vsr[13]),
    OFFSET_T(SHIM_REGS_VSR14, PAL_CONTEXT, xtregs.vsr[14]),
    OFFSET_T(SHIM_REGS_VSR15, PAL_CONTEXT, xtregs.vsr[15]),
    OFFSET_T(SHIM_REGS_VSR16, PAL_CONTEXT, xtregs.vsr[16]),
    OFFSET_T(SHIM_REGS_VSR17, PAL_CONTEXT, xtregs.vsr[17]),
    OFFSET_T(SHIM_REGS_VSR18, PAL_CONTEXT, xtregs.vsr[18]),
    OFFSET_T(SHIM_REGS_VSR19, PAL_CONTEXT, xtregs.vsr[19]),
    OFFSET_T(SHIM_REGS_VSR20, PAL_CONTEXT, xtregs.vsr[20]),
    OFFSET_T(SHIM_REGS_VSR21, PAL_CONTEXT, xtregs.vsr[21]),
    OFFSET_T(SHIM_REGS_VSR22, PAL_CONTEXT, xtregs.vsr[22]),
    OFFSET_T(SHIM_REGS_VSR23, PAL_CONTEXT, xtregs.vsr[23]),
    OFFSET_T(SHIM_REGS_VSR24, PAL_CONTEXT, xtregs.vsr[24]),
    OFFSET_T(SHIM_REGS_VSR25, PAL_CONTEXT, xtregs.vsr[25]),
    OFFSET_T(SHIM_REGS_VSR26, PAL_CONTEXT, xtregs.vsr[26]),
    OFFSET_T(SHIM_REGS_VSR27, PAL_CONTEXT, xtregs.vsr[27]),
    OFFSET_T(SHIM_REGS_VSR28, PAL_CONTEXT, xtregs.vsr[28]),
    OFFSET_T(SHIM_REGS_VSR29, PAL_CONTEXT, xtregs.vsr[29]),
    OFFSET_T(SHIM_REGS_VSR30, PAL_CONTEXT, xtregs.vsr[30]),
    OFFSET_T(SHIM_REGS_VSR31, PAL_CONTEXT, xtregs.vsr[31]),
    OFFSET_T(SHIM_REGS_VSR32, PAL_CONTEXT, xtregs.vsr[32]),
    OFFSET_T(SHIM_REGS_VSR33, PAL_CONTEXT, xtregs.vsr[33]),
    OFFSET_T(SHIM_REGS_VSR34, PAL_CONTEXT, xtregs.vsr[34]),
    OFFSET_T(SHIM_REGS_VSR35, PAL_CONTEXT, xtregs.vsr[35]),
    OFFSET_T(SHIM_REGS_VSR36, PAL_CONTEXT, xtregs.vsr[36]),
    OFFSET_T(SHIM_REGS_VSR37, PAL_CONTEXT, xtregs.vsr[37]),
    OFFSET_T(SHIM_REGS_VSR38, PAL_CONTEXT, xtregs.vsr[38]),
    OFFSET_T(SHIM_REGS_VSR39, PAL_CONTEXT, xtregs.vsr[39]),
    OFFSET_T(SHIM_REGS_VSR40, PAL_CONTEXT, xtregs.vsr[40]),
    OFFSET_T(SHIM_REGS_VSR41, PAL_CONTEXT, xtregs.vsr[41]),
    OFFSET_T(SHIM_REGS_VSR42, PAL_CONTEXT, xtregs.vsr[42]),
    OFFSET_T(SHIM_REGS_VSR43, PAL_CONTEXT, xtregs.vsr[43]),
    OFFSET_T(SHIM_REGS_VSR44, PAL_CONTEXT, xtregs.vsr[44]),
    OFFSET_T(SHIM_REGS_VSR45, PAL_CONTEXT, xtregs.vsr[45]),
    OFFSET_T(SHIM_REGS_VSR46, PAL_CONTEXT, xtregs.vsr[46]),
    OFFSET_T(SHIM_REGS_VSR47, PAL_CONTEXT, xtregs.vsr[47]),
    OFFSET_T(SHIM_REGS_VSR48, PAL_CONTEXT, xtregs.vsr[48]),
    OFFSET_T(SHIM_REGS_VSR49, PAL_CONTEXT, xtregs.vsr[49]),
    OFFSET_T(SHIM_REGS_VSR50, PAL_CONTEXT, xtregs.vsr[50]),
    OFFSET_T(SHIM_REGS_VSR51, PAL_CONTEXT, xtregs.vsr[51]),
    OFFSET_T(SHIM_REGS_VSR52, PAL_CONTEXT, xtregs.vsr[52]),
    OFFSET_T(SHIM_REGS_VSR53, PAL_CONTEXT, xtregs.vsr[53]),
    OFFSET_T(SHIM_REGS_VSR54, PAL_CONTEXT, xtregs.vsr[54]),
    OFFSET_T(SHIM_REGS_VSR55, PAL_CONTEXT, xtregs.vsr[55]),
    OFFSET_T(SHIM_REGS_VSR56, PAL_CONTEXT, xtregs.vsr[56]),
    OFFSET_T(SHIM_REGS_VSR57, PAL_CONTEXT, xtregs.vsr[57]),
    OFFSET_T(SHIM_REGS_VSR58, PAL_CONTEXT, xtregs.vsr[58]),
    OFFSET_T(SHIM_REGS_VSR59, PAL_CONTEXT, xtregs.vsr[59]),
    OFFSET_T(SHIM_REGS_VSR60, PAL_CONTEXT, xtregs.vsr[60]),
    OFFSET_T(SHIM_REGS_VSR61, PAL_CONTEXT, xtregs.vsr[61]),
    OFFSET_T(SHIM_REGS_VSR62, PAL_CONTEXT, xtregs.vsr[62]),
    OFFSET_T(SHIM_REGS_VSR63, PAL_CONTEXT, xtregs.vsr[63]),
    OFFSET_T(SHIM_REGS_VRSAVE, PAL_CONTEXT, xtregs.vrsave),
    OFFSET_T(SHIM_REGS_FPSCR, PAL_CONTEXT, xtregs.fpscr),
    OFFSET_T(SHIM_REGS_VSCR,  PAL_CONTEXT, xtregs.vscr),
    OFFSET_T(SHIM_STACK_GUARD, PAL_CONTEXT, stack_guard),
    DEFINE(SHIM_REGS_SIZE, sizeof(PAL_CONTEXT)),
#endif
    OFFSET_END,
};
