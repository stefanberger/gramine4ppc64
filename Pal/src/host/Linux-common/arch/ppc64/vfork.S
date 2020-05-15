/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2022 IBM Corportion
 */

#include <linux/sched.h>

	.global vfork
	.type vfork, @function
	.align 0x10
vfork:
	.cfi_startproc

	li	%r0, __NR_clone
	li	%r3, (CLONE_VM | CLONE_VFORK)
	li	%r4, 0
	li	%r5, 0
	li	%r6, 0
	sc
	blr

	.cfi_endproc
	.size vfork,.-vfork;
