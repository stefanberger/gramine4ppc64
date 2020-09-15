/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2022 IBM Corporation */

/* Check that the stack is 16-byte aligned */
	.text
	.global _start
	.type _start, @function
	.align 0x10
_start:
	mr	%r3, %r1
	andi.	%r3, %r3, 0xf
	b	test_exit
