/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2022 IBM Corporation */

#include "gramine_entry_api.h"

	.text
	.global do_syscall
	.type do_syscall, @function
	.align 0x10
do_syscall:
	// This is from glibc's patched syscall function calling syscall via
	// %r9.
	mr	%r9, %r0

	// call syscalldb using tcbhead_t->LibOS_TCB, which gives us
	// PAL_TCB*
	subi	%r12, %r13, 0x7000 + TCBHEAD_LIBOS_PTR_FROM_END_OFFSET
	// pointer to PAL_TCB with tcbhead_t
	ld	%r12, 0(%r12)
	// out special offset in PAL_TCB containing syscalldb address
	ld	%r12, GRAMINE_SYSCALL_OFFSET(%r12)
	// function address must be in r12
	mtctr	%r12
	bctr
