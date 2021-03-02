/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2020 IBM Corporation */

#ifndef _SHIM_TYPES_ARCH_H_
#define _SHIM_TYPES_ARCH_H_

#include <assert.h>
#include <stdint.h>

#include "shim_tcb-arch.h"

/* asm/signal.h */
#define NUM_SIGS            64
#define NUM_KNOWN_SIGS      32

typedef struct {
    unsigned long __val[NUM_SIGS / (8 * sizeof(unsigned long))];
} __sigset_t;

// FIXME: Check this
#define RED_ZONE_SIZE 	4096

#endif /* _SHIM_TYPES_ARCH_H_ */
