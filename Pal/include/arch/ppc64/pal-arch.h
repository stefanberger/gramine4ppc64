/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2020 IBM Corporation */

/*
 * pal-arch.h
 *
 * This file contains definition of PAL host ABI for PowerPC 64.
 */

#ifndef PAL_ARCH_H
#define PAL_ARCH_H

#include <assert.h>

#include "cpu.h"

#define STACK_PROTECTOR_CANARY_DEFAULT  0xbadbadbadbadUL

/* Used to represent plain integers (only numeric values) */
#define PAL_SYSFS_INT_FILESZ 16
/* Used to represent buffers having numeric values with text. E.g "1024576K" */
#define PAL_SYSFS_BUF_FILESZ 64
/* Used to represent cpumaps like "00000000,ffffffff,00000000,ffffffff" */
#define PAL_SYSFS_MAP_FILESZ 256

/* glibc's define for the offset of the END of the thread control block's
 * address relative to the value of register r13. The register r13 contains
 * a higher address, so we must subtract this value.
 */
#define TLS_TCB_OFFSET	0x7000

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

static inline tcbhead_t *r13_to_tcbhead_t(uint64_t r13) {
    return (tcbhead_t*)(r13 - TLS_TCB_OFFSET - sizeof(tcbhead_t));
}

/* FIXME: be more precise about the size */
#define PAL_LIBOS_TCB_SIZE  2048 /* generous space for GPRs, SPRs, FPRs, VECRs etc. */

typedef struct pal_tcb {
    struct pal_tcb* self;
    /* currently unused 64bit field we need so that syscalldb in libos_tcb is at
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
    /* signal info */
    int si_signo;
    int si_code;
} PAL_CONTEXT;

#define DEFAULT_OBJECT_EXEC_ADDR ((void*)0x555555560000) /* Linux base location for PIE binaries */

static inline void pal_context_set_ip(PAL_CONTEXT* context, PAL_NUM insnptr) {
    context->gpregs.nip = insnptr;
}

static inline PAL_NUM pal_context_get_ip(PAL_CONTEXT* context) {
    return context->gpregs.nip;
}

#include <linux/signal.h>

static inline void siginfo_to_pal_context(PAL_CONTEXT* context, siginfo_t *info) {
    context->si_signo = info->si_signo;
    context->si_code = info->si_code;
}

static inline bool pal_context_has_user_pagefault(PAL_CONTEXT* context) {
    return (context->si_signo == SIGSEGV) && (context->si_code == SEGV_ACCERR);
}

enum {
    HUGEPAGES_2M = 0,
    HUGEPAGES_1G,
    HUGEPAGES_MAX,
};

/* PAL_CPU_INFO holds /proc/cpuinfo data */
typedef struct PAL_CPU_INFO_ {
    /* Number of logical cores available in the host */
    PAL_NUM online_logical_cores;
    /* Max number of logical cores available in the host */
    PAL_NUM possible_logical_cores;
    /* Number of physical cores in a socket (physical package) */
    PAL_NUM physical_cores_per_socket;
    /* array of "logical core -> socket" mappings; has online_logical_cores elements */
    int* cpu_socket;
    PAL_STR processor;
    PAL_STR cpu;
    PAL_STR clock;
    PAL_STR revision;
    PAL_STR timebase;
    PAL_STR platform;
    PAL_STR model;
    PAL_STR machine;
    PAL_STR firmware;
    PAL_STR mmu;
} PAL_CPU_INFO;

typedef struct PAL_CORE_CACHE_INFO_ {
    char shared_cpu_map[PAL_SYSFS_MAP_FILESZ];
    char level[PAL_SYSFS_INT_FILESZ];
    char type[PAL_SYSFS_BUF_FILESZ];
    char size[PAL_SYSFS_BUF_FILESZ];
    char coherency_line_size[PAL_SYSFS_INT_FILESZ];
    char number_of_sets[PAL_SYSFS_INT_FILESZ];
#if 0
    char physical_line_partition[PAL_SYSFS_INT_FILESZ]; // on x86_64
#endif
} PAL_CORE_CACHE_INFO;

typedef struct PAL_CORE_TOPO_INFO_ {
    /* [0] element is uninitialized because core 0 is always online */
    char is_logical_core_online[PAL_SYSFS_INT_FILESZ];
    char core_id[PAL_SYSFS_INT_FILESZ];
    char core_siblings[PAL_SYSFS_MAP_FILESZ];
    char thread_siblings[PAL_SYSFS_MAP_FILESZ];
    PAL_CORE_CACHE_INFO* cache; /* Array of size num_cache_index, owned by this struct */
} PAL_CORE_TOPO_INFO;

typedef struct PAL_NUMA_HUGEPAGE_INFO_ {
    char nr_hugepages[PAL_SYSFS_INT_FILESZ];
} PAL_NUMA_HUGEPAGE_INFO;

typedef struct PAL_NUMA_TOPO_INFO_ {
    char cpumap[PAL_SYSFS_MAP_FILESZ];
    char distance[PAL_SYSFS_INT_FILESZ];
    PAL_NUMA_HUGEPAGE_INFO hugepages[HUGEPAGES_MAX];
} PAL_NUMA_TOPO_INFO;

/* This struct takes ~1.6KB. On a single socket, 4 logical core system, with 3 cache levels
 * it would take ~8KB in memory. */
typedef struct PAL_TOPO_INFO_ {
    char online_logical_cores[PAL_SYSFS_BUF_FILESZ];
    char possible_logical_cores[PAL_SYSFS_BUF_FILESZ];
    char online_nodes[PAL_SYSFS_BUF_FILESZ];
    /* Number of nodes available in the host */
    PAL_NUM num_online_nodes;
    /* cache index corresponds to number of cache levels (such as L2 or L3) available on the host */
    PAL_NUM num_cache_index;
    PAL_CORE_TOPO_INFO* core_topology; /* Array of logical core topology info, owned by this struct */
    PAL_NUMA_TOPO_INFO* numa_topology; /* Array of numa topology info, owned by this struct */
} PAL_TOPO_INFO;

#endif /* PAL_ARCH_H */
