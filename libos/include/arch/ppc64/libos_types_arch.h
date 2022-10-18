/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2020 IBM Corporation */

#pragma once

#include <assert.h>
#include <stdint.h>

#include "libos_tcb_arch.h"

/* asm/signal.h */
#define SIGS_CNT            64
#define KNOWN_SIGS_CNT      32

typedef struct {
    unsigned long __val[SIGS_CNT / (8 * sizeof(unsigned long))];
} __sigset_t;

// see: https://elixir.bootlin.com/linux/v5.11/source/arch/powerpc/include/asm/ptrace.h#L84
#define RED_ZONE_SIZE		512
