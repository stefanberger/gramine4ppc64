/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2020 IBM Corporation */

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
