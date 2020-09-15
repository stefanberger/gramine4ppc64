/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2020 IBM Corporation */

/*
 * shim_syscalls.h
 */

#ifndef SHIM_SYSCALLS_H_
#define SHIM_SYSCALLS_H_

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
#ifndef __NR_syscalls
#define __NR_syscalls 428
#endif

#endif /* SHIM_SYSCALLS_H_ */
