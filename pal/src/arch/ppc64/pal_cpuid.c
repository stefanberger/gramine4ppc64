/* SPDX-License-Identifier: LGPL-3.0-or-later */

#include "api.h"
#include "cpu.h"
#include "linux_utils.h"
#include "pal_linux.h"
#include "topo_info.h"

static char* get_cpuinfo_entry(const char* entry) {
    char buf[1024*100];
    ssize_t len;

    len = read_file_buffer("/proc/cpuinfo", buf, sizeof(buf) - 1);
    if (len < 0)
        return NULL;
    buf[len] = 0;

    return get_string_from_cpuinfo_buf(entry, buf);
}

int _PalGetCPUInfo (struct pal_cpu_info* ci) {
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
