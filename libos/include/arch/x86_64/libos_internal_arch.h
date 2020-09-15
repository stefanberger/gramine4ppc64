/* SPDX-License-Identifier: LGPL-3.0-or-later */

#pragma once

#include "api.h"

#define LIBOS_ELF_HOST_MACHINE EM_X86_64

/* x86 never executes syscalls from within libos */
static inline bool is_valid_libos_syscall(PAL_CONTEXT *context) {
    __UNUSED(context);
    return false;
}
