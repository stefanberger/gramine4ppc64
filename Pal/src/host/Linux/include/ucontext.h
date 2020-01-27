/* Copyright (C) 2001, 2002 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#ifndef _UCONTEXT_H
#define _UCONTEXT_H 1

#include <bits/wordsize.h>
#include <stdint.h>

/* We need the signal context definitions even if they are not used
   included in <signal.h>.  */
#include <sigcontext.h>

#if defined(__i386__) || defined (__x86_64__)

#if __WORDSIZE == 64

/* Type for general register.  */
typedef long int greg_t;

/* Number of general registers.  */
#define NGREG 23

/* Container for all general registers.  */
typedef greg_t gregset_t[NGREG];

/* Number of each register in the `gregset_t' array.  */
enum {
    REG_R8 = 0,
#define REG_R8 REG_R8
    REG_R9,
#define REG_R9 REG_R9
    REG_R10,
#define REG_R10 REG_R10
    REG_R11,
#define REG_R11 REG_R11
    REG_R12,
#define REG_R12 REG_R12
    REG_R13,
#define REG_R13 REG_R13
    REG_R14,
#define REG_R14 REG_R14
    REG_R15,
#define REG_R15 REG_R15
    REG_RDI,
#define REG_RDI REG_RDI
    REG_RSI,
#define REG_RSI REG_RSI
    REG_RBP,
#define REG_RBP REG_RBP
    REG_RBX,
#define REG_RBX REG_RBX
    REG_RDX,
#define REG_RDX REG_RDX
    REG_RAX,
#define REG_RAX REG_RAX
    REG_RCX,
#define REG_RCX REG_RCX
    REG_RSP,
#define REG_RSP REG_RSP
    REG_RIP,
#define REG_RIP REG_RIP
    REG_EFL,
#define REG_EFL REG_EFL
    REG_CSGSFS, /* Actually short cs, gs, fs, __pad0.  */
#define REG_CSGSFS REG_CSGSFS
    REG_ERR,
#define REG_ERR REG_ERR
    REG_TRAPNO,
#define REG_TRAPNO REG_TRAPNO
    REG_OLDMASK,
#define REG_OLDMASK REG_OLDMASK
    REG_CR2
#define REG_CR2 REG_CR2
};

struct _libc_fpxreg {
    unsigned short int significand[4];
    unsigned short int exponent;
    unsigned short int padding[3];
};

struct _libc_xmmreg {
    uint32_t element[4];
};

struct _libc_fpstate {
    /* 64-bit FXSAVE format.  */
    uint16_t cwd;
    uint16_t swd;
    uint16_t ftw;
    uint16_t fop;
    uint64_t rip;
    uint64_t rdp;
    uint32_t mxcsr;
    uint32_t mxcr_mask;
    struct _libc_fpxreg _st[8];
    struct _libc_xmmreg _xmm[16];
    uint32_t padding[24];
};

/* Structure to describe FPU registers.  */
typedef struct _libc_fpstate* fpregset_t;

/* Context to describe whole processor state.  */
typedef struct {
    gregset_t gregs;
    /* Note that fpregs is a pointer.  */
    fpregset_t fpregs;
    unsigned long __reserved1[8];
} mcontext_t;

/* Userlevel context.  */
typedef struct ucontext {
    unsigned long int uc_flags;
    struct ucontext* uc_link;
    stack_t uc_stack;
    mcontext_t uc_mcontext;
    __sigset_t uc_sigmask;
    struct _libc_fpstate __fpregs_mem;
} ucontext_t;

#else /* __WORDSIZE == 32 */

/* Type for general register.  */
typedef int greg_t;

/* Number of general registers.  */
#define NGREG 19

/* Container for all general registers.  */
typedef greg_t gregset_t[NGREG];

#ifdef __USE_GNU
/* Number of each register is the `gregset_t' array.  */
enum {
    REG_GS = 0,
#define REG_GS     REG_GS
    REG_FS,
#define REG_FS     REG_FS
    REG_ES,
#define REG_ES     REG_ES
    REG_DS,
#define REG_DS     REG_DS
    REG_EDI,
#define REG_EDI    REG_EDI
    REG_ESI,
#define REG_ESI    REG_ESI
    REG_EBP,
#define REG_EBP    REG_EBP
    REG_ESP,
#define REG_ESP    REG_ESP
    REG_EBX,
#define REG_EBX    REG_EBX
    REG_EDX,
#define REG_EDX    REG_EDX
    REG_ECX,
#define REG_ECX    REG_ECX
    REG_EAX,
#define REG_EAX    REG_EAX
    REG_TRAPNO,
#define REG_TRAPNO REG_TRAPNO
    REG_ERR,
#define REG_ERR    REG_ERR
    REG_EIP,
#define REG_EIP    REG_EIP
    REG_CS,
#define REG_CS     REG_CS
    REG_EFL,
#define REG_EFL    REG_EFL
    REG_UESP,
#define REG_UESP   REG_UESP
    REG_SS
#define REG_SS     REG_SS
};
#endif

/* Definitions taken from the kernel headers.  */
struct _libc_fpreg {
    unsigned short int significand[4];
    unsigned short int exponent;
};

