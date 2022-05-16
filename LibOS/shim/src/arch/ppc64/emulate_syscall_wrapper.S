/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2022 IBM Corporation */

	.text
	.global emulate_syscall_wrapper
	.type emulate_syscall_wrapper,@function
	.align 0x10

emulate_syscall_wrapper:
	.cfi_startproc

//	li	%r0, 0
//0:	cmpdi	%r0, 0
//	beq	0b

	addi	%r1, %r1, 48

	ld	%r2, 16(%r1)
	mtlr	%r2
	ld	%r2, 40(%r1)
	mtctr	%r2
	ld	%r2, 24(%r1)
	ld	%r12, 32(%r1)

	addi	%r1, %r1, 4096
	bctr

	.cfi_endproc
	.size	emulate_syscall_wrapper, .-emulate_syscall_wrapper
