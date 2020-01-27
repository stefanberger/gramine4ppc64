/* Copyright (C) 2014 Stony Brook University
   Copyright (C) 2020 IBM Corporation
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

/* This file is imported and modified from the GNU C Library */

#ifndef _LINUX_PPC64_SYSDEP_H
#define _LINUX_PPC64_SYSDEP_H 1

#include <sysdeps/generic/sysdep.h>

/* For Linux we can use the system call table in the header file
    /usr/include/asm/unistd.h
   of the kernel.  But these symbols do not follow the SYS_* syntax
   so we have to redefine the `SYS_ify' macro here.  */
#undef SYS_ify
#define SYS_ify(syscall_name) __NR_##syscall_name

/* This is a kludge to make syscalls.list find these under the names
   pread and pwrite, since some kernel headers define those names
   and some define the *64 names for the same system calls.  */
#if !defined __NR_pread && defined __NR_pread64
#define __NR_pread __NR_pread64
#endif
#if !defined __NR_pwrite && defined __NR_pwrite64
#define __NR_pwrite __NR_pwrite64
#endif

#ifdef __ASSEMBLER__

/* ELF uses byte-counts for .align, most others use log2 of count of bytes.  */
#define ALIGNARG(log2)       (1 << (log2))
#define ASM_GLOBAL_DIRECTIVE .global
/* For ELF we need the `.type' directive to make shared libs work right.  */
#define ASM_TYPE_DIRECTIVE(name, typearg) .type name,typearg;
#define ASM_SIZE_DIRECTIVE(name)          .size name,.-name;

#define C_LABEL(name) name

/* Define an entry point visible from C.  */
#define ENTRY(name)                      \
    ASM_GLOBAL_DIRECTIVE name;           \
    ASM_TYPE_DIRECTIVE(name, @function); \
    .align ALIGNARG(4);                  \
    name:                                \
    cfi_startproc;

#undef END
#define END(name) \
    cfi_endproc;  \
    ASM_SIZE_DIRECTIVE(name)

/* The Linux/ppc64 kernel expects the system call parameters in
   registers according to the following table:

    syscall number    r0
    arg 1        r3
    arg 2        r4
    arg 3        r5
    arg 4        r6
    arg 5        r7
    arg 6        r8

    The Linux kernel uses and destroys internally these registers:
    return address from
    syscall        
    additionally clobbered:

    Normal function call, including calls to the system call stub
    functions in the libc, get the first six parameters passed in
    registers and the seventh parameter and later on the stack.  The
    register use is as follows:

     system call number    in the DO_CALL macro
     arg 1        r3
     arg 2        r4
     arg 3        r5
     arg 4        r5
     arg 5        r7
     arg 6        r8

    We have to take care that the stack is aligned to 16 bytes.  When
    called the stack is not aligned since the return address has just
    been pushed.


    Syscalls of more than 6 arguments are not supported.  */

#ifndef DO_SYSCALL
#define DO_SYSCALL sc
#endif

#undef DO_CALL
#define DO_CALL(syscall_number)  \
    li %r0, syscall_number;      \
    DO_SYSCALL;

#define DOARGS_0 /* nothing */
#define DOARGS_1 /* nothing */
#define DOARGS_2 /* nothing */
#define DOARGS_3 /* nothing */
#define DOARGS_4 /* nothing */
#define DOARGS_5 /* nothing */
#define DOARGS_6 /* nothing */

#else /* !__ASSEMBLER__ */
/* Define a macro which expands inline into the wrapper code for a system
   call.  */
#undef INLINE_SYSCALL

#define INLINE_SYSCALL(name, nr, args...) INTERNAL_SYSCALL(name, , nr, args)

#undef INTERNAL_SYSCALL_DECL
#define INTERNAL_SYSCALL_DECL(err) \
    do {                           \
    } while (0)

#ifndef DO_SYSCALL
#define DO_SYSCALL "sc"
#endif

#define CR0_SO (0x80000000 >> 3)  /* summary overflow; indicates syscall error */

