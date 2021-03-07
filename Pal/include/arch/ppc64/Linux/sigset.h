#ifndef __LINUX_PPC64_SIGSET_H__
#define __LINUX_PPC64_SIGSET_H__

/* __sig_atomic_t, __sigset_t, and related definitions.  Linux version.
   Copyright (C) 1991, 1992, 1994, 1996, 1997, 2007
   Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */



#define _SIGSET_H_types 1
#define _SIGSET_H_fns   1

#define ____sigset_t_defined

typedef int __sig_atomic_t;

/* A `sigset_t' has a bit for each signal.  */

/* user space only has 64 signals; kernel may have 1024 */
#define _SIGSET_NWORDS (64 / (8 * sizeof(unsigned long int)))

typedef struct {
    unsigned long int __val[_SIGSET_NWORDS];
} __sigset_t;

/* Return a mask that includes the bit for SIG only.  */
#define __sigmask(sig) (((unsigned long int)1) << (((sig) - 1) % (8 * sizeof(unsigned long int))))

/* Return the word index for SIG.  */
#define __sigword(sig) (((sig) - 1) / (8 * sizeof(unsigned long int)))

#define __sigemptyset(set)                  \
    ({                                      \
        int __cnt         = _SIGSET_NWORDS; \
        __sigset_t* __set = (set);          \
        while (--__cnt >= 0) {              \
            __set->__val[__cnt] = 0;        \
        }                                   \
        0;                                  \
    })
#define __sigfillset(set)                   \
    ({                                      \
        int __cnt         = _SIGSET_NWORDS; \
        __sigset_t* __set = (set);          \
        while (--__cnt >= 0) {              \
            __set->__val[__cnt] = ~0UL;     \
        }                                   \
        0;                                  \
    })
#define __sigcopyset(set, src)                         \
    ({                                                 \
        int __cnt         = _SIGSET_NWORDS;            \
        __sigset_t* __set = (set);                     \
        __sigset_t* __src = (src);                     \
        while (--__cnt >= 0) {                         \
            __set->__val[__cnt] = __src->__val[__cnt]; \
        }                                              \
        0;                                             \
    })

/* These functions needn't check for a bogus signal number -- error
   checking is done in the non __ versions.  */

#define __SIGSETFN(NAME, BODY, CONST)                            \
    static inline int NAME(CONST __sigset_t* __set, int __sig) { \
        unsigned long int __mask = __sigmask(__sig);             \
        unsigned long int __word = __sigword(__sig);             \
        return BODY;                                             \
    }

__SIGSETFN(__sigismember, (__set->__val[__word] & __mask) ? 1 : 0, __const)
__SIGSETFN(__sigaddset, ((__set->__val[__word] |= __mask), 0), )
__SIGSETFN(__sigdelset, ((__set->__val[__word] &= ~__mask), 0), )

#undef __SIGSETFN

#include "sysdep-arch.h"
#include <asm/signal.h>

static_assert(sizeof(__sigset_t) == 8, "sizeof(__sigset_t) must be 8");

// https://elixir.bootlin.com/linux/v5.11.3/source/include/linux/compat.h#L142
struct compat_sigaction {
    void (*sa_handler)(int);
    unsigned long sa_flags;
    void (*sa_restorer)(void); // ppc64 has it but doesn't see to use it
    __sigset_t sa_mask;
};
static_assert(sizeof(struct compat_sigaction) == 32, "sizeof(compat_sigaction) must be 32");

// rt_sigprocmask: https://elixir.bootlin.com/linux/v5.11.3/source/kernel/signal.c#L3084
static inline int arch_do_rt_sigprocmask(int how, int sig) {
    __sigset_t mask;
    __sigemptyset(&mask);
    __sigaddset(&mask, sig);

    return INLINE_SYSCALL(rt_sigprocmask, 4, how, &mask, NULL, sizeof(__sigset_t) /* = 8 */);
}

// rt_sigaction:   https://elixir.bootlin.com/linux/v5.11.3/source/kernel/signal.c#L4313
static inline int arch_do_rt_sigaction(int sig, void* handler,
                                       const int* async_signals, size_t num_async_signals,
                                       const void* restore_rt) {
    __UNUSED(restore_rt);
    struct compat_sigaction action = {0};

    action.sa_handler = handler;
    action.sa_flags   = SA_SIGINFO | SA_ONSTACK;

    /* disallow nested asynchronous signals during exception handling */
    __sigemptyset(&action.sa_mask);
    for (size_t i = 0; i < num_async_signals; i++)
        __sigaddset(&action.sa_mask, async_signals[i]);

    return INLINE_SYSCALL(rt_sigaction, 4, sig, &action, NULL, sizeof(__sigset_t) /* = 8 */);
}

#endif /* __LINUX_PPC64_SIGSET_H__  */
