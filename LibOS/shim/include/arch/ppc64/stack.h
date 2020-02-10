/* Copyright (C) 2020 IBM Corporation
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
#ifndef _PAL_STACK_H
#define _PAL_STACK_H

struct stackframe {
    void *backchain;		// stack points here
    uint64_t cr_save;		// 8(r1)
    uint64_t lr_save;		// 16(r1)
    uint64_t toc_save;		// 24(r1)
    uint64_t parm_save[];	// 32(r1)
};

#endif

