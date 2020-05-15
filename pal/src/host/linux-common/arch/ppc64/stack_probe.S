/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2022 IBM Corportion
 */

// void probe_stack(size_t pages_count)
	.global	probe_stack
	.type probe_stack, @function
	.align 0x2
probe_stack:
	.cfi_startproc

	mr	%r4, %r1
0:
	subis	%r4, %r4, (0x10000 >> 16)
	ld	%r5, 0(%r4)
	subi	%r3, %r3, 1

	cmpdi	%cr7, %r3, 0
	bne	%cr7, 0b

	blr

	.cfi_endproc
	.size probe_stack,.-probe_stack
