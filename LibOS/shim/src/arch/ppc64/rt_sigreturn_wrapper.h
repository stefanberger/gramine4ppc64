/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2022 IBM Corporation */

#ifndef _LIBOS_SHIM_RT_SIGRETURN_WRAPPER_H
#define _LIBOS_SHIM_RT_SIGRETURN_WRAPPER_H

#include <stdnoreturn.h>

noreturn void rt_sigreturn_wrapper(void);

#endif /* _LIBOS_SHIM_RT_SIGRETURN_WRAPPER_H */
