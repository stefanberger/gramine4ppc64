/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2020 IBM Corporation */

/*
 * pal_host-arch.h
 *
 * This file contains Linux on ppc64 specific functions related to the PAL.
 */

#ifndef __LINUX_PPC64_PAL_HOST_ARCH_H__
#define __LINUX_PPC64_PAL_HOST_ARCH_H__

/* glibc's define for the offset of the END of the thread control block's
 * address relative to the value of register r13. The register r13 contains
 * a higher address, so we must subtract this value.
 * The reason for this is offset-based addressing..
 */
#define TLS_TCB_OFFSET	0x7000

/* TLS_PAL_TCB_OFFSET is the address offset of PAL_TCB relative to the
 * value of register r13 (if it is not NULL). We have to subtract this
 * value from r13.
 */
#define TLS_PAL_TCB_OFFSET (TLS_TCB_OFFSET + sizeof(PAL_TCB))

static inline PAL_TCB* r13_to_pal_tcb(uint64_t r13) {
    return (void*)r13 - TLS_PAL_TCB_OFFSET;
}

static inline PAL_TCB* pal_get_tcb(void) {
    register void* r13 __asm__("r13");

    if (!r13)
        return NULL;

    PAL_TCB* tcb = r13_to_pal_tcb((uint64_t)r13);
    return tcb->glibc_tcb.LibOS_TCB;
}

/* pal_set_r13 sets register r13 so that it points at the correct offset
 * realtive to the address of PAL_TCB.
 */
static inline void pal_set_r13(PAL_TCB* tcb) {
    if (tcb) {
        __asm__("addi %%r13, %0, %1\n\t"
                :
                : "r" (tcb),
                  "i" (sizeof(PAL_TCB) + TLS_TCB_OFFSET)
                : "r13"
        );
    } else {
        __asm__("li %%r13, 0\n\t"
                :
                :
                : "r13"
        );
    }
}

/* Set r13 to the PAL_TCB.  Returns 0 on success. */
static inline int pal_set_tcb(PAL_TCB* tcb) {
    pal_set_r13(tcb);
    return 0;
}

#ifdef IN_PAL

/* We need to return from the signal handler by us calling rt_sigreturn.
 * The function needs to be in assembly for the stack adjustment to work properly. */
#define SIGHANDLER_WRAPPER(FNAME)			\
void FNAME (int, siginfo_t*, struct ucontext*);		\
__asm__(						\
    "\t.section\t\".text\"\n\t"				\
    ".align 2\n\t"					\
    ".globl\t"#FNAME"\n\t"				\
    ".type\t"#FNAME", @function\n"			\
    #FNAME":\n\t"					\
    ".cfi_startproc\n"					\
    "0:\taddis 2, 12, (.TOC.-0b)@ha\n\t"		\
    "addi 2, 2, (.TOC.-0b)@l\n\t"			\
    ".localentry     "#FNAME",.-"#FNAME"\n\t"		\
    "bl _"#FNAME"\n\t"					\
    "nop\n\t"						\
    "addi 1,1,128\n\t"					\
    "li 0,172\n\t"					\
    "sc\n\t"						\
    ".cfi_endproc\n\t"					\
    ".size\t"#FNAME",.-"#FNAME"\n\t"			\
);
# define SIGHANDLER_FUNCTION(FUNCNAME)	               \
SIGHANDLER_WRAPPER(FUNCNAME)                           \
void _ ## FUNCNAME(int, siginfo_t*, struct ucontext*); \
void _ ## FUNCNAME

#endif /* IN_PAL */

#endif /* __LINUX_X86_64_PAL_HOST_ARCH_H__ */

