/* SPDX-License-Identifier: LGPL-3.0-or-later */

/*
 * pal_start-ppc64.S
 *
 * This file contains architecture-specific implementation of pal_start.
 */

#include "sysdep-arch.h"
#include "stack.h"

	.text
	.global pal_start
	.type pal_start,@function

ENTRY(pal_start)
0:	addis %r2, %r12, (.TOC. - 0b)@ha
	addi  %r2, %r2, (.TOC. - 0b)@l

	.localentry pal_start,.-pal_start
	mr    %r3, %r1
	li    %r4, 0
	stdu  %r1, -FRAME_MIN_SIZE(%r1)
	b     pal_linux_main

END(pal_start)
