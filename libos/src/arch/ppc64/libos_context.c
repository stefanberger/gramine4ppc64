/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2022 IBM Corporation */

/*
 * libos_context.c
 *
 * This file contains code for ppc64-specific CPU context manipulation.
 */

#include <asm/unistd.h>
#include <stdalign.h>
#include <stdnoreturn.h>

#include "pal.h"
#include "libos_context.h"
#include "libos_internal.h"
#include "libos_thread.h"
#include "stack.h"
#include "ucontext.h"
#include "libos_table_arch.h"
#include "rt_sigreturn_wrapper.h"

noreturn void libos_return_with_context(PAL_CONTEXT* context);

/* Jump back a function by restoring the context from PAL_CONTEXT.
 *
 * We jump back to where the code would expect to resume via the 'nip'
 * (next instruction pointer), which typically is stored via the link
 * register. In most cases this will bring us back to the memory
 * location after the one that jumped to libos_syscall_entry. We captured the
 * context in libos_syscall_entry and while executing the syscall we manipulated
 * a few registers in the context, such as r3 and cr, to be able to
 * return the result of a syscall.
 */
static noreturn void return_with_context(PAL_CONTEXT* context) {
    libos_return_with_context(context);
}

/* Glibc passes the thread's function to call and the parameter via registers
 * used for 'backup' purposes.
 */
static void fixup_child_context_thread(PAL_CONTEXT* context, uint64_t r13) {
    // glibc's clone.s saved the function in r31 and the parameter in r30
    context->gpregs.gpr[3] = context->gpregs.gpr[31];         // parameter to function
    context->gpregs.gpr[12] = context->gpregs.gpr[30];        // function to call
    context->gpregs.gpr[13] = r13;                            // TLS
    context->gpregs.nip = context->gpregs.gpr[30];            // function to call

    log_debug("child swapping stack to 0x%lx return 0x%lx",
              context->gpregs.gpr[1], context->gpregs.nip);
}

/* A child process needs to return 0 on the fork() or clone() call to indicate child.
 */
static void fixup_child_context_process(PAL_CONTEXT* context) {
    context->gpregs.gpr[3] = 0;
    context->gpregs.ccr &= ~CR0_SO;
}

noreturn void restore_child_context_after_clone(struct libos_context* context, bool is_process) {
    assert(context->regs);

    uint64_t tls = context->tls;

    if (is_process)
        fixup_child_context_process(context->regs);
    else
        fixup_child_context_thread(context->regs, tls);

    context->syscall_nr = -1;

    // the process we will resume points to an old PAL_TCB via r13; its
    // glibc_tcb.LibOS_TCB is inaccessible and so we need to update it
    // to the one we are using here
    libos_r13_set_tcb(context->regs->gpregs.gpr[13], libos_get_tcb());

    PAL_CONTEXT* regs = context->regs;
    context->regs = NULL;

#if 0
    log_debug("restore context: SP = 0x%08lx, IP = 0x%08lx, R2 = 0x%lx, R3 = 0x%lx, R4 = 0x%lx, R5 = 0x%lx, R12 = 0x%lx, R13 = 0x%lx",
          regs->gpregs.gpr[1], regs->gpregs.nip, regs->gpregs.gpr[2],
          regs->gpregs.gpr[3], regs->gpregs.gpr[4], regs->gpregs.gpr[5],
          regs->gpregs.gpr[12], regs->gpregs.gpr[13]);
    log_debug("restore context: vrsave: 0x%lx fpscr: 0x%lx vscr: 0x%08lx%08lx",
          regs->xtregs.vrsave, regs->xtregs.fpscr,
          regs->xtregs.vscr.a, regs->xtregs.vscr.b);
    struct stackframe* sf = (void *)regs->gpregs.gpr[1];
    log_debug("Resume: Stack of child: sp: %p lr_save: 0x%lx  toc_save: 0x%lx",
          sf, sf->lr_save, sf->toc_save);
#endif

    /* context is all set up for process and thread to directly return */
    return_with_context(regs);
}

