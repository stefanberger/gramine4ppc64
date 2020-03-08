#ifndef _SHIM_ATOMIC_H_
#define _SHIM_ATOMIC_H_

/* Copyright (C) 2014 Stony Brook University
 * Copyright (C) 2017 Fortanix Inc, and University of North Carolina
 * at Chapel Hill.
 * Copyright (C) 2020 IBM Corporation
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

/*
/----------------------------------------------------------------------
Copyright (C) 2005-2014 Rich Felker, et al.

    Permission is hereby granted, free of charge, to any person obtaining
    a copy of this software and associated documentation files (the
    "Software"), to deal in the Software without restriction, including
    without limitation the rights to use, copy, modify, merge, publish,
    distribute, sublicense, and/or sell copies of the Software, and to
    permit persons to whom the Software is furnished to do so, subject to
    the following conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
    ----------------------------------------------------------------------
*/

#include <stdint.h>
#include <stddef.h>

/* Optimization barrier */
#define COMPILER_BARRIER() __asm__ __volatile__("": : :"memory")
#define CPU_RELAX() __asm__ __volatile__("or 1,1,1; or 2,2,2" ::: "memory")

#define MB() __asm__ __volatile__("": : :"memory")

struct atomic_int {
    volatile int64_t counter;
};

#define ATOMIC_INIT(i)      { (i) }

/* Read the value currently stored in the atomic_int */
static inline int64_t atomic_read (const struct atomic_int * v)
{
    //  Effectively:
    //      return v->counter;
    int64_t i;
    /* Use inline assembly to ensure this is one instruction */
    __asm__ __volatile__("ld %0, %2(%1)"
                         : "=r"(i)
                         : "r"(v),
                           "i"(offsetof(struct atomic_int, counter))
    );
    return i;
}

/* Does a blind write to the atomic variable */
static inline void atomic_set (struct atomic_int * v, int64_t i)
{
    //  Effectively:
    //      v->counter = i;
    /* Use inline assembly to ensure this is one instruction */
    __asm__ __volatile__("std %0, %2(%1)"
                         :
                         : "r"(i),
                           "r"(v),
                           "i"(offsetof(struct atomic_int, counter))
   );
}

/* Helper function that atomically adds a value to an atomic_int,
 * and returns the _new_ value. */
static inline int64_t _atomic_add (int64_t i, struct atomic_int * v)
{
    int64_t result;

    __asm__ __volatile__(
"0:      ldarx   %0,0,%2 \n\t"
        "add     %0,%0,%3 \n\t"
        "stdcx.	 %0,0,%2 \n\t"
        "bne-    0b \n\t"
        : "=&r" (result), "+m" (v->counter)
        : "r" (&v->counter), "r" (i)
        : "cc", "memory"
    );

    return result;
}

/* Atomically adds i to v.  Does not return a value. */
static inline void atomic_add (int64_t i, struct atomic_int * v)
{
    _atomic_add(i, v);
}

/* Atomically substracts i from v.  Does not return a value. */
static inline void atomic_sub (int64_t i, struct atomic_int * v)
{
    _atomic_add(-i, v);
}

/* Atomically adds 1 to v.  Does not return a value. */
static inline void atomic_inc (struct atomic_int * v)
{
    _atomic_add(1, v);
}

/* Atomically substracts 1 from v.  Does not return a value. */
static inline void atomic_dec (struct atomic_int * v)
{
    _atomic_add(-1, v);
}

/* Atomically substracts 1 from v.  Returns 1 if this causes the
   value to reach 0; returns 0 otherwise. */
static inline int64_t atomic_dec_and_test (struct atomic_int * v)
{
    int64_t i = _atomic_add(-1, v);
    return i == 0;
}

/* Helper function to atomically compare-and-swap the value pointed to by p.
 * t is the old value, s is the new value.  Returns
 * the value originally in p. */
static inline int64_t cmpxchg(volatile int64_t *p, int64_t t, int64_t s)
{
    // FIXME: Needs proper implementation
    int64_t orig = *p;
    if (orig == t)
        *p = s;
    return orig;
}

#define atomic_add_return(i, v)  _atomic_add(i, v)
#define atomic_inc_return(v)     _atomic_add(1, v)

/* Helper function to atomically compare-and-swap the value in v.
 * If v == old, it sets v = new.
 * Returns the value originally in v. */
static inline int64_t atomic_cmpxchg (struct atomic_int * v, int64_t old, int64_t new)
{
    return cmpxchg(&v->counter, old, new);
}

#endif /* _SHIM_ATOMIC_H_ */
