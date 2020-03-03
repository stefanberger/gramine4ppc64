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
#include "asm-offsets.h"

#define SAVE_GPRS(OFFSET, FRAME)			\
	std	%r0, OFFSET+SHIM_REGS_GPR0(FRAME);	\
	std	%r1, OFFSET+SHIM_REGS_GPR1(FRAME);	\
	std	%r2, OFFSET+SHIM_REGS_GPR2(FRAME);	\
	std	%r3, OFFSET+SHIM_REGS_GPR3(FRAME);	\
	std	%r4, OFFSET+SHIM_REGS_GPR4(FRAME);	\
	std	%r5, OFFSET+SHIM_REGS_GPR5(FRAME);	\
	std	%r6, OFFSET+SHIM_REGS_GPR6(FRAME);	\
	std	%r7, OFFSET+SHIM_REGS_GPR7(FRAME);	\
	std	%r8, OFFSET+SHIM_REGS_GPR8(FRAME);	\
	std	%r9, OFFSET+SHIM_REGS_GPR9(FRAME);	\
	std	%r10, OFFSET+SHIM_REGS_GPR10(FRAME);	\
	std	%r11, OFFSET+SHIM_REGS_GPR11(FRAME);	\
	std	%r12, OFFSET+SHIM_REGS_GPR11(FRAME)

#define LOAD_GPRS_R0_AND_R3_TO_R12(OFFSET, FRAME)	\
	ld	%r0, OFFSET+SHIM_REGS_GPR0(FRAME);	\
	ld	%r3, OFFSET+SHIM_REGS_GPR3(FRAME);	\
	ld	%r4, OFFSET+SHIM_REGS_GPR4(FRAME);	\
	ld	%r5, OFFSET+SHIM_REGS_GPR5(FRAME);	\
	ld	%r6, OFFSET+SHIM_REGS_GPR6(FRAME);	\
	ld	%r7, OFFSET+SHIM_REGS_GPR7(FRAME);	\
	ld	%r8, OFFSET+SHIM_REGS_GPR8(FRAME);	\
	ld	%r9, OFFSET+SHIM_REGS_GPR9(FRAME);	\
	ld	%r10, OFFSET+SHIM_REGS_GPR10(FRAME);	\
	ld	%r11, OFFSET+SHIM_REGS_GPR11(FRAME);	\
	ld	%r12, OFFSET+SHIM_REGS_GPR11(FRAME)

#define SAVE_NV_GPRS(OFFSET, FRAME)			\
	std	%r13, OFFSET+SHIM_REGS_GPR13(FRAME);	\
	std	%r14, OFFSET+SHIM_REGS_GPR14(FRAME);	\
	std	%r15, OFFSET+SHIM_REGS_GPR15(FRAME);	\
	std	%r16, OFFSET+SHIM_REGS_GPR16(FRAME);	\
	std	%r17, OFFSET+SHIM_REGS_GPR17(FRAME);	\
	std	%r18, OFFSET+SHIM_REGS_GPR18(FRAME);	\
	std	%r19, OFFSET+SHIM_REGS_GPR19(FRAME);	\
	std	%r20, OFFSET+SHIM_REGS_GPR20(FRAME);	\
	std	%r21, OFFSET+SHIM_REGS_GPR21(FRAME);	\
	std	%r22, OFFSET+SHIM_REGS_GPR22(FRAME);	\
	std	%r23, OFFSET+SHIM_REGS_GPR23(FRAME);	\
	std	%r24, OFFSET+SHIM_REGS_GPR24(FRAME);	\
	std	%r25, OFFSET+SHIM_REGS_GPR25(FRAME);	\
	std	%r26, OFFSET+SHIM_REGS_GPR26(FRAME);	\
	std	%r27, OFFSET+SHIM_REGS_GPR27(FRAME);	\
	std	%r28, OFFSET+SHIM_REGS_GPR28(FRAME);	\
	std	%r29, OFFSET+SHIM_REGS_GPR29(FRAME);	\
	std	%r30, OFFSET+SHIM_REGS_GPR30(FRAME);	\
	std	%r31, OFFSET+SHIM_REGS_GPR31(FRAME);

#define SAVE_SPRS(OFFSET, FRAME)			\
	mflr	%r0;					\
	std	%r0, OFFSET+SHIM_REGS_LINK(FRAME);	\
	std	%r0, OFFSET+SHIM_REGS_NIP(FRAME);	\
	mfctr	%r0;					\
	std	%r0, OFFSET+SHIM_REGS_CTR(FRAME);	\
	mfxer	%r0;					\
	std	%r0, OFFSET+SHIM_REGS_XER(FRAME);	\
	mfcr	%r0;					\
	std	%r0, OFFSET+SHIM_REGS_CCR(FRAME);

        .global syscalldb
        .type syscalldb, @function

	.global shim_table_dispatch
	.type shim_table_dispatch, @function

	.global shim_set_context_regs
	.type shim_set_context_regs, @function

# We are called with syscall number in r9, syscall parameters in r3-r8
# We pass them straight through to shim_table_dispatch & pointer to cr in r10
syscalldb:
        .cfi_startproc
0:
	# OPTIMIZE: we likely only need to store the registers
	#           for a few syscalls related to fork/vfork/clone
	#           and skip over most of this in other cases

	# before we clobber any registers, we store them on the stack
	SAVE_GPRS(-SHIM_REGS_SIZE, %r1)
	SAVE_NV_GPRS(-SHIM_REGS_SIZE, %r1)
	# we now clobber r0 but this should be ok
	SAVE_SPRS(-SHIM_REGS_SIZE, %r1)
	# we only clobbered r0

	addis	%r2, %r12, (.TOC. - 0b)@ha
	addi	%r2, %r2, (.TOC. - 0b)@l

	mflr	%r0
	std	%r0,16(%r1)			# save lr
	mfcr	%r0
	std	%r0,8(%r1)			# save cr

	# make room on stack for pt_regs we stored above
	stdu	%r1,-SHIM_REGS_SIZE-8-32(%r1)	# @1: make room on stack
						# SHIM_REGS_SIZE: pt_regs
						#  8: r3 temp storage
						# 32: for normal frame
	# pt_regs are now at %r1+32+8
	# call into c code to set the TCB's context.regs
	addi	%r3, %r1, 32+8
	bl	shim_set_context_regs
	nop
	# restore the registers the function may have clobbered
	LOAD_GPRS_R0_AND_R3_TO_R12(32+8, %r1)

	addi	%r10,%r1,32+8+SHIM_REGS_SIZE+8	# address of cr

	bl	shim_table_dispatch
	nop

	# we can now reset the context.regs to NULL
	# need to save %r3
	std	%r3,32(%r1)
	li	%r3, 0
	bl	shim_set_context_regs
	nop

	ld	%r3,32(%r1)

	addi	%r1,%r1,32+8+SHIM_REGS_SIZE	# undo @1

	ld	%r0,8(%r1)			# get cr modified by
						# shim_table_dispatch
	mtcr	%r0

	ld	%r0,16(%r1)			# restore lr
	mtlr 	%r0
	blr

	.cfi_endproc
        .size syscalldb, .-syscalldb
