/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2023 Intel Corporation
 *                    Michał Kowalczyk <mkow@invisiblethingslab.com>
 */

#pragma once

/* Types and structures used by various Linux ABIs (e.g. syscalls). */
/* These need to be binary-identical with the ones used by Linux. */

#if defined(__x86_64__)
#include <asm/prctl.h>
#endif
#include <linux/sched.h> // for CLONE_* stuff
#include <linux/wait.h>
