/* SPDX-License-Identifier: LGPL-3.0-or-later */

/*
 * shim_context.h
 *
 * This file contains definitions for CPU context.
 */

#ifndef _SHIM_CONTEXT_H_
#define _SHIM_CONTEXT_H_

#include "shim_tcb.h"

void restore_context(struct shim_context* context);
void fixup_child_context(struct shim_regs* regs, void* stack, uint64_t fs_base);

#endif /* _SHIM_CONTEXT_H_ */
