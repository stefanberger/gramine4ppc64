/* SPDX-License-Identifier: LGPL-3.0-or-later */

/*
 * shim_context-ppc64.c
 *
 * This file contains code for ppc64-specific CPU context manipulation.
 */

#include "shim_context.h"

#include "asm-offsets.h"
#include "shim_internal.h"
#include "stack.h"

#define XSTATE_RESET_SIZE ()

uint32_t g_shim_xsave_size = 0;

void shim_xstate_init(void) {
}

void shim_xstate_save(void* xstate_extended) {
    (void)xstate_extended;
}

void shim_xstate_restore(const void* xstate_extended) {
    (void)xstate_extended;
}

void restore_child_context_after_clone (struct shim_context* context) {
    assert(context->regs);
    struct shim_regs regs = *context->regs;

    debug("restore context: SP = 0x%08lx, IP = 0x%08lx, R2 = 0x%lx, R3 = 0x%lx, R12 = 0x%lx, R13 = 0x%lx\n",
          regs.pt_regs.gpr[1], regs.pt_regs.nip, regs.pt_regs.gpr[2], regs.pt_regs.gpr[3], regs.pt_regs.gpr[12], regs.pt_regs.gpr[13]);
#if 1
    struct stackframe *sf = (void *)regs.pt_regs.gpr[1];
    debug("Resume: Stack of child: sp: %p lr_save: 0x%lx  toc_save: 0x%lx\n",
          sf, sf->lr_save, sf->toc_save);
#endif

    /* Ready to resume execution, re-enable preemption. */
    shim_tcb_t* tcb = shim_get_tcb();
    __enable_preempt(tcb);

    unsigned long fs_base = context->fs_base;
    memset(context, 0, sizeof(struct shim_context));
    context->fs_base = fs_base;

    // the process we will resume points to an old PAL_TCB via r13; it is
    // inaccessible and so we need to update it to the one we are using here
    shim_r13_set_tcb(regs.pt_regs.gpr[13], tcb);

    __asm__ __volatile__(
        "mr 31, %0\n\t"
        "ld  0," XSTRINGIFY(SHIM_REGS_VRSAVE) "(31)\n\t; mtvrsave 0\n\t"
        "lfd 0," XSTRINGIFY(SHIM_REGS_FPSCR)  "(31)\n\t; mtfsf 255,0,1,0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSCR) "\n\t; lvx 0,31,0\n\t; mtvscr 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x  0, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x  1, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x  2, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x  3, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x  4, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x  5, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x  6, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x  7, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x  8, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x  9, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 10, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 11, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 12, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 13, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 14, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 15, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 16, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 17, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 18, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 19, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 20, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 21, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 22, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 23, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 24, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 25, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 26, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 27, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 28, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 29, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 30, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 31, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 32, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 33, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 34, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 35, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 36, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 37, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 38, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 39, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 40, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 41, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 42, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 43, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 44, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 45, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 46, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 47, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 48, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 49, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 50, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 51, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 52, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 53, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 54, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 55, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 56, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 57, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 58, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 59, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 60, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 61, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 62, 31, 0\n\t"
        "li  0," XSTRINGIFY(SHIM_REGS_VSR0)";lxvd2x 63, 31, 0\n\t"
        "ld  1," XSTRINGIFY(SHIM_REGS_GPR1)  "(31)\n\t"
        "ld  2," XSTRINGIFY(SHIM_REGS_GPR2)  "(31)\n\t"
        "ld  3," XSTRINGIFY(SHIM_REGS_GPR3)  "(31)\n\t"
        "ld  4," XSTRINGIFY(SHIM_REGS_GPR4)  "(31)\n\t"
        "ld  5," XSTRINGIFY(SHIM_REGS_GPR5)  "(31)\n\t"
        "ld  6," XSTRINGIFY(SHIM_REGS_GPR6)  "(31)\n\t"
        "ld  7," XSTRINGIFY(SHIM_REGS_GPR7)  "(31)\n\t"
        "ld  8," XSTRINGIFY(SHIM_REGS_GPR8)  "(31)\n\t"
        "ld  9," XSTRINGIFY(SHIM_REGS_GPR9)  "(31)\n\t"
        "ld 10," XSTRINGIFY(SHIM_REGS_GPR10) "(31)\n\t"
        "ld 11," XSTRINGIFY(SHIM_REGS_GPR11) "(31)\n\t"
        "ld 12," XSTRINGIFY(SHIM_REGS_GPR12) "(31)\n\t"
        "ld 13," XSTRINGIFY(SHIM_REGS_GPR13) "(31)\n\t"
        "ld 14," XSTRINGIFY(SHIM_REGS_GPR14) "(31)\n\t"
        "ld 15," XSTRINGIFY(SHIM_REGS_GPR15) "(31)\n\t"
        "ld 16," XSTRINGIFY(SHIM_REGS_GPR16) "(31)\n\t"
        "ld 17," XSTRINGIFY(SHIM_REGS_GPR17) "(31)\n\t"
        "ld 18," XSTRINGIFY(SHIM_REGS_GPR18) "(31)\n\t"
        "ld 19," XSTRINGIFY(SHIM_REGS_GPR19) "(31)\n\t"
        "ld 20," XSTRINGIFY(SHIM_REGS_GPR20) "(31)\n\t"
        "ld 21," XSTRINGIFY(SHIM_REGS_GPR21) "(31)\n\t"
        "ld 22," XSTRINGIFY(SHIM_REGS_GPR22) "(31)\n\t"
        "ld 23," XSTRINGIFY(SHIM_REGS_GPR23) "(31)\n\t"
        "ld 24," XSTRINGIFY(SHIM_REGS_GPR24) "(31)\n\t"
        "ld 25," XSTRINGIFY(SHIM_REGS_GPR25) "(31)\n\t"
        "ld 26," XSTRINGIFY(SHIM_REGS_GPR26) "(31)\n\t"
        "ld 27," XSTRINGIFY(SHIM_REGS_GPR27) "(31)\n\t"
        "ld 28," XSTRINGIFY(SHIM_REGS_GPR28) "(31)\n\t"
        "ld 29," XSTRINGIFY(SHIM_REGS_GPR29) "(31)\n\t"
        "ld 30," XSTRINGIFY(SHIM_REGS_GPR30) "(31)\n\t"
        "ld  0," XSTRINGIFY(SHIM_REGS_CTR )  "(31)\n\t"
        "mtctr 0\n\t"
        "ld  0," XSTRINGIFY(SHIM_REGS_XER)   "(31)\n\t"
        "mtxer 0\n\t"
        "ld  0," XSTRINGIFY(SHIM_REGS_CCR)   "(31)\n\t"
        "mtcr 0\n\t"
        "ld  0," XSTRINGIFY(SHIM_REGS_NIP)   "(31)\n\t"
        "mtlr 0\n\t"
        "ld  0," XSTRINGIFY(SHIM_REGS_GPR0)  "(31)\n\t"
        "ld 31," XSTRINGIFY(SHIM_REGS_GPR31) "(31)\n\t"
        "blr\n\t"
        :
        : "r"(&regs)
        :
    );

    debug("Should never get here! STOP!\n");while(1);
}

void fixup_child_context(struct shim_regs* regs, void* stack, uint64_t fs_base) {
    struct stackframe *sf = stack - offsetof(struct stackframe, backchain);
    // glibc's clone.S set parm_save and lr_save
    regs->pt_regs.gpr[3] = sf->parm_save[0];              // parameter to function
    regs->pt_regs.gpr[12] = sf->lr_save;                  // function to call
    regs->pt_regs.gpr[13] = shim_fs_base_to_r13(fs_base); // TLS
    regs->pt_regs.nip = sf->lr_save;                      // function to call

    debug("child swapping stack to %p return 0x%lx\n", stack, regs->pt_regs.nip);
}