struct _libc_fpstate {
    unsigned long int cw;
    unsigned long int sw;
    unsigned long int tag;
    unsigned long int ipoff;
    unsigned long int cssel;
    unsigned long int dataoff;
    unsigned long int datasel;
    struct _libc_fpreg _st[8];
    unsigned long int status;
};

/* Structure to describe FPU registers.  */
typedef struct _libc_fpstate* fpregset_t;

/* Context to describe whole processor state.  */
typedef struct {
    gregset_t gregs;
    /* Due to Linux's history we have to use a pointer here.  The SysV/i386
       ABI requires a struct with the values.  */
    fpregset_t fpregs;
    unsigned long int oldmask;
    unsigned long int cr2;
} mcontext_t;

/* Userlevel context.  */
typedef struct ucontext {
    unsigned long int uc_flags;
    struct ucontext* uc_link;
    stack_t uc_stack;
    mcontext_t uc_mcontext;
    __sigset_t uc_sigmask;
    struct _libc_fpstate __fpregs_mem;
} ucontext_t;

#endif /* __WORDSIZE == 32 */

#elif defined(__powerpc64__)

struct pt_regs {
	unsigned long gpr[32];
	unsigned long nip;
	unsigned long msr;
	unsigned long orig_gpr3;        /* Used for restarting system calls */
	unsigned long ctr;
	unsigned long link;
	unsigned long xer;
	unsigned long ccr;
#ifdef __powerpc64__
	unsigned long softe;            /* Soft enabled/disabled */
#else
	unsigned long mq;               /* 601 only (not used at present) */
                                        /* Used on APUS to hold IPL value. */
#endif
	unsigned long trap;             /* Reason for being here */
	/* N.B. for critical exceptions on 4xx, the dar and dsisr
	   fields are overloaded to hold srr0 and srr1. */
	unsigned long dar;              /* Fault registers */
	unsigned long dsisr;            /* on 4xx/Book-E used for ESR */
	unsigned long result;           /* Result of a system call */
};

#define __ctx(fld) fld

struct __ctx(pt_regs);

/* Number of general registers.  */
# define __NGREG        48      /* includes r0-r31, nip, msr, lr, etc.   */
# define __NFPREG       33      /* includes fp0-fp31 &fpscr.  */
# define __NVRREG       34      /* includes v0-v31, vscr, & vrsave in
                                   split vectors */

typedef unsigned long gregset_t[__NGREG];
typedef double fpregset_t[__NFPREG];

/* Container for Altivec/VMX Vector Status and Control Register.  Only 32-bits
   but can only be copied to/from a 128-bit vector register.  So we allocated
   a whole quadword speedup save/restore.  */
typedef struct _libc_vscr
{
	unsigned int __pad[3];
	unsigned int __ctx(vscr_word);
} vscr_t;

/* Container for Altivec/VMX registers and status.
   Must to be aligned on a 16-byte boundary. */
typedef struct _libc_vrstate
{
	unsigned int    __ctx(vrregs)[32][4];
	vscr_t          __ctx(vscr);
	unsigned int    __ctx(vrsave);
	unsigned int    __pad[3];
} vrregset_t  __attribute__((__aligned__(16)));

typedef struct {
	unsigned long   __glibc_reserved[4];
	int             __ctx(signal);
	int             __pad0;
	unsigned long   __ctx(handler);
	unsigned long   __ctx(oldmask);
	struct __ctx(pt_regs)   *__ctx(regs);
	gregset_t       __ctx(gp_regs);
	fpregset_t      __ctx(fp_regs);
/*
 * To maintain compatibility with current implementations the sigcontext is
 * extended by appending a pointer (v_regs) to a quadword type (elf_vrreg_t)
 * followed by an unstructured (vmx_reserve) field of 69 doublewords.  This
 * allows the array of vector registers to be quadword aligned independent of
 * the alignment of the containing sigcontext or ucontext. It is the
 * responsibility of the code setting the sigcontext to set this pointer to
 * either NULL (if this processor does not support the VMX feature) or the
 * address of the first quadword within the allocated (vmx_reserve) area.
 *
 * The pointer (v_regs) of vector type (elf_vrreg_t) is essentually
 * an array of 34 quadword entries.  The entries with
 * indexes 0-31 contain the corresponding vector registers.  The entry with
 * index 32 contains the vscr as the last word (offset 12) within the
 * quadword.  This allows the vscr to be stored as either a quadword (since
 * it must be copied via a vector register to/from storage) or as a word.
 * The entry with index 33 contains the vrsave as the first word (offset 0)
 * within the quadword.
 */
	vrregset_t      *__ctx(v_regs);
	long            __ctx(vmx_reserve)[__NVRREG+__NVRREG+1];
} mcontext_t;

/* Userlevel context.  */
typedef struct ucontext
  {
    unsigned long int __ctx(uc_flags);
    struct ucontext *uc_link;
    stack_t uc_stack;
    __sigset_t    uc_sigmask;
    mcontext_t  uc_mcontext;  /* last for extensibility */
  } ucontext_t;

#undef __ctx

#else
# error Unsupported architecture
#endif

#endif /* ucontext.h */
