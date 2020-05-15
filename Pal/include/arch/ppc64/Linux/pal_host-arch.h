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
 * pal_host-arch.h
 *
 * This file contains Linux on ppc64 specific functions related to the PAL.
 */

#ifndef __LINUX_PPC64_PAL_HOST_ARCH_H__
#define __LINUX_PPC64_PAL_HOST_ARCH_H__

#ifdef IN_PAL

static inline PAL_TCB* pal_set_r13(PAL_TCB* tcb) {
    PAL_TCB* orig_tcb = pal_get_tcb();

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
    return orig_tcb;
}

static inline int pal_set_tcb(PAL_TCB *tcb) {
    pal_set_r13(tcb);
    return 0;
}

/* We need to return from the signal handler by us calling rt_sigreturn.
 * The function needs to be in assembly for the stack adjustment to work properly. */
#define SIGHANDLER_WRAPPER(FNAME)			\
void FNAME (int, siginfo_t *, struct ucontext *);	\
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
# define SIGHANDLER_FUNCTION(FUNCNAME)	                 \
SIGHANDLER_WRAPPER(FUNCNAME)                             \
void _ ## FUNCNAME(int, siginfo_t *, struct ucontext *); \
void _ ## FUNCNAME

#endif /* IN_PAL */

#endif /* __LINUX_X86_64_PAL_HOST_ARCH_H__ */
