/*
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

#ifndef _SHIM_UCONTEXT_ARCH_H_
#define _SHIM_UCONTEXT_ARCH_H_

#include <shim_types.h>
#include <ucontext.h>

static inline void shim_regs_to_ucontext(ucontext_t* context, struct shim_regs* regs) {
    /* shim_regs being pt_regs, this is just a memcpy; ucontext's gp_regs is larger than pt_regs!*/
    assert(sizeof(context->uc_mcontext.gp_regs) >= sizeof(regs));
    __builtin_memcpy(&context->uc_mcontext.gp_regs[0], &regs->gpr[0], sizeof(regs));
}

#endif /* _SHIM_UCONTEXT_ARCH_H_ */

