/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2022 IBM Corporation
 */

#include <stdlib.h>

#include "common.h"

int verify_env(const char *envp[]);

int verify_env(const char *envp[])
{
    if (test_str_neq("foo=bar", envp[0]))
        return 1;
    if (test_str_neq("env0=val0", envp[1]))
        return 2;
    if (envp[2] != NULL)
        return 3;
    return 0;
}