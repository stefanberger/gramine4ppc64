/* Copyright (C) 2020 IBM Corporation
   Copyright (C) 2014 Stony Brook University
   This file is part of Graphene Library OS.

   Graphene Library OS is free software: you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License
   as published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   Graphene Library OS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/*
 * syscallas-ppc64.S
 *
 * This file contains the entry point of system call table in library OS.
 */

#include <shim_defs.h>
#include <shim_unistd_defs.h>

        .global syscalldb
        .type syscalldb, @function

# We are called with syscall number in r9, syscall parameters in r3-r8
# We pass them straight through to shim_table_dispatch & pointer to cr in r10
syscalldb:
        .cfi_startproc
0:	addis	%r2, %r12, (.TOC. - 0b)@ha
	addi	%r2, %r2, (.TOC. - 0b)@l

	mflr	%r0
	std	%r0,16(%r1)	# save lr
	mfcr	%r0
	std	%r0,8(%r1)	# save cr

	addi	%r10,%r1,8	# address of cr

	stdu	%r1,-32(%r1)	# @1 make room on stack

	bl	shim_table_dispatch
	nop

	addi	%r1,%r1,32

	ld	%r0,8(%r1)
	mtcr	%r0

	ld	%r0,16(%r1)	# restore lr
	mtlr 	%r0
	blr

	.cfi_endproc
        .size syscalldb, .-syscalldb

#if 0
        /*
         * syscall_wrapper: emulate syscall instruction
         *   prohibited in e.g. Linux-SGX PAL which raises a SIGILL exception
         * See illegal_upcall() @ shim_signal.c and
         *     fixup_child_context() @ shim_clone.c
         *
         * input:
         * %rcx: Instruction address to continue app execution after trapped
         *       syscall instruction
         * %r11: rflags on entering syscall
         */
syscall_wrapper:
        .cfi_startproc
        .cfi_def_cfa %rsp, 0
        # %rcx is used as input for returning %rip
        .cfi_register %rip, %rcx
        # %r11 is used as input to keep %rflags
        .cfi_register %rflags, %r11
        subq $RED_ZONE_SIZE, %rsp
        .cfi_adjust_cfa_offset RED_ZONE_SIZE
        callq *syscalldb@GOTPCREL(%rip)
syscall_wrapper_after_syscalldb:
        addq $RED_ZONE_SIZE, %rsp
        .cfi_adjust_cfa_offset -RED_ZONE_SIZE
        # restore %rflags for syscall abi compatibility.
        # This must be done after "addq $RED_ZONE_SIZE, %rsp" above
        # which destroys %rflags
        xchg %r11, (%rsp)
        .cfi_offset %rflags, 0
        popfq
        .cfi_adjust_cfa_offset -8
        .cfi_same_value %rflags
        pushq %r11
        .cfi_adjust_cfa_offset 8
        jmp *%rcx

        .cfi_endproc
        .size syscall_wrapper, .-syscall_wrapper
#endif
