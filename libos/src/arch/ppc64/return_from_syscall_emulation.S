/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2022 IBM Corporation */

#include "gramine_entry_api.h"

	.text
	.global return_from_syscall_emulation
	.type return_from_syscall_emulation,@function
	.align 0x10

return_from_syscall_emulation:
	.cfi_startproc

	// To be able to restore all registers we need to be in a signal
	// handler, so let's get us into a signal handler via a syscall.
	// Otherwise we would either have to clobber ctr or lr to be able
	// jump to the location after the original sc instruction. Both
	// must be preserved in a syscall though.
	li	%r0, (GRAMINE_CUSTOM_SYSCALL_RETURN_FROM_SYSCALL_EMULATION & 0xffff)
	oris	%r0, %r0, (GRAMINE_CUSTOM_SYSCALL_RETURN_FROM_SYSCALL_EMULATION >> 16)
	sc

	.cfi_endproc
	.size	return_from_syscall_emulation, .-return_from_syscall_emulation
