/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2020 IBM Corporation */

#ifndef PAL_LINUX_DEFS_H
#define PAL_LINUX_DEFS_H

#define USER_ADDRESS_LOWEST 0x10000

#define THREAD_STACK_SIZE (PRESET_PAGESIZE)
#define ALT_STACK_SIZE    (PRESET_PAGESIZE)

#define USE_VSYSCALL_GETTIME 1
#define USE_VDSO_GETTIME     0
#define USE_CLOCK_GETTIME    1

#define USE_ARCH_RDRAND 0

#define BLOCK_SIGFAULT 0

#include <asm/ioctl.h>
#define FIONREAD _IOR('f', 127, int)

#endif /* PAL_LINUX_DEFS_H */
