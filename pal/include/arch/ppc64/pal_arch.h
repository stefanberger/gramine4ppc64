/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2020 IBM Corporation */

/*
 * pal-arch.h
 *
 * This file contains definition of PAL host ABI for PowerPC 64.
 */

#pragma once

#include <assert.h>

#include "cpu.h"
#include "api.h"

#define STACK_PROTECTOR_CANARY_DEFAULT  0xbadbadbadbadUL

/* glibc's define for the offset of the END of the thread control block's
 * address relative to the value of register r13. The register r13 contains
 * a higher address, so we must subtract this value.
 */
#define TLS_TCB_OFFSET			0x7000
#define TLS_STACK_GUARD_OFFSET		(TLS_TCB_OFFSET + sizeof(tcbhead_t) - offsetof(tcbhead_t, stack_guard))

typedef struct pal_tcb PAL_TCB;

/* This is glib'c extended TCB structure; we have to know about it */
typedef struct {
    /* NEW: pointer to LibOS TCB */
        PAL_TCB *LibOS_TCB; /* keep at offset (-14 * 8) from END! */

    /* Reservation for HWCAP data. */
        unsigned int hwcap2;
        unsigned int hwcap; /* not used in LE ABI */

    /* Indicate if HTM capable (ISA 2.07). */
        int tm_capable;
        int tm_pad;

     /* Reservation for dynamic system optimizer ABI. */
        uintptr_t dso_slot2;
        uintptr_t dso_slot1;

    /* Reservation for tar register (ISA 2.07). */
        uintptr_t tar_save;

    /* GCC split stack support. */
        void *__private_ss;

    /* Reservation for the event-based branching ABI. */
        uintptr_t ebb_handler;
        uintptr_t ebb_ctx_pointer;
        uintptr_t ebb_reserved1;
        uintptr_t ebb_reserved2;
        uintptr_t pointer_guard;

    /* Reservation for stack guard */
        uintptr_t stack_guard;

    /* DTV pointer */
        void *dtv;
} tcbhead_t;

static_assert(TLS_STACK_GUARD_OFFSET == 28688,
              "stack guard offset is wrong");

/* FIXME: be more precise about the size */
#define PAL_LIBOS_TCB_SIZE  2048 /* generous space for GPRs, SPRs, FPRs, VECRs etc. */

typedef struct pal_tcb {
    struct pal_tcb* self;
    /* currently unused 64bit field we need so that libos_syscall_entry ptr in libos_tcb is at
     * a specific offset.
     */
    uint64_t unused;
    /* uint64_t for alignment */
    uint8_t libos_tcb[PAL_LIBOS_TCB_SIZE];
    /* data private to PAL implementation follows this struct. */
    /* we have to keep this last */
    tcbhead_t glibc_tcb;
} PAL_TCB;

__attribute__((__optimize__("-fno-stack-protector")))
static inline void pal_tcb_arch_set_stack_canary(PAL_TCB* tcb, uint64_t canary) {
    tcb->glibc_tcb.stack_guard = canary;
}

static_assert(offsetof(PAL_TCB, glibc_tcb.stack_guard) ==
              sizeof(PAL_TCB) - sizeof(void*) /* dtv */ - sizeof(uintptr_t) /* stack_guard */,
              "stack guard is at wrong offset in PAL_TCB");

static inline void pal_tcb_arch_init(PAL_TCB* tcb) {
    tcb->glibc_tcb.LibOS_TCB = tcb;
}

#include "pal_host_arch.h"

/* PAL_CPUSTATE needs the same layout as pt_regs */
typedef struct {
    uint64_t gpr[32];
    uint64_t nip;
    uint64_t msr;
    uint64_t orig_gpr3;
    uint64_t ctr;
    uint64_t link;
    uint64_t xer;
    uint64_t ccr;
    uint64_t softe;
    uint64_t trap;
    uint64_t dar;
    uint64_t dsisr;
    uint64_t result;
} PAL_CPUSTATE;

typedef struct {
    uint64_t hi;
    uint64_t lo;
} __attribute__((aligned(16))) vsr_reg;

typedef struct {
    vsr_reg  vsr[64];
    uint64_t vrsave;
    uint64_t fpscr __attribute__((aligned(16))); /* needs to be 16 byte aligned */
    vsr_reg  vscr;	/* stvx stores 16 bytes */
} __attribute__((aligned(16))) PAL_XTSTATE;

typedef struct PAL_CONTEXT {
    PAL_CPUSTATE gpregs;
    PAL_XTSTATE  xtregs;
    uintptr_t stack_guard; /* we need the stack guard here to support fork()'s child side */
} PAL_CONTEXT;

static_assert((sizeof(PAL_CONTEXT) & 0xf) == 0,
              "Size of PAL_CONTEXT should be multiple of 16 bytes");

typedef int64_t arch_syscall_arg_t;

#define ALL_SYSCALL_ARGS(context) \
    (context)->gpregs.gpr[3], \
    (context)->gpregs.gpr[4], \
    (context)->gpregs.gpr[5], \
    (context)->gpregs.gpr[6], \
    (context)->gpregs.gpr[7], \
    (context)->gpregs.gpr[8]

static inline void pal_context_set_ip(PAL_CONTEXT* context, uintptr_t insnptr) {
    context->gpregs.nip = insnptr;
}

static inline uintptr_t pal_context_get_ip(PAL_CONTEXT* context) {
    return context->gpregs.nip;
}

static inline void pal_context_set_sp(PAL_CONTEXT* context, uintptr_t sp) {
    context->gpregs.gpr[1] = sp;
}
static inline uintptr_t pal_context_get_sp(PAL_CONTEXT* context) {
    return context->gpregs.gpr[1];
}

static inline void pal_context_set_retval(PAL_CONTEXT* context, uint64_t val) {
    context->gpregs.gpr[3] = val;
}
static inline uint64_t pal_context_get_retval(PAL_CONTEXT* context) {
    return context->gpregs.gpr[3];
}

/* r9 holds the syscall nr when call from modified glibc */
#define GLIBC_SYSCALL_NR_REGISTER	9

static inline uint64_t pal_context_get_syscall(PAL_CONTEXT* context) {
    return context->gpregs.gpr[GLIBC_SYSCALL_NR_REGISTER];
}

static inline void pal_context_set_syscall(PAL_CONTEXT* context, uint64_t sysnr) {
    context->gpregs.gpr[GLIBC_SYSCALL_NR_REGISTER] = sysnr;
}

static inline void pal_context_copy(PAL_CONTEXT* dst, PAL_CONTEXT* src) {
    *dst = *src;
}

static inline unsigned long count_ulong_bits_set(unsigned long x) {
    return __builtin_popcountl(x);
}

/* holds /proc/cpuinfo data */
struct pal_cpu_info {
    const char* processor;
    const char* cpu;
    const char* clock;
    const char* revision;
    const char* timebase;
    const char* platform;
    const char* model;
    const char* machine;
    const char* firmware;
    const char* mmu;
};

#define INSN_IS_SC(INSN)	(((INSN >> 24) & 0x7f) == 0x44)
#define INSN_IS_NOP(INSN)	(INSN == 0x60000000)

#define STOP_LOOP do { volatile int i=0; while (!i); } while(0)
