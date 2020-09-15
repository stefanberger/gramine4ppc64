/* SPDX-License-Identifier: LGPL-3.0-or-later */

#ifndef _SHIM_INTERNAL_ARCH_H_
#define _SHIM_INTERNAL_ARCH_H_

#define SHIM_ELF_HOST_MACHINE EM_X86_64

/* x86 never executes syscalls from within libos */
static inline bool is_valid_libos_syscall(PAL_CONTEXT *context) {
    return false;
}

#endif /* _SHIM_INTERNAL_ARCH_H_ */
