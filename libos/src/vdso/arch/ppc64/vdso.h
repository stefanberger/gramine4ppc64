/* SPDX-License-Identifier: LGPL-3.0-or-later */

#pragma once

#include "libos_types.h"

int c__kernel_gettimeofday(struct timeval* tv, struct timezone* tz);
int c__kernel_clock_gettime(clockid_t clock_id, struct timespec *tp);
int c__kernel_clock_getres(clockid_t clock_id, struct timespec *tp);
time_t c__kernel_time(time_t* t);
long c__kernel_getcpu(unsigned* cpu, struct getcpu_cache* unused);
