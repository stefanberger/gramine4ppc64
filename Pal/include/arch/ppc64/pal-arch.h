/* Copyright (C) 2020 IBM Corporation
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
 * pal-arch.h
 *
 * This file contains definition of PAL host ABI for PowerPC 64.
 */

#ifndef PAL_ARCH_H
#define PAL_ARCH_H

#define TLS_TCB_OFFSET	0x7000

#define PAL_LIBOS_TCB_SIZE  256

typedef struct pal_tcb PAL_TCB;
/* This is glib'c tcb structure; we have to know about it */
typedef struct {
    /* NEW: pointer to LibOS TCB */
        PAL_TCB *LibOS_TCB;

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

typedef struct pal_tcb {
    struct pal_tcb * self;
    /* uint64_t for alignment */
    uint64_t libos_tcb[(PAL_LIBOS_TCB_SIZE + sizeof(uint64_t) - 1) / sizeof(uint64_t)];
    /* data private to PAL implementation follows this struct. */
    /* we have to keep this last */
    tcbhead_t glibc_tcb;
} PAL_TCB;

static inline PAL_TCB * pal_get_tcb (void)
{
    PAL_TCB * tcb;
    __asm__ ("addi %0, %%r13, %1\n\r"
             : "=r" (tcb)
             : "i" (-TLS_TCB_OFFSET - sizeof(PAL_TCB)));
    return tcb->glibc_tcb.LibOS_TCB;
}

/* PAL_CPUSTATE needs the same layout as pt_regs */
typedef struct {
    PAL_NUM r0, r1, r2, r3, r4, r5, r6, r7;
    PAL_NUM r8, r9, r10, r11, r12, r13, r14, r15;
    PAL_NUM r16, r17, r18, r19, r20, r21, r22, r23;
    PAL_NUM r24, r25, r26, r27, r28, r29, r30, r31;
    PAL_NUM nip, msr, orig_gpr3, ctr, link, xer, ccr, softe;
    PAL_NUM trap, dar, dsisr, result;
} PAL_CPUSTATE;

typedef struct {
    uint64_t fpr[32][2] __attribute__((aligned(16)));
    uint64_t fpscr;
} PAL_FPSTATE;

typedef struct {
    PAL_CPUSTATE gpregs;
    PAL_FPSTATE  fpregs;
} PAL_CONTEXT;

#endif
