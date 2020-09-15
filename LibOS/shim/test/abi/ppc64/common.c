/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2022 Intel Corporation
 *                    Mariusz Zaborski <oshogbo@invisiblethingslab.com>
 * Copyright (C) 2022 IBM Corporation
 */

#include "common.h"

int test_str_neq(const char* orig, const char* new) {
    if (orig == new)
        return 0;

    while (*orig && *orig == *new) {
        orig++;
        new++;
    }

    return *orig != *new;
}
