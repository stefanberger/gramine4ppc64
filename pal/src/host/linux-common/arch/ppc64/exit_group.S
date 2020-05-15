/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2022 IBM Corportion
 */
	.global exit_group
	.type exit_group, @function
	.align 0x2
exit_group:
	.cfi_startproc

	li 	%r0, __NR_exit_group
	sc

	.cfi_endproc
	.size exit_group,.-exit_group;
