/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2020 IBM Corporation */

#ifndef _SHIM_UCONTEXT_ARCH_H_
#define _SHIM_UCONTEXT_ARCH_H_

#include "shim_types.h"
#include "ucontext.h"

static inline void shim_regs_to_ucontext(ucontext_t* context, struct shim_regs* regs) {
    /* shim_regs being pt_regs, this is just a memcpy; ucontext's gp_regs is larger than pt_regs!*/
    assert(sizeof(context->uc_mcontext.gp_regs) >= sizeof(regs->pt_regs));
    memcpy(&context->uc_mcontext.gp_regs[0], &regs->pt_regs.gpr[0], sizeof(regs->pt_regs));
}

#endif /* _SHIM_UCONTEXT_ARCH_H_ */

