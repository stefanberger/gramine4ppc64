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

#define PAGE_SIZE       (1 << 16)
#define PRESET_PAGESIZE PAGE_SIZE

typedef struct pal_tcb PAL_TCB;

/* This is glib'c extended TCB structure; we have to know about it */
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

/* FIXME: be more precise about the size */
#define PAL_LIBOS_TCB_SIZE  2048 /* generous space for GPRs, SPRs, FPRs, VECRs etc. */

typedef struct pal_tcb {
    struct pal_tcb* self;
    /* uint64_t for alignment */
    uint8_t libos_tcb[PAL_LIBOS_TCB_SIZE];
    /* data private to PAL implementation follows this struct. */
    /* we have to keep this last */
    tcbhead_t glibc_tcb;
} PAL_TCB;

static inline void pal_tcb_arch_init(PAL_TCB* tcb) {
    tcb->glibc_tcb.LibOS_TCB = tcb;
}

#include "pal_host-arch.h"

/* PAL_CPUSTATE needs the same layout as pt_regs */
typedef struct {
    PAL_NUM gpr[32];
    PAL_NUM nip, msr, orig_gpr3, ctr, link, xer, ccr, softe;
    PAL_NUM trap, dar, dsisr, result;
} PAL_CPUSTATE;

typedef struct {
    uint64_t fpr[32];
    uint64_t fpscr;
} PAL_FPSTATE;

typedef struct {
    PAL_CPUSTATE gpregs;
    PAL_FPSTATE  fpregs;
} PAL_CONTEXT;

#define DEFAULT_OBJECT_EXEC_ADDR ((void*)0x555555560000) /* Linux base location for PIE binaries */

static inline void pal_context_set_ip(PAL_CONTEXT* context, PAL_NUM insnptr) {
    context->gpregs.nip = insnptr;
}

static inline PAL_NUM pal_context_get_ip(PAL_CONTEXT *context) {
    return context->gpregs.nip;
}

#endif
