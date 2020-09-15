/* SPDX-License-Identifier: LGPL-3.0-or-later */
/*
 * Copyright (C) 2022 IBM Corporation
 *
 */

#define VDSO_FUNC(FNAME)					\
	.align 2;						\
	.globl __kernel_##FNAME;				\
	.type   __kernel_##FNAME, @function;			\
__kernel_##FNAME:						\
	.cfi_startproc;						\
								\
0:	mr 	%r12, %r0;		/* VDSO interface */	\
	mflr	%r0;						\
	std	%r0, 16(%r1);		/* save lr */		\
	std	%r2, 8(%r1);		/* save r2 */		\
	stdu	%r1, -32(%r1);					\
								\
	addis   %r2, %r12, (.TOC. - 0b)@ha;			\
	addi    %r2, %r2, (.TOC. - 0b)@l;			\
								\
	bl	c__kernel_##FNAME;				\
								\
	addi	%r1, %r1, 32;					\
								\
	ld	%r2, 8(%r1);		/* restore r2 */	\
	ld	%r0, 16(%r1);		/* restore lr */	\
	mtlr	%r0;						\
								\
	blr;							\
								\
	.cfi_endproc;						\
	.size   __kernel_##FNAME,.-__kernel_##FNAME;		\
	.weak   #FNAME;						\
	.set    #FNAME,__kernel_##FNAME;

	VDSO_FUNC(gettimeofday)
	VDSO_FUNC(clock_gettime)
	VDSO_FUNC(clock_getres)
	VDSO_FUNC(time)
	VDSO_FUNC(getcpu)
