/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2021 IBM Corporation
 *                    Stefan Berger <stefanb@linux.ibm.com>
 */

#pragma once

/* Offsets of shim_tcb entry vectors relative to pal_tcb */
#define GRAMINE_SYSCALL_OFFSET           24 /* void* libos_syscall_entry relative to pal_tcb */

/* Magic syscall number for Gramine custom calls */
#define GRAMINE_CUSTOM_SYSCALL_NR 0x100000
#define GRAMINE_CUSTOM_SYSCALL_RETURN_FROM_SYSCALL_EMULATION 0x100001

/* offset of LIBOS_PTR from tcbhead_t's end */
#define TCBHEAD_LIBOS_PTR_FROM_END_OFFSET (15 * 8)

#ifdef __ASSEMBLER__

/*
 * SYSCALLDB_R9:
 * This macro is a replacement for the 'sc' instruction receiving the
 * syscall number in r0.
 * Its register clobbering behavior should be like that of a Linux syscall
 * where the following registers are volatile: r0, r3-r8, cr0
 * Since this function calls other C functions it may be clobbering the following
 * registers: r0, r3-r12, lr, ctr, xer, cr0-cr1, cr5-cr7 (+some fp registers)
 * So we have to store: r9 - r12, lr, ctr, xer, cr1, cr5-cr7
 *
 * From the perspective of an application calling libc for a syscall, the
 * clobbering of register by this macro does not matter since some registers
 * are expected to be clobbered. It only matters to the function where this
 * macro is embedded in.
 *
 * This macro clobbers: r9, r12, ctr
 *
 * The following are callers:
 * - clone() using DO_CALL_STACKROOM: clobbered registers do not matter
 * - vfork() using DO_CALL_SC
 * - many syscalls using T_PSEUDO or T_PSEUDO_NOERRNO:
 *    these functions are C wrappes for the syscall and therefore the
 *    clobbered registers do not matter (since expected)
 *
 * Call Gramine's libos_syscall_entry with r9 holding the syscall number
 * (rather than r0) and registers r3-r8 holding parameters.
 */
# define SYSCALLDB_R9(FRAMESIZE)                 \
    std 0,-FRAMESIZE(1); /* save syscall nr */   \
    mflr 0;                                      \
    std 0,16(1);         /* save lr */           \
    ld 9,-FRAMESIZE(1);  /* restore syscall nr */\
    stdu 1,-FRAMESIZE(1);                        \
    /* call libos_syscall_entry using tcbhead_t->LibOS */   \
    std 2,24(1);                                 \
    /* pointer to PAL_TCB in tcbhead_t */        \
    ld 12,-0x7000-TCBHEAD_LIBOS_PTR_FROM_END_OFFSET(13); \
    /* our special offset in PAL_TCB for libos_syscall_entry */ \
    ld 12,GRAMINE_SYSCALL_OFFSET(12);             \
    /* address of function must be in r12 */     \
    mtctr 12;                                    \
    bctrl;                                       \
    ld 2,24(1);                                  \
    addi 1,1,FRAMESIZE;                          \
    ld 0,16(1);         /* restore lr */         \
    mtlr 0;

#else /* !__ASSEMBLER__ */

#include <stdbool.h>
#include <stdint.h>


/* Custom call numbers */
enum {
    GRAMINE_CALL_REGISTER_LIBRARY = 1,
    GRAMINE_CALL_RUN_TEST,
    /* For RW locks test. GRAMINE_CALL_RUN_TEST is not suitable for it, because we need full control
     * over locks from multiple threads. Additionally, this test is non-trivial in size, so it
     * doesn't seem right to always compile it into our LibOS. */
    GRAMINE_CALL_RWLOCK_CREATE,
    GRAMINE_CALL_RWLOCK_DESTROY,
    GRAMINE_CALL_RWLOCK_READ_LOCK,
    GRAMINE_CALL_RWLOCK_READ_UNLOCK,
    GRAMINE_CALL_RWLOCK_WRITE_LOCK,
    GRAMINE_CALL_RWLOCK_WRITE_UNLOCK,
};

typedef int(*libos_syscall_entry_func)(unsigned long arg1, unsigned long arg2, unsigned long arg3,
                                       unsigned long arg4, unsigned long arg5, unsigned long arg6,
                                       unsigned long syscall_nr);

static inline libos_syscall_entry_func __get_libos_syscall_entry_func(void) {
    libos_syscall_entry_func libos_syscall_entry_fn;

    __asm__ __volatile__("ld %0,%1(13)\n\t"
                         "ld %0,%2(%0)\n\t"
                         : "=&r" (libos_syscall_entry_fn)
                         : "i" (-0x7000 - TCBHEAD_LIBOS_PTR_FROM_END_OFFSET),
                           "i" (GRAMINE_SYSCALL_OFFSET)
                         :);
    return libos_syscall_entry_fn;
}

static inline int gramine_syscall(int syscallnr,
                                  unsigned long arg1, unsigned long arg2, unsigned long arg3,
                                  unsigned long arg4, unsigned long arg5, unsigned long arg6)
{
    libos_syscall_entry_func libos_syscall_entry_fn = __get_libos_syscall_entry_func();
    return libos_syscall_entry_fn(arg1, arg2, arg3, arg4, arg5, arg6, syscallnr);
}

static inline int gramine_call(int number, unsigned long arg1, unsigned long arg2) {
    libos_syscall_entry_func libos_syscall_entry_fn = __get_libos_syscall_entry_func();
    return libos_syscall_entry_fn(number, arg1, arg2, 0, 0, 0, GRAMINE_CUSTOM_SYSCALL_NR);
}

static inline int gramine_register_library(const char* name, unsigned long load_address) {
    return gramine_call(GRAMINE_CALL_REGISTER_LIBRARY, (unsigned long)name, load_address);
}

static inline int gramine_run_test(const char* test_name) {
    return gramine_call(GRAMINE_CALL_RUN_TEST, (unsigned long)test_name, 0);
}

static inline bool gramine_rwlock_create(void** out_lock) {
    return gramine_call(GRAMINE_CALL_RWLOCK_CREATE, (unsigned long)out_lock, 0);
}

static inline void gramine_rwlock_destroy(void* lock) {
    gramine_call(GRAMINE_CALL_RWLOCK_DESTROY, (unsigned long)lock, 0);
}

static inline void gramine_rwlock_read_lock(void* lock) {
    gramine_call(GRAMINE_CALL_RWLOCK_READ_LOCK, (unsigned long)lock, 0);
}

static inline void gramine_rwlock_read_unlock(void* lock) {
    gramine_call(GRAMINE_CALL_RWLOCK_READ_UNLOCK, (unsigned long)lock, 0);
}

static inline void gramine_rwlock_write_lock(void* lock) {
    gramine_call(GRAMINE_CALL_RWLOCK_WRITE_LOCK, (unsigned long)lock, 0);
}

static inline void gramine_rwlock_write_unlock(void* lock){
    gramine_call(GRAMINE_CALL_RWLOCK_WRITE_UNLOCK, (unsigned long)lock, 0);
}

#endif /* __ASSEMBLER__ */
