#ifndef _SHIM_ATOMIC_H_
#define _SHIM_ATOMIC_H_

/* Copyright (C) 2014 Stony Brook University
 * Copyright (C) 2017 Fortanix Inc, and University of North Carolina
 * at Chapel Hill.
 *
 * This file defines atomic operations (And barriers) for use in
 * Graphene.
 *
 * The atomic operation assembly code is taken from musl libc, which
 * is subject to the MIT license.
 *
 * At this point, we primarily focus on x86_64; there are some vestigial
 * 32-bit definitions here, but a more portable version would need to
 * move and reimplement portions of this for 32-bit x86 (or other architectures).
 */

#ifdef __x86_64__
# include "atomic-x86_64.h"
#elif defined (__powerpc64__)
# include "atomic-ppc64.h"
#endif

#endif