#define INTERNAL_SYSCALL_NCS(name, err, nr, args...) \
  ({									\
    register long int r0  __asm__ ("r0");				\
    register long int r3  __asm__ ("r3");				\
    register long int r4  __asm__ ("r4");				\
    register long int r5  __asm__ ("r5");				\
    register long int r6  __asm__ ("r6");				\
    register long int r7  __asm__ ("r7");				\
    register long int r8  __asm__ ("r8");				\
    LOADARGS_##nr (name, ##args);					\
    __asm__ __volatile__						\
      ("sc\n\t"								\
       "mfcr  %0\n\t"							\
       "andis. %%r9, %0, 4096\n\t"					\
       "beq 0f\n\t"							\
       "neg %%r3,%%r3\n\t"						\
       "0:"								\
       : "=&r" (r0),							\
         "=&r" (r3), "=&r" (r4), "=&r" (r5),				\
         "=&r" (r6), "=&r" (r7), "=&r" (r8)				\
       : ASM_INPUT_##nr							\
       : "r9", "r10", "r11", "r12",					\
         "cc", "ctr", "memory");					\
    r3;									\
  })

#define LOADARGS_0(name, dummy) \
	r0 = name
#define LOADARGS_1(name, __arg1) \
	long int arg1 = (long int) (__arg1); \
	LOADARGS_0(name, 0); \
	r3 = arg1
#define LOADARGS_2(name, __arg1, __arg2) \
	long int arg2 = (long int) (__arg2); \
	LOADARGS_1(name, __arg1); \
	r4 = arg2
#define LOADARGS_3(name, __arg1, __arg2, __arg3) \
	long int arg3 = (long int) (__arg3); \
	LOADARGS_2(name, __arg1, __arg2); \
	r5 = arg3
#define LOADARGS_4(name, __arg1, __arg2, __arg3, __arg4) \
	long int arg4 = (long int) (__arg4); \
	LOADARGS_3(name, __arg1, __arg2, __arg3); \
	r6 = arg4
#define LOADARGS_5(name, __arg1, __arg2, __arg3, __arg4, __arg5) \
	long int arg5 = (long int) (__arg5); \
	LOADARGS_4(name, __arg1, __arg2, __arg3, __arg4); \
	r7 = arg5
#define LOADARGS_6(name, __arg1, __arg2, __arg3, __arg4, __arg5, __arg6) \
	long int arg6 = (long int) (__arg6); \
	LOADARGS_5(name, __arg1, __arg2, __arg3, __arg4, __arg5); \
	r8 = arg6

#define ASM_INPUT_0 "0" (r0)
#define ASM_INPUT_1 ASM_INPUT_0, "1" (r3)
#define ASM_INPUT_2 ASM_INPUT_1, "2" (r4)
#define ASM_INPUT_3 ASM_INPUT_2, "3" (r5)
#define ASM_INPUT_4 ASM_INPUT_3, "4" (r6)
#define ASM_INPUT_5 ASM_INPUT_4, "5" (r7)
#define ASM_INPUT_6 ASM_INPUT_5, "6" (r8)

#if 0
#define INTERNAL_SYSCALL_NCS(callnr, err, nr, args...)   \
    ({                                                   \
        long resultvar;                                  \
        long cr;                                         \
        LOAD_ARGS_##nr(args);                            \
        __asm__ volatile("mr 0, %2\n\t"                  \
                         MR_##nr                         \
                         DO_SYSCALL "\n\t"               \
                         "mr %0, 3\n\t"                  \
                         "mfcr %1\n\t"                   \
                         : "=&r"(resultvar), "=&r" (cr)  \
                         : "r" (callnr)ASM_ARGS_##nr     \
                         : "r0", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "cr0", "ctr", "memory");        \
        if (cr & CR0_SO)                                 \
            resultvar = -resultvar;                      \
        resultvar;                                       \
    })

#endif

#undef INTERNAL_SYSCALL
#define INTERNAL_SYSCALL(name, err, nr, args...) INTERNAL_SYSCALL_NCS(__NR_##name, err, nr, ##args)

#undef INTERNAL_SYSCALL_ERROR
#define INTERNAL_SYSCALL_ERROR(val) ((val) < 0)

