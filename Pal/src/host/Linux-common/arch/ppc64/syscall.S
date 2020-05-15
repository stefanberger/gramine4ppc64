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

#include "clone.S"
#include "exit_group.S"
#include "do_syscall.S"
#include "rt_sigreturn.S"
#include "vfork.S"

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

	nop

	.cfi_endproc
	.size _DkThreadExit_asm_stub,.-_DkThreadExit_asm_stub;

gramine_raw_syscalls_code_end:
