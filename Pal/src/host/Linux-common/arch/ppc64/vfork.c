/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2021 IBM Corporation */

#include <linux/sched.h>

#include "pal.h"

long vfork(void) {
    return DO_SYSCALL(clone, CLONE_VM | CLONE_VFORK, 0, NULL, NULL);
}

