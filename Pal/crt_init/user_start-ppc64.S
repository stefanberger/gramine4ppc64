/* Startup code for programs linked with GNU libc.  PowerPC64 version.
   Copyright (C) 1998-2020 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
   In addition to the permissions in the GNU Lesser General Public
   License, the Free Software Foundation gives you unlimited
   permission to link the compiled version of this file with other
   programs, and to distribute those programs without any restriction
   coming from the use of this file. (The GNU Lesser General Public
   License restrictions do apply in other respects; for example, they
   cover modification of the file, and distribution when not linked
   into another program.)
   Note that people who make modified versions of this file are not
   obligated to grant this special exception for their modified
   versions; it is their choice whether to do so. The GNU Lesser
   General Public License gives permission to release a modified
   version without this exception; this exception also makes it
   possible to release a modified version which carries forward this
   exception.
   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

#define ASM_SIZE_DIRECTIVE(name) .size name,.-name

#define FUNC_LABEL(X) X
#define JUMPTARGET(name) FUNC_LABEL(name)

#ifdef PIC
	.section ".data.rel.ro.local","aw"
#else
	.section ".rodata"
#endif

	.text
	.globl _start
	.type _start,@function
_start:
0:	addis	%r2, %r12, (.TOC. - 0b)@ha
	addi	%r2, %r2, (.TOC. - 0b)@l

 /* r3 holds the array to the cookies which looks like this:
    cookies[0]      = argc
    cookies[1]      = argv[0]
    ...
    cookies[argc]   = argv[arc-1]
    cookies[argc+1] = env[0]
    ...
    cookies[argc+x] = env[x]

 */
	mflr	%r0
	std	%r0, 16(%r1)
	std	%r31, -8(%r1)

	mr	%r6, %r3	// pointer to cookies
	ld	%r3, 0(%r6)	// argc
	addi	%r4, %r6, 8	// argv

	slwi	%r5, %r3, 3
	add	%r5, %r4, %r5
	addi	%r5, %r5, 8	// envp

	stdu	%r1, -32(%r1)	// room on stack

	mr	%r31, %r1	// save our stack pointer
 
	bl	main
	nop

	addi	%r1, %r31, 32	// restore original stack
	ld	%r31, -8(%r1)
	ld	%r0, 16(%r1)
	mtlr	%r0
	blr
