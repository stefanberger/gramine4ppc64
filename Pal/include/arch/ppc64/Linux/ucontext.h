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

#ifndef _LINUX_PPC64_UCONTEXT_H
#define _LINUX_PPC64_UCONTEXT_H 1

#include <assert.h>
#include <bits/wordsize.h>
#include <stdint.h>

/* We need the signal context definitions even if they are not used
   included in <signal.h>.  */
#include <sigcontext.h>

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

/* fpregs is shallow copied by only setting a pointer */
static inline void ucontext_to_pal_context(PAL_CONTEXT* context, ucontext_t* uc) {
    assert(sizeof(uc->uc_mcontext.gp_regs) == sizeof(context->gpregs));
    // "uc's gpregs and PAL_CONTEXT's gp_regs sizes are different"
    memcpy(&context->gpregs, uc->uc_mcontext.gp_regs, sizeof(context->gpregs));
    assert(sizeof(uc->uc_mcontext.fp_regs) == sizeof(context->fpregs));
    // "uc's fpregs and PAL_CONTEXT's fp_regs sizes are different"
    memcpy(&context->fpregs, uc->uc_mcontext.fp_regs, sizeof(context->fpregs));
}

/* fpregs is shallow copied by only setting a pointer */
static inline void pal_context_to_ucontext(ucontext_t* uc, PAL_CONTEXT* context) {
    assert(sizeof(uc->uc_mcontext.gp_regs) == sizeof(context->gpregs));
    // "uc's gpregs and PAL_CONTEXT's gp_regs sizes are different"
    memcpy(uc->uc_mcontext.gp_regs, &context->gpregs, sizeof(context->gpregs));
    assert(sizeof(uc->uc_mcontext.fp_regs) == sizeof(context->fpregs));
    // "uc's fpregs and PAL_CONTEXT's fp_regs sizes are different"
    memcpy(uc->uc_mcontext.fp_regs, &context->fpregs, sizeof(context->fpregs));
}

#endif /* _LINUX_PPC64_UCONTEXT_H */
