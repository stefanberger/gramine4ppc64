#ifndef _SHIM_TYPES_ARCH_H_
#define _SHIM_TYPES_ARCH_H_

#include <stdint.h>

/* asm/signal.h */
#define NUM_SIGS            64
#define NUM_KNOWN_SIGS      32

typedef struct {
    unsigned long __val[NUM_SIGS / (8 * sizeof(unsigned long))];
} __sigset_t;

/* sys/ucontext.h */
/* Type for general register.  */
typedef long int greg_t;

#define __ctx(fld) fld

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

typedef struct _libc_vrstate
{
	unsigned int    __ctx(vrregs)[32][4];
	vscr_t          __ctx(vscr);
	unsigned int    __ctx(vrsave);
	unsigned int    __pad[3];
} vrregset_t  __attribute__((__aligned__(16)));

typedef struct {
	unsigned long	__glibc_reserved[4];
	int		__ctx(signal);
	int		__pad0;
	unsigned long	__ctx(handler);
	unsigned long	__ctx(oldmask);
	struct __ctx(pt_regs)	*__ctx(regs);
	gregset_t	__ctx(gp_regs);
	fpregset_t	__ctx(fp_regs);
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
	vrregset_t	*__ctx(v_regs);
	long		__ctx(vmx_reserve)[__NVRREG+__NVRREG+1];
} mcontext_t;

/* Userlevel context.  */
typedef struct ucontext_t
  {
    unsigned long int __ctx(uc_flags);
    struct ucontext_t *uc_link;
    stack_t uc_stack;
    sigset_t    uc_sigmask;
    mcontext_t  uc_mcontext;  /* last for extensibility */
  } ucontext_t;

#undef __ctx

#endif /* _SHIM_TYPES_ARCH_H_ */
