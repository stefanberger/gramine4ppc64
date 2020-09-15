/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2021 IBM Corporation
 *                    Stefan Berger <stefanb@linux.ibm.com>
 */

#ifndef GRAMINE_ENTRY_API_H_
#define GRAMINE_ENTRY_API_H_

/* Offsets of shim_tcb entry vectors relative to pal_tcb */
#define GRAMINE_SYSCALL_OFFSET           24 /* void* syscalldb */

/* Magic syscall number for Gramine custom calls */
#define GRAMINE_CUSTOM_SYSCALL_NR 0x100000
#define GRAMINE_CUSTOM_SYSCALL_RETURN_FROM_SYSCALL_EMULATION 0x100001

/* offset of LIBOS_PTR from tcbhead_t's end */
#define TCBHEAD_LIBOS_PTR_FROM_END_OFFSET (14 * 8)

#ifdef __ASSEMBLER__

/*
 * SYSCALLDB_R9:
 * This macro is a replacement for the 'sc' instruction.
 * As such its register clobbering behavior is like that of a Linux syscall
 * where the following registers are volatile: r0, r3-r8, cr0
 * Since we are doing an API call here we may be clobbering the following
 * registers: r0, r3-r12, lr, ctr, xer, cr0-cr1, cr5-cr7 (+some fp registers)
 * So we have to store: r9 - r12, lr, ctr, xer, cr1, cr5-cr7
 *
 * We do an ABI-compliant call to Graphene's syscalldb with r9 holding the
 * syscall number (rather than r0) and registers r3-r8 holding parameters.
 */
/* FIXME: we may need to store more registers at a SAFE PLACE! */
# define SYSCALLDB_R9(FRAMESIZE)                 \
    std 0,-FRAMESIZE(1); /* save syscall nr */   \
    mflr 0;                                      \
    std 0,16(1);         /* save lr */           \
    ld 9,-FRAMESIZE(1);  /* restore syscall nr */\
    stdu 1,-FRAMESIZE(1);                        \
    /* call syscalld using tcbhead_t->LibOS */   \
    std 2,24(1);                                 \
    subi 12,13,0x7000+TCBHEAD_LIBOS_PTR_FROM_END_OFFSET; \
    /* pointer to PAL_TCB in tcbhead_t */        \
    ld 12,0(12);                                 \
    /* our special offset in PAL_TCB for syscalldb */ \
    ld 12,GRAMINE_SYSCALL_OFFSET(12);             \
    /* address of function must be in r12 */     \
    mtctr 12;                                    \
    bctrl;                                       \
    ld 2,24(1);                                  \
    addi 1,1,FRAMESIZE;                          \
    ld 0,16(1);         /* restore lr */         \
    mtlr 0;

#else /* !__ASSEMBLER__ */

/* Custom call numbers */
enum {
    GRAMINE_CALL_REGISTER_LIBRARY = 1,
    GRAMINE_CALL_RUN_TEST,
};

static inline int gramine_call(int number, unsigned long arg1, unsigned long arg2) {
    typedef int(*syscalldb_func)(unsigned long arg1, unsigned long arg2, unsigned long arg3,
                                 unsigned long arg4, unsigned long arg5, unsigned long arg6,
                                 unsigned long syscall_nr);
    syscalldb_func syscalldb_fn;

    __asm__ __volatile__("subi %0,13,%1\n\t"
                         "ld %0,0(%0)\n\t"
                         "ld %0,%2(%0)\n\t"
                         : "=&r" (syscalldb_fn)
                         : "i" (0x7000 + TCBHEAD_LIBOS_PTR_FROM_END_OFFSET),
                           "i" (GRAMINE_SYSCALL_OFFSET)
                         :);
    return syscalldb_fn(number, arg1, arg2, 0, 0, 0, GRAMINE_CUSTOM_SYSCALL_NR);
}

static inline int gramine_register_library(const char* name, unsigned long load_address) {
    return gramine_call(GRAMINE_CALL_REGISTER_LIBRARY, (unsigned long)name, load_address);
}

static inline int gramine_run_test(const char* test_name) {
    return gramine_call(GRAMINE_CALL_RUN_TEST, (unsigned long)test_name, 0);
}

#endif /* __ASSEMBLER__ */

#endif /* GRAMINE_ENTRY_API_H_ */