#undef INTERNAL_SYSCALL_ERROR_P
#define INTERNAL_SYSCALL_ERROR_P(val) ((unsigned long)(val) >= (unsigned long)-4095L)

#undef INTERNAL_SYSCALL_ERRNO
#define INTERNAL_SYSCALL_ERRNO(val) (-(val))

#undef INTERNAL_SYSCALL_ERRNO_P
#define INTERNAL_SYSCALL_ERRNO_P(val) (-((long)val))

#if 0

#define LOAD_ARGS_0()
#define LOAD_REGS_0
#define ASM_ARGS_0
#define MR_0
#define CLOBBER_0 "r0", "r3"

#define LOAD_ARGS_1(a1)           \
    unsigned long int __arg1 = (unsigned long)(a1); \
    LOAD_ARGS_0()
#define LOAD_REGS_1                               \
    register unsigned long int _a1 __asm__("r3") = __arg1; \
    LOAD_REGS_0
#define ASM_ARGS_1 ASM_ARGS_0, "r"(__arg1)
#define MR_1 MR_0 "mr %%r3, %3\n\t"
#define CLOBBER_1 CLOBBER_0

#define LOAD_ARGS_2(a1, a2)       \
    unsigned long int __arg2 = (unsigned long)(a2); \
    LOAD_ARGS_1(a1)
#define LOAD_REGS_2                               \
    register unsigned long int _a2 __asm__("r4") = __arg2; \
    LOAD_REGS_1
#define ASM_ARGS_2 ASM_ARGS_1, "r"(__arg2)
#define MR_2 MR_1 "mr %%r4, %4\n\t"
#define CLOBBER_2 CLOBBER_1, "r4"

#define LOAD_ARGS_3(a1, a2, a3)   \
    unsigned long int __arg3 = (unsigned long)(a3); \
    LOAD_ARGS_2(a1, a2)
#define LOAD_REGS_3                               \
    register unsigned long int _a3 __asm__("r5") = __arg3; \
    LOAD_REGS_2
#define ASM_ARGS_3 ASM_ARGS_2, "r"(__arg3)
#define MR_3 MR_2 "mr %%r5, %5\n\t"
#define CLOBBER_3 CLOBBER_2, "r5"

#define LOAD_ARGS_4(a1, a2, a3, a4) \
    unsigned long int __arg4 = (unsigned long)(a4);   \
    LOAD_ARGS_3(a1, a2, a3)
#define LOAD_REGS_4                               \
    register unsigned long int _a4 __asm__("r6") = __arg4; \
    LOAD_REGS_3
#define ASM_ARGS_4 ASM_ARGS_3, "r"(__arg4)
#define MR_4 MR_3 "mr %%r6, %6\n\t"
#define CLOBBER_4 CLOBBER_3, "r6"

#define LOAD_ARGS_5(a1, a2, a3, a4, a5) \
    unsigned long int __arg5 = (unsigned long)(a5);       \
    LOAD_ARGS_4(a1, a2, a3, a4)
#define LOAD_REGS_5                               \
    register unsigned long int _a5 __asm__("r7") = __arg5; \
    LOAD_REGS_4
#define ASM_ARGS_5 ASM_ARGS_4, "r"(__arg5)
#define MR_5 MR_4 "mr %%r7, %7\n\t"
#define CLOBBER_5 CLOBBER_4, "r7"

#define LOAD_ARGS_6(a1, a2, a3, a4, a5, a6) \
    unsigned long int __arg6 = (unsigned long)(a6);           \
    LOAD_ARGS_5(a1, a2, a3, a4, a5)
#define LOAD_REGS_6                               \
    register unsigned long int _a6 __asm__("r8") = __arg6; \
    LOAD_REGS_5
#define ASM_ARGS_6 ASM_ARGS_5, "r"(__arg6)
#define MR_6 MR_5 "mr %%r8, %8\n\t"
#define CLOBBER_6 CLOBBER_5, "r8"

#endif

#endif /* __ASSEMBLER__ */

#endif /* linux/ppc64/sysdep.h */
