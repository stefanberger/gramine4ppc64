/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2021 IBM Corportion
 */

	.global do_syscall
	.type do_syscall, @function
	.align 0x10
do_syscall:
	.cfi_startproc
	mr	%r0, %r3
	mr	%r3, %r4
	mr	%r4, %r5
	mr	%r5, %r6
	mr	%r6, %r7
	mr	%r7, %r8
	mr	%r8, %r9
	sc
	mfcr	%r0
	andis.	%r9, %r0, 4096
	beq	0f
	neg	%r3, %r3
0:
	blr
	.cfi_endproc
	.size do_syscall,.-do_syscall
