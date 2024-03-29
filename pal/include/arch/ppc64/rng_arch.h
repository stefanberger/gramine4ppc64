/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2020 IBM Corporation */

#pragma once

/* get a 64 bit random number */
static inline unsigned long long get_rand64(void) {
    unsigned long long rand64;
    do {
        __asm__ ("darn %0,2" : "=r"(rand64));
    } while (rand64 == (unsigned long long)~0);
    return rand64;
}
