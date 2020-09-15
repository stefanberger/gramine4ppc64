/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2018 Intel Corporation
 *                    Isaku Yamahata <isaku.yamahata at gmail.com>
 *                                   <isaku.yamahata at intel.com>
 * Copyright (C) 2020 Intel Corporation
 *                    Borys Popławski <borysp@invisiblethingslab.com>
 *
 * Copyright (C) 2022 IBM Corporation
 *
 */

#include <asm/unistd.h>

#include "vdso.h"
#include "vdso_syscall.h"

int c__kernel_gettimeofday(struct timeval* tv, struct timezone* tz) {
    return vdso_arch_syscall(__NR_gettimeofday, (long)tv, (long)tz);
}

int c__kernel_clock_gettime(clockid_t clock_id, struct timespec* tp) {
    return vdso_arch_syscall(__NR_clock_gettime, (long)clock_id, (long)tp);
}

int c__kernel_clock_getres(clockid_t clock_id, struct timespec* tp) {
    return vdso_arch_syscall(__NR_clock_getres, (long)clock_id, (long)tp);
}

time_t c__kernel_time(time_t* t) {
    return vdso_arch_syscall(__NR_time, (long)t, 0);
}

long c__kernel_getcpu(unsigned* cpu, struct getcpu_cache* unused) {
    return vdso_arch_syscall(__NR_getcpu, (long)cpu, (long)unused);
}
