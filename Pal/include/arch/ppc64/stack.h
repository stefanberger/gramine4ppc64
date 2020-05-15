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

#ifndef _PAL_STACK_H_
#define _PAL_STACK_H_

/* glibc's definitions of stack offsets */
#define FRAME_MIN_SIZE_PARM     96
#define FRAME_MIN_SIZE          32
#define FRAME_LR_SAVE           16
#define FRAME_TOC_SAVE          24
#define FRAME_PARM_SAVE	        32

#ifndef __ASSEMBLER__

struct stackframe {
    void *backchain;		// stack pointer points here
    uint64_t cr_save;		// 8(r1)
    uint64_t lr_save;		// 16(r1)
    uint64_t toc_save;		// 24(r1)
    uint64_t parm_save[];	// 32(r1)
};

#endif /* __ASSEMBLER__ */

#endif /* _PAL_STACK_H_ */
