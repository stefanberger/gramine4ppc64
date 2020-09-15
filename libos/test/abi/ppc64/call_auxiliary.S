// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2022 Intel Corporation
//                    Mariusz Zaborski <oshogbo@invisiblethingslab.com>
// Copyright (C) 2022 IBM Corporation

// Verify auxiliary which should be in:
// 'rsp + 8 + (8 * (argc + 1)) + (8 * (env_count + 1))'
// In this test we don't pass any additional arguments so argc is 1,
// we also should not have any environment variables so env_count is 0.

#define    argc      1
#define    env_count 0

	.text
	.global _start
	.type _start, @function
	.align 0x2

_start:
0:	addis   %r2, %r12, (.TOC. - 0b)@ha
	addi    %r2, %r2, (.TOC. - 0b)@l

	addi	%r3, %r1, 8 + 8 * (argc + 1) + 8 * (env_count + 1)

	bl	verify_auxiliary

	b	test_exit
