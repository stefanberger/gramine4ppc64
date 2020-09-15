/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2020 IBM Corporation */

/*
 * shim_syscalls.h
 */

#pragma once

/* This is to help old header files where various syscall numbers are not
   available.  */
#if !defined __NR_clock_gettime64
#define __NR_clock_gettime64 403
#endif
#if !defined __NR_clock_getres_time64
#define __NR_clock_getres_time64 406
#endif
#if !defined __NR_clock_nanosleep_time64
#define __NR_clock_nanosleep_time64 407
#endif
#if !defined __NR_semtimedop_time64
#define __NR_semtimedop_time64 420
#endif
#if !defined __NR_futex_time64
#define __NR_futex_time64 422
#endif
#if !defined __NR_pidfd_send_signal
#define __NR_pidfd_send_signal 424
#endif
#if !defined __NR_io_uring_setup
#define __NR_io_uring_setup 425
#endif
#if !defined __NR_io_uring_enter
#define __NR_io_uring_enter 426
#endif
#if !defined __NR_io_uring_register
#define __NR_io_uring_register 427
#endif
#ifndef __NR_close_range
#define __NR_close_range 436
#endif
#ifndef __NR_openat2
#define __NR_openat2 437
#endif
#ifndef __NR_pidfd_getfd
#define __NR_pidfd_getfd 438
#endif
#ifndef __NR_faccessat2
#define __NR_faccessat2 439
#endif
#ifndef __NR_process_madvise
#define __NR_process_madvise 440
#endif
#ifndef __NR_epoll_pwait2
#define __NR_epoll_pwait2 441
#endif
#ifndef __NR_mount_setattr
#define __NR_mount_setattr 442
#endif
#ifndef __NR_quotactl_fd
#define __NR_quotactl_fd 443
#endif
#ifndef __NR_landlock_create_ruleset
#define __NR_landlock_create_ruleset 444
#endif
#ifndef __NR_landlock_add_rule
#define __NR_landlock_add_rule 445
#endif
#ifndef __NR_landlock_restrict_self
#define __NR_landlock_restrict_self 446
#endif
#ifndef __NR_memfd_secret
#define __NR_memfd_secret 447
#endif
#ifndef __NR_process_mrelease
#define __NR_process_mrelease 448
#endif
#ifndef __NR_futex_waitv
#define __NR_futex_waitv 449
#endif
#ifndef __NR_set_mempolicy_home_node
#define __NR_set_mempolicy_home_node 450
#endif
#ifndef __NR_syscalls
#define __NR_syscalls 451
#endif
