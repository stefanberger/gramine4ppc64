/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2022 IBM Corporation
 *
 */
#pragma once

#include "gramine_entry_api.h"

static inline long vdso_arch_syscall(unsigned long syscallnr,
                                     unsigned long arg1, unsigned long arg2) {
    return gramine_syscall(syscallnr, arg1, arg2, 0, 0, 0, 0);
}
