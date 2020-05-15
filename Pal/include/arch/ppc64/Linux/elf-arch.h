/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2014 Stony Brook University
 * Copyright (C) 2020 IBM Corporation
 */

/*
 * dl-machine.h
 *
 * This files contain architecture-specific implementation of ELF dynamic
 * relocation function.
 * The source code is imported and modified from the GNU C Library.
 */

#define ELF_MACHINE_NAME "powerpc64"

#include <sys/param.h>
#include <sysdeps/generic/ldsodefs.h>

#include "pal_internal.h"

/* Return the run-time load address of the shared object.  */
static inline Elf64_Addr __attribute__((const)) elf_machine_load_address(void) {
    Elf64_Addr ret;

    /* The first entry in .got (and thus the first entry in .toc) is the
       link-time TOC_base, ie. r2.  So the difference between that and
       the current r2 set by the kernel is how far the shared lib has
       moved.  */
    __asm__("	ld	%0,-32768(2)\n"
         "	subf	%0,%0,2\n"
	: "=r"	(ret));
    return ret;
}

/* Return the link-time address of _DYNAMIC.  Conveniently, this is the
   first element of the GOT.  This must be inlined in a function which
   uses global data.  */
static inline Elf64_Addr __attribute__((unused)) elf_machine_dynamic(void) {
    Elf64_Addr runtime_dynamic;
    /* It's easier to get the run-time address.  */
    __asm__("	addis   %0,2,pal_start@toc@ha\n" /* trick used to pull in pal_start */
            "	addis	%0,2,_DYNAMIC@toc@ha\n"
            "	addi	%0,%0,_DYNAMIC@toc@l\n"
            : "=b"(runtime_dynamic));
    /* Then subtract off the load address offset.  */
    return runtime_dynamic - elf_machine_load_address() ;
}
