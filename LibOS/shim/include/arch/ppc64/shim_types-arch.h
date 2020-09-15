/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2020 IBM Corporation */

#ifndef _SHIM_TYPES_ARCH_H_
#define _SHIM_TYPES_ARCH_H_

#include <assert.h>
#include <stdint.h>

#include "shim_tcb-arch.h"

/* asm/signal.h */
#define SIGS_CNT            64
#define KNOWN_SIGS_CNT      32

typedef struct {
    unsigned long __val[SIGS_CNT / (8 * sizeof(unsigned long))];
} __sigset_t;

// see: https://elixir.bootlin.com/linux/v5.11/source/arch/powerpc/include/asm/ptrace.h#L84
#define RED_ZONE_SIZE 	4096 /* FIXME: should be 512, but causes crashes */

/* powerpc64 needs this: */
struct timespec64 {
    int64_t tv_sec;
    long tv_nsec;
};


#endif /* _SHIM_TYPES_ARCH_H_ */
