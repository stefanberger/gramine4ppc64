/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2020 IBM Corporation */
/*
 * shim_internal-arch.h
 */

#pragma once

#include <stdnoreturn.h>

#include "pal.h"
#include "gramine_entry_api.h"
#include "stack.h"

#define __SWITCH_STACK(stack_top, func, arg1, arg2)             \
    do {                                                        \
        /* 16 Bytes align of stack */                           \
        uintptr_t __stack_top = (uintptr_t)(stack_top);         \
        __stack_top &= ~0xf;                                    \
        __stack_top -= FRAME_MIN_SIZE_PARM;                     \
        memset((void *)__stack_top, 0, FRAME_MIN_SIZE_PARM);    \
        __asm__ volatile (                                      \
            "mr %%r1, %0\n\t"                                   \
            "mr %%r3, %2\n\t"                                   \
            "mr %%r4, %3\n\t"                                   \
            "mr %%r12, %1\n\t"                                  \
            "mtctr %%r12\n\t"                                   \
            "bctr\n\t"                                          \
            :							\
            : "r"(__stack_top), "r"(func), "r"(arg1), "r"(arg2)	\
            : "r3", "r12", "ctr", "memory");			\
        __builtin_unreachable();                                \
    } while (0)

#define LIBOS_ELF_HOST_MACHINE EM_PPC64

noreturn void return_from_syscall_emulation(void);

/* On ppc64 there's one syscall that may be executed from within libos */
static inline bool is_valid_libos_syscall(PAL_CONTEXT *context) {
    return (INSN_IS_SC(*(uint32_t *)context->gpregs.nip) &&
        context->gpregs.gpr[0] == GRAMINE_CUSTOM_SYSCALL_RETURN_FROM_SYSCALL_EMULATION);
}
