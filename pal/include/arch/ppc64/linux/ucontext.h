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

#pragma once

#include <assert.h>
#include <stdint.h>
#include <stdalign.h>

/* We need the signal context definitions even if they are not used
   included in <signal.h>.  */

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
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
	unsigned int __pad[3];
	unsigned int __ctx(vscr_word);
#else
	unsigned int __ctx(vscr_word);
	unsigned int __pad[3];
#endif
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
	long            __ctx(vmx_reserve)[__NVRREG+__NVRREG+1+32 /* 32 was missing*/];
} mcontext_t;

/* Userlevel context.  */
typedef struct ucontext
  {
    unsigned long int __ctx(uc_flags);
    struct ucontext *uc_link;
    stack_t uc_stack;
    union {
        __sigset_t    uc_sigmask;
        uint8_t uc_force128[128];  // FIXME: __sigset_t is too small!
    };
    mcontext_t  uc_mcontext;  /* last for extensibility */
  } ucontext_t;

#undef __ctx

static inline void ucontext_to_pal_context(PAL_CONTEXT* context, ucontext_t* uc) {
    mcontext_t* mc = &uc->uc_mcontext;
    static_assert(sizeof(mc->gp_regs) >= sizeof(context->gpregs),
                  "gp_regs must be >= than gpregs");
    // "uc's gpregs and PAL_CONTEXT's gp_regs sizes are different"
    memcpy(&context->gpregs, &mc->gp_regs, sizeof(context->gpregs));

    /* floating point */
    for (size_t i = 0; i < 32; i++) {
        uint64_t* in = (uint64_t*)&mc->fp_regs[i];
        context->xtregs.vsr[i].hi = *in;
    }
    uint64_t* in = (uint64_t*)&mc->fp_regs[32];
    context->xtregs.fpscr = *in;

    vrregset_t* v_regs = mc->v_regs;

    /* vsx0-31 */
    unsigned long* vsx_hi_regs = (unsigned long*)((unsigned long)v_regs + __NVRREG * 16);
    for (size_t i = 0; i < 32; i++)
        context->xtregs.vsr[i].lo = vsx_hi_regs[i];

    /* vsx32-63 */
    for (size_t i = 0; i < 32; i++) {
        memcpy(&context->xtregs.vsr[32 + i].hi, &v_regs->vrregs[i][2], 8);
        memcpy(&context->xtregs.vsr[32 + i].lo, &v_regs->vrregs[i][0], 8);
    }

    /* vrsave */
    context->xtregs.vrsave = v_regs->vrsave;
    /* vscr */
    context->xtregs.vscr.hi = v_regs->vscr.vscr_word;

    /* stack guard */
    context->stack_guard = *(unsigned long *)(context->gpregs.gpr[13] - TLS_STACK_GUARD_OFFSET);
}

static inline void pal_context_to_ucontext(ucontext_t* uc, PAL_CONTEXT* context) {
    mcontext_t* mc = &uc->uc_mcontext;
    static_assert(sizeof(mc->gp_regs) >= sizeof(context->gpregs),
                  "gp_regs must be larger than gpregs");
    mc->regs = (void*)&mc->gp_regs;
    // "uc's gpregs and PAL_CONTEXT's gp_regs sizes are different"
    memcpy(&mc->gp_regs, &context->gpregs, sizeof(context->gpregs));

    /* floating point */
    static_assert(sizeof(uc->uc_stack) == 24, "uc_stack has wrong size");
    static_assert(sizeof(uc->uc_force128) == 128, "uc_sigmask has wrong size");
    for (size_t i = 0; i < 32; i++) {
        double* in = (double*)&context->xtregs.vsr[i].hi;
        mc->fp_regs[i] = *in;
    }
    double* in = (double*)&context->xtregs.fpscr;
    mc->fp_regs[32] = *in;

    vrregset_t* v_regs = (void*)ALIGN_UP((unsigned long)&mc->vmx_reserve,
                                         alignof(vrregset_t));
    mc->v_regs = v_regs;

    /* vsx0-31 */
    unsigned long* vsx_hi_regs = (unsigned long*)((unsigned long)v_regs + __NVRREG * 16);
    for (size_t i = 0; i < 32; i++)
        vsx_hi_regs[i] = context->xtregs.vsr[i].lo;

    /* vsx32-63 */
    for (size_t i = 0; i < 32; i++) {
        memcpy(&v_regs->vrregs[i][2], &context->xtregs.vsr[32 + i].hi, 8);
        memcpy(&v_regs->vrregs[i][0], &context->xtregs.vsr[32 + i].lo, 8);
    }

    /* vrsave */
    v_regs->vrsave = context->xtregs.vrsave;
    /* vscr */
    v_regs->vscr.vscr_word = context->xtregs.vscr.hi;
}

static inline uint64_t ucontext_get_ip(ucontext_t* uc) {
    return uc->uc_mcontext.regs->nip;
}

/* `si_call_addr` is a macro, we cannot use it as an argument name. */
static inline void ucontext_revert_syscall(ucontext_t* uc, unsigned int arch, int syscall_nr,
                                           void* _si_call_addr) {
    __UNUSED(arch);
    /* man seccomp(2) says that "si_call_addr will show the address of the system call instruction",
     * which is not true - it points to the next instruction past "syscall". */
    uc->uc_mcontext.regs->nip = (uint64_t)_si_call_addr - 4;
    uc->uc_mcontext.regs->gpr[0] = syscall_nr;

    uint32_t* nip = (uint32_t*)uc->uc_mcontext.regs->nip;
    assert(INSN_IS_SC(nip[0]));
    __UNUSED(nip);
}