noreturn void _return_from_syscall(PAL_CONTEXT* context) {
    /* a special case is the syscall restart; we must not clobber gpr3 ! */
    if (context->gpregs.nip == (unsigned long)&libos_syscall_entry)
        return_with_context(context);

    int64_t* ret = (int64_t *)&context->gpregs.gpr[3];
    uint64_t* ccr = &context->gpregs.ccr;

    if (*ret < 0) {
        *ret = -(*ret);
        *ccr |= CR0_SO;
    } else {
        *ccr &= ~CR0_SO;
    }

    return_with_context(context);
}

noreturn void rt_sigreturn_caller(PAL_CONTEXT* context);
noreturn void rt_sigreturn_caller(PAL_CONTEXT* context) {
    /* synthesize a call to rt_sigreturn; do NOT call function directly! */
    pal_context_set_syscall(context, __NR_rt_sigreturn);
    libos_syscall_table_dispatch(context);
}

struct sigframe {
    ucontext_t uc;
    siginfo_t siginfo;
};

void prepare_sigframe(PAL_CONTEXT* context, siginfo_t* siginfo, void* handler, void* restorer,
                      bool should_use_altstack, __sigset_t* old_mask) {
    struct libos_thread* current = get_cur_thread();

    uint64_t stack = get_stack_for_sighandler(context->gpregs.gpr[1], should_use_altstack);

    struct sigframe* sigframe = NULL;
    stack = ALIGN_DOWN(stack - sizeof(*sigframe), alignof(*sigframe));
    stack = ALIGN_DOWN(stack, 0x10);
    /* Make sure `stack` is now aligned to both `alignof(*sigframe)` and 0x10. */
    static_assert(alignof(*sigframe) % 0x10 == 0 || 0x10 % alignof(*sigframe) == 0,
                  "Incorrect sigframe alignment");
    sigframe = (struct sigframe*)stack;
    /* This could probably be omited as we set all fields explicitly below. */
    memset(sigframe, 0, sizeof(*sigframe));

    sigframe->siginfo = *siginfo;

    sigframe->uc.uc_flags = 0;
    sigframe->uc.uc_link = NULL;
    sigframe->uc.uc_stack = current->signal_altstack;

    pal_context_to_ucontext(&sigframe->uc, context);

    sigframe->uc.uc_sigmask = *old_mask;

    /* Below the sigframe is our function stack frame;
     * We need a 'struct stackframe' with 1 parameter
     */
    stack = ALIGN_DOWN(stack - sizeof(struct stackframe) - 8, alignof(struct stackframe));
    stack = ALIGN_DOWN(stack, 0x10);

    context->gpregs.gpr[1] = stack;
    /* parameters to signal handler 'sa_sigaction' */
    context->gpregs.gpr[3] = (uint64_t)siginfo->si_signo;
    context->gpregs.gpr[4] = (uint64_t)&sigframe->siginfo;
    context->gpregs.gpr[5] = (uint64_t)&sigframe->uc;
    /* the signal handler to call */
    context->gpregs.nip = (uint64_t)handler;
    context->gpregs.gpr[12] = (uint64_t)handler;

    context->gpregs.link = (uint64_t)rt_sigreturn_wrapper;

    /* build a stackframe for rt_sigreturn_wrapper to get r2 and parameter from */
    struct stackframe* stackframe = (struct stackframe*)stack;
    register uint64_t r2 __asm__("r2");
    stackframe->toc_save = r2;
    stackframe->parm_save[0] = (uint64_t)context;
    log_debug("Created sigframe for sig: %d at %p (handler: %p, restorer: %p, context: %p)",
              siginfo->si_signo, sigframe, handler, restorer, context);
}

void restore_sigreturn_context(PAL_CONTEXT* context, __sigset_t* new_mask) {
    uint64_t stack = context->gpregs.gpr[1];
    stack = ALIGN_UP(stack + sizeof(struct stackframe) + 8, alignof(struct stackframe));
    stack = ALIGN_UP(stack, 0x10);
    struct sigframe* sigframe = (struct sigframe*)stack;
    log_debug("Restoring sigframe at %p (context: %p)", sigframe, context);

    *new_mask = sigframe->uc.uc_sigmask;

    ucontext_to_pal_context(context, &sigframe->uc);
}

