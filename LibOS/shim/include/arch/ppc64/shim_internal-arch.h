/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2020 IBM Corporation */
/*
 * shim_internal-arch.h
 */

#ifndef _SHIM_INTERNAL_ARCH_H_
#define _SHIM_INTERNAL_ARCH_H_

#include "stack.h"

#define __SWITCH_STACK(stack_top, func, arg)                    \
    do {                                                        \
        /* 16 Bytes align of stack */                           \
        uintptr_t __stack_top = (uintptr_t)(stack_top);         \
        __stack_top &= ~0xf;                                    \
        __stack_top -= FRAME_MIN_SIZE_PARM;                     \
        memset((void *)__stack_top, 0, FRAME_MIN_SIZE_PARM);    \
        __asm__ volatile (                                      \
            "mr %%r1, %0\n\t"                                   \
            "mr %%r3, %2\n\t"                                   \
            "mr %%r12, %1\n\t"                                  \
            "mtctr %%r12\n\t"                                   \
            "bctr\n\t"                                          \
            :							\
            : "r"(__stack_top), "r"(func), "r"(arg)		\
            : "r3", "r12", "ctr", "memory");			\
    } while (0)

static_always_inline void* current_stack(void) {
    register void *sp __asm__("r1");

    return sp;
}

#endif /* _SHIM_INTERNAL_ARCH_H_ */
