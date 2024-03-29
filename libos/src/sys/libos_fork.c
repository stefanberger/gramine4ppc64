/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2020 Intel Corporation
 *                    Borys Popławski <borysp@invisiblethingslab.com>
 */

#include "libos_table.h"
#include "linux_abi/process.h"
#include "linux_abi/signals.h"

long libos_syscall_fork(void) {
#if defined(__x86_64__)
    return libos_syscall_clone(SIGCHLD, 0, NULL, NULL, 0);
#elif defined(__powerpc64__)
    return libos_syscall_clone(SIGCHLD, 0, NULL, 0, NULL);
#endif
}

long libos_syscall_vfork(void) {
#if defined(__x86_64__)
    return libos_syscall_clone(CLONE_VFORK | CLONE_VM | SIGCHLD, 0, NULL, NULL, 0);
#elif defined(__powerpc64__)
    return libos_syscall_clone(CLONE_VFORK | CLONE_VM | SIGCHLD, 0, NULL, 0, NULL);
#endif
}
