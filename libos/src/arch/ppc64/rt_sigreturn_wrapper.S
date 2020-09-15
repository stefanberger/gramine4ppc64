/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2022 IBM Corporation */

	.text
	.globl rt_sigreturn_wrapper
	.type rt_sigreturn_wrapper,@function
	.align 0x10

rt_sigreturn_wrapper:
	.cfi_startproc
	ld	%r2, 24(%r1)	// restore r2
	ld	%r3, 32(%r1)	// restore context
	b	rt_sigreturn_caller
	.cfi_endproc
	.size	rt_sigreturn_wrapper, .-rt_sigreturn_wrapper
