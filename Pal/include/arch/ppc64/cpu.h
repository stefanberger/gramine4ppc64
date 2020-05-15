/* SPDX-License-Identifier: LGPL-3.0-or-later */

#ifndef CPU_H
#define CPU_H

#define PAGE_SIZE       (1 << 16)
#define PRESET_PAGESIZE PAGE_SIZE

static inline void cpu_pause(void) {
}

/* Optimization barrier */
#define CPU_RELAX() __asm__ __volatile__("or 1,1,1; or 2,2,2" ::: "memory")

#define MB() __asm__ __volatile__("": : :"memory")

#endif /* CPU_H */
