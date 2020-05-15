/*
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
