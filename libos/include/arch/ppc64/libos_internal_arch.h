/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2020 IBM Corporation */
/*
 * shim_internal-arch.h
 */

#pragma once

#include <stdnoreturn.h>

#include "pal.h"
#include "gramine_entry_api.h"
#include "stack.h"

#define LIBOS_ELF_HOST_MACHINE EM_PPC64

noreturn void return_from_syscall_emulation(void);

/* On ppc64 there's one syscall that may be executed from within libos */
static inline bool is_valid_libos_syscall(PAL_CONTEXT *context) {
    return (INSN_IS_SC(*(uint32_t *)context->gpregs.nip) &&
        context->gpregs.gpr[0] == GRAMINE_CUSTOM_SYSCALL_RETURN_FROM_SYSCALL_EMULATION);
}
