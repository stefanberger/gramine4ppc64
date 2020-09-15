/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2022 IBM Corporation */

#include "api.h"
#include "pal.h"
#include "shim_fs_proc.h"

#define ADD_INFO(fmt, ...)                                            \
    do {                                                              \
        int ret = print_to_str(str, *size, max, fmt, ##__VA_ARGS__);  \
        if (ret < 0) {                                                \
            return ret;                                               \
        }                                                             \
        *size += ret;                                                 \
    } while (0)

int proc_cpuinfo_display_cpu(char** str, size_t* size, size_t* max,
                             const struct pal_topo_info* topo,
                             const struct pal_cpu_info* cpu, size_t i,
                             struct pal_cpu_thread_info* thread) {
    __UNUSED(topo);
    __UNUSED(thread);

    ADD_INFO("processor\t: %lu\n", i);
    ADD_INFO("cpu\t\t: %s\n", cpu->cpu);
    ADD_INFO("clock\t\t: %s\n", cpu->clock);
    ADD_INFO("revision\t: %s\n", cpu->revision);
    ADD_INFO("\n");

    return 0;
}

int proc_cpuinfo_display_tail(char** str, size_t* size, size_t* max,
                              const struct pal_cpu_info* cpu) {
    ADD_INFO("timebase\t: %s\n", cpu->timebase);
    ADD_INFO("platform\t: %s\n", cpu->platform);
    ADD_INFO("model\t\t: %s\n", cpu->model);
    ADD_INFO("machine\t\t: %s\n", cpu->machine);
    ADD_INFO("firmware\t: %s\n", cpu->firmware);
    ADD_INFO("MMU\t\t: %s\n", cpu->mmu);

    return 0;
}
