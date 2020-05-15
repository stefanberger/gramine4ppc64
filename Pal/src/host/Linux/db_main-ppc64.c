/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2014 Stony Brook University */

/*
 * db_main-x86_64.c
 *
 * This file contains x86_64-specific functions of the PAL loader.
 */

#include "api.h"
#include "bogomips.h"
#include "cpu.h"
#include "pal_linux.h"

static char* get_cpuinfo_entry(const char* entry) {
    char buf[1024*100];
    ssize_t len;

    len = read_file_buffer("/proc/cpuinfo", buf, sizeof(buf) - 1);
    if (len < 0)
        return NULL;
    buf[len] = 0;

    return get_string_from_cpuinfo_buf(entry, buf);
}

int _DkGetCPUInfo (PAL_CPU_INFO* ci) {
    int cores = get_cpu_count();
    if (cores < 0) {
        return cores;
    }
    ci->cpu_num = cores;
    ci->cpu = get_cpuinfo_entry("cpu");
    ci->clock = get_cpuinfo_entry("clock");
    ci->revision = get_cpuinfo_entry("revision");
    ci->timebase = get_cpuinfo_entry("timebase");
    ci->platform = get_cpuinfo_entry("platform");
    ci->model = get_cpuinfo_entry("model");
    ci->machine = get_cpuinfo_entry("machine");
    ci->firmware = get_cpuinfo_entry("firmware");
    ci->mmu = get_cpuinfo_entry("MMU");

    return 0;
}
