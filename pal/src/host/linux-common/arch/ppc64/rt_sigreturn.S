/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2022 IBM Corporation
 */

	.text
	.global rt_sigreturn
	.type rt_sigreturn, @function
	.align 0x2
rt_sigreturn:
	.cfi_startproc

	li	%r0, __NR_rt_sigreturn
	sc

	.cfi_endproc
	.size rt_sigreturn,.-rt_sigreturn;
