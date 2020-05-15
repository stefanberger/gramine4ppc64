/*
   This file is part of Graphene Library OS.

   Graphene Library OS is free software: you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License
   as published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   Graphene Library OS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#ifndef _PPC64_RNG_ARCH_H_
#define _PPC64_RNG_ARCH_H_

/* get a 64 bit random number */
static inline unsigned long long get_rand64(void) {
    unsigned long long rand64;
    do {
        __asm__ ("darn %0,2" : "=r"(rand64));
    } while (rand64 == (unsigned long long)~0);
    return rand64;
}

#endif /* _PPC64_RNG_ARCH_H_ */
