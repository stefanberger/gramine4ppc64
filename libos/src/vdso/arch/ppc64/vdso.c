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
#include "gramine_entry_api.h"

int c__kernel_gettimeofday(struct timeval* tv, struct timezone* tz) {
    return gramine_syscall(__NR_gettimeofday, (long)tv, (long)tz, 0, 0, 0, 0);
}

int c__kernel_clock_gettime(clockid_t clock_id, struct timespec* tp) {
    return gramine_syscall(__NR_clock_gettime, (long)clock_id, (long)tp, 0, 0, 0, 0);
}

int c__kernel_clock_getres(clockid_t clock_id, struct timespec* tp) {
    return gramine_syscall(__NR_clock_getres, (long)clock_id, (long)tp, 0, 0, 0, 0);
}

time_t c__kernel_time(time_t* t) {
    return gramine_syscall(__NR_time, (long)t, 0, 0, 0, 0, 0);
}

long c__kernel_getcpu(unsigned* cpu, void* unused_cache) {
    return gramine_syscall(__NR_getcpu, (long)cpu, (long)unused_cache, 0, 0, 0, 0);
}
