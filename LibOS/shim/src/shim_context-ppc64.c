/*
   This file is part of Graphene Library OS.
   Graphene Library OS is free software: you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License
   as published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.
   Graphene Library OS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.
   You should have received a copy of the GNU Lesser General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/*
 * shim_context-ppc64.c
 *
 * This file contains code for ppc64-specific CPU context manipulation.
 */

#include "shim_context.h"

#include "asm-offsets.h"
#include "shim_internal.h"
#include "stack.h"

void restore_context (struct shim_context* context) {
    assert(context->regs);
    struct shim_regs regs = *context->regs;

    debug("restore context: SP = 0x%08lx, IP = 0x%08lx, R2 = 0x%lx, R3 = 0x%lx, R12 = 0x%lx, R13 = 0x%lx\n",
          regs.gpr[1], regs.nip, regs.gpr[2], regs.gpr[3], regs.gpr[12], regs.gpr[13]);
#if 1
    struct stackframe *sf = (void *)regs.gpr[1];
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
    shim_r13_set_tcb(regs.gpr[13], tcb);

    __asm__ __volatile__(
        "mr 31, %0\n\t"
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
    regs->gpr[3] = sf->parm_save[0];              // parameter to function
    regs->gpr[12] = sf->lr_save;                  // function to call
    regs->gpr[13] = shim_fs_base_to_r13(fs_base); // TLS
    regs->nip = sf->lr_save;                      // function to call

    debug("child swapping stack to %p return 0x%lx\n", stack, regs->nip);
}
