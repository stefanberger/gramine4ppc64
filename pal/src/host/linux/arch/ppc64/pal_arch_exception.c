/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2021 IBM Corporation
 */

#include <assert.h>
#include <stddef.h>
#include <asm/signal.h>

#include "pal.h"
#include "pal_host.h"
#include "sigset.h"
#include "syscall.h"

static_assert(sizeof(__sigset_t) == 8, "sizeof(__sigset_t) must be 8");

// https://elixir.bootlin.com/linux/v5.11.3/source/include/linux/compat.h#L142
struct compat_sigaction {
    void (*sa_handler)(int);
    unsigned long sa_flags;
    void (*sa_restorer)(void); // ppc64 has it but doesn't seem to use it
    __sigset_t sa_mask;
};
static_assert(sizeof(struct compat_sigaction) == 32, "sizeof(compat_sigaction) must be 32");

// rt_sigprocmask: https://elixir.bootlin.com/linux/v5.11.3/source/kernel/signal.c#L3084
int arch_do_rt_sigprocmask(int sig, int how) {
    __sigset_t mask;
    __sigemptyset(&mask);
    __sigaddset(&mask, sig);

    return DO_SYSCALL(rt_sigprocmask, how, &mask, NULL, sizeof(__sigset_t) /* = 8 */);
}

// rt_sigaction:   https://elixir.bootlin.com/linux/v5.11.3/source/kernel/signal.c#L4313
int arch_do_rt_sigaction(int sig, void* handler,
                         const int* async_signals, size_t num_async_signals) {
    struct compat_sigaction action = {
         .sa_handler = handler,
         .sa_flags   = SA_SIGINFO | SA_ONSTACK,
    };

    /* disallow nested asynchronous signals during exception handling */
    __sigemptyset(&action.sa_mask);
    for (size_t i = 0; i < num_async_signals; i++)
        __sigaddset(&action.sa_mask, async_signals[i]);

    return DO_SYSCALL(rt_sigaction, sig, &action, NULL, sizeof(__sigset_t) /* = 8 */);
}

