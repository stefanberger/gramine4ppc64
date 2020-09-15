/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2022 IBM Corporation */

	.text
	.global call_elf_entry
	.type call_elf_entry, @function
	.align 0x2

// noreturn void call_elf_entry(elf_addr_t entry, void* argp)
call_elf_entry:
	mr	%r1, %r4
	mr	%r12, %r3
	mtctr	%r12
	bctr