void restart_syscall(PAL_CONTEXT* context, uint64_t sysnr) {
    context->gpregs.gpr[3] = context->gpregs.orig_gpr3;
    pal_context_set_syscall(context, sysnr);
    context->gpregs.nip = (uint64_t)&libos_syscall_entry;
}

/*
 * Restore registers saved in setup_syscall_emulation and restore stack
 * to return to applcation right after its 'sc' call.
 */
static void finish_return_from_syscall_emulation(PAL_CONTEXT* context) {
    uint64_t stack = context->gpregs.gpr[1];

    stack += FRAME_MIN_SIZE;

    struct stackframe* stackframe = (struct stackframe*)stack;
    context->gpregs.link = stackframe->lr_save;
    context->gpregs.gpr[2] = stackframe->toc_save;
    context->gpregs.gpr[12] = stackframe->parm_save[0];
    context->gpregs.nip = stackframe->parm_save[1];
    context->gpregs.ctr = stackframe->parm_save[2];

    stack += RED_ZONE_SIZE;
    context->gpregs.gpr[1] = stack;
}

/*
 * Prepare for emulation of an sc instruction by storing non-volatile
 * registers onto the stack that would otherwise get clobbered.
 * Volatile registers for a syscall are: r0, r3-r8, cr0
 *
 * The emulation of 'sc' is done via libos_syscall_entry, which stores and
 * restores the whole context 'around' the syscall emulation, except
 * for 'ctr', so we need to store 'ctr' here as well.
 */
static void setup_syscall_emulation(PAL_CONTEXT* context) {
    uint32_t* nip = (uint32_t*)context->gpregs.nip;

    /* protect the app's red zone that normal sc handling may leave alone */
    uint64_t stack = context->gpregs.gpr[1];
    stack -= RED_ZONE_SIZE;

    /* The stack frame setup here is undone in finish_return_from_syscall_emulation */
    struct stackframe* stackframe = (struct stackframe*)stack;
    stackframe->backchain = (void*)context->gpregs.gpr[1];
    /* link holds the link register at the point when app ran 'sc' instruction */
    stackframe->lr_save = context->gpregs.link;
    stackframe->toc_save = context->gpregs.gpr[2];
    /* store r12 that we need to clobber */
    stackframe->parm_save[0] = context->gpregs.gpr[12];
    /* nip + 4 is pointer to after the 'sc' instruction */
    stackframe->parm_save[1] = (uint64_t)nip + 4;
    stackframe->parm_save[2] = context->gpregs.ctr;

    stack -= FRAME_MIN_SIZE;

    /*
     * After emulation of the syscall resume in return_form_syscall_emulation
     * which runs a custom syscall via 'sc' so we an restore all registers in
     * a signal handler in finish_return_from_syscall_emulation above.
     */
    context->gpregs.link = (uint64_t)return_from_syscall_emulation;
    pal_context_set_syscall(context, context->gpregs.gpr[0]);
    context->gpregs.gpr[1] = stack;
    context->gpregs.nip = (uint64_t)&libos_syscall_entry;
    context->gpregs.gpr[12] = (uint64_t)&libos_syscall_entry;
}

bool maybe_emulate_syscall(PAL_CONTEXT* context) {
    uint32_t* nip = (uint32_t*)context->gpregs.nip;

    if (INSN_IS_SC(nip[0])) {
        switch (context->gpregs.gpr[0]) {
        case GRAMINE_CUSTOM_SYSCALL_RETURN_FROM_SYSCALL_EMULATION:
            finish_return_from_syscall_emulation(context);
            break;
        case GRAMINE_CUSTOM_SYSCALL_NR:
            die_or_inf_loop();
            break;
        default:
            setup_syscall_emulation(context);
        }
        return true;
    }
    return false;
}

void libos_xstate_init(void) {
}
