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
#define _LINUX_PPC64_SYSDEP_H

#include <sysdeps/generic/sysdep.h>

#define FRAME_LR_SAVE           16
#define FRAME_MIN_SIZE_PARM	96
#define FRAME_MIN_SIZE          32
#define FRAME_TOC_SAVE          24
#define FRAME_PARM_SAVE		32

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

#else /* !__ASSEMBLER__ */
/* Define a macro which expands inline into the wrapper code for a system
   call.  */
#undef INLINE_SYSCALL

#define INLINE_SYSCALL(name, nr, args...) INTERNAL_SYSCALL(name, , nr, args)

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

#endif /* __ASSEMBLER__ */

#endif /* linux/ppc64/sysdep.h */
