/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2022 IBM Corporation */

#include <asm/unistd.h>

	.text
	.global test_exit
	.type test_exit, @function
	.align 0x10
test_exit:
	li	%r0, __NR_exit
	sc
