/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2022 IBM Corporation */

#include <asm/unistd.h>

	.text
	.global test_exit
	.type test_exit, @function
	.align 0x10
test_exit:
	// On ppc64 we cannot currently use 'sc' to call syscall without
	// support for seccomp.

	li	%r0, __NR_exit
	b	do_syscall
