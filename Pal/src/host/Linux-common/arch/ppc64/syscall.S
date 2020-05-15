/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2021 IBM Corportion
 */

#define __ASSEMBLY__
#include <asm/unistd.h>

	.global gramine_raw_syscalls_code_begin
	.global gramine_raw_syscalls_code_end
	.type gramine_raw_syscalls_code_begin, @function
	.type gramine_raw_syscalls_code_end, @function
gramine_raw_syscalls_code_begin:

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
	.size do_syscall,.-do_syscall;


	.global _DkThreadExit_asm_stub
	.type _DkThreadExit_asm_stub, @function
	.align 0x10
_DkThreadExit_asm_stub:
	.cfi_startproc

	// r3: &g_thread_stack_lock.lock [int *]
	// r4: clear_child_tid [int *]

	li	%r5, 0
	stw	%r5, 0(%r3)
	cmpdi	cr7, %r4, 0
	beq	cr7, 0f
	stw	%r5, 0(%r4)
0:
	li	%r3, 0
	li	%r0, __NR_exit
	sc

	.cfi_endproc
	.size _DkThreadExit_asm_stub,.-_DkThreadExit_asm_stub;

gramine_raw_syscalls_code_end:
