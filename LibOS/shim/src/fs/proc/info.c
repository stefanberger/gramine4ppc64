/* Copyright (C) 2014 Stony Brook University
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

/*!
 * \file
 *
 * This file contains the implementation of `/proc/meminfo` and `/proc/cpuinfo`.
 */

#include "shim_fs.h"

static int proc_info_mode(const char* name, mode_t* mode) {
    __UNUSED(name);
    *mode = FILE_R_MODE | S_IFREG;
    return 0;
}

static int proc_info_stat(const char* name, struct stat* buf) {
    __UNUSED(name);
    memset(buf, 0, sizeof(struct stat));
    buf->st_dev  = 1;    /* dummy ID of device containing file */
    buf->st_ino  = 1;    /* dummy inode number */
    buf->st_mode = FILE_R_MODE | S_IFREG;
    return 0;
}

static int proc_meminfo_open(struct shim_handle* hdl, const char* name, int flags) {
    __UNUSED(name);
    if (flags & (O_WRONLY | O_RDWR))
        return -EACCES;

    int len, max = 128;
    char* str = NULL;

    struct {
        const char* fmt;
        unsigned long val;
    } meminfo[] = {
        {
            "MemTotal:      %8lu kB\n",
            pal_control.mem_info.mem_total / 1024,
        },
        {
            "MemFree:       %8lu kB\n",
            DkMemoryAvailableQuota() / 1024,
        },
    };

retry:
    max *= 2;
    len = 0;
    free(str);
    str = malloc(max);
    if (!str)
        return -ENOMEM;

    for (size_t i = 0; i < ARRAY_SIZE(meminfo); i++) {
        int ret = snprintf(str + len, max - len, meminfo[i].fmt, meminfo[i].val);

        if (len + ret == max)
            goto retry;

        len += ret;
    }

    struct shim_str_data* data = malloc(sizeof(struct shim_str_data));
    if (!data) {
        free(str);
        return -ENOMEM;
    }

    memset(data, 0, sizeof(struct shim_str_data));
    data->str          = str;
    data->len          = len;
    hdl->type          = TYPE_STR;
    hdl->flags         = flags & ~O_RDONLY;
    hdl->acc_mode      = MAY_READ;
    hdl->info.str.data = data;
    return 0;
}

// FIXME: remove once global realloc is enabled
static void* realloc(void* ptr, size_t old_size, size_t new_size) {
    void* tmp = malloc(new_size);
    if (!tmp) {
        return NULL;
    }

    memcpy(tmp, ptr, old_size);

    free(ptr);

    return tmp;
}

static int print_to_str(char** str, size_t off, size_t* size, const char* fmt, ...) {
    int ret;
    va_list ap;

retry:
    va_start(ap, fmt);
    ret = vsnprintf(*str + off, *size - off, fmt, ap);
    va_end(ap);

    if (ret < 0) {
        return -EINVAL;
    }

    if ((size_t)ret >= *size - off) {
        char* tmp = realloc(*str, *size, *size + 128);
        if (!tmp) {
            return -ENOMEM;
        }
        *size += 128;
        *str = tmp;
        goto retry;
    }

    return ret;
}

static int proc_cpuinfo_open(struct shim_handle* hdl, const char* name, int flags) {
    // This function only serves one file
    __UNUSED(name);

    if (flags & (O_WRONLY | O_RDWR))
        return -EACCES;

    size_t len = 0,
           max = 128;
    char* str = malloc(max);
    if (!str) {
        return -ENOMEM;
    }

#define ADD_INFO(fmt, ...) do {                                         \
        int ret = print_to_str(&str, len, &max, fmt, ##__VA_ARGS__);    \
        if (ret < 0) {                                                  \
            free(str);                                                  \
            return ret;                                                 \
        }                                                               \
        len += ret;                                                     \
    } while (0)

    for (size_t n = 0; n < pal_control.cpu_info.cpu_num; n++) {
#if defined(__i386__) || defined(__x86_64__)
        /* Below strings must match exactly the strings retrieved from /proc/cpuinfo
         * (see Linux's arch/x86/kernel/cpu/proc.c) */
        ADD_INFO("processor\t: %lu\n", n);
        ADD_INFO("vendor_id\t: %s\n", pal_control.cpu_info.cpu_vendor);
        ADD_INFO("cpu family\t: %lu\n", pal_control.cpu_info.cpu_family);
        ADD_INFO("model\t\t: %lu\n", pal_control.cpu_info.cpu_model);
        ADD_INFO("model name\t: %s\n", pal_control.cpu_info.cpu_brand);
        ADD_INFO("stepping\t: %lu\n", pal_control.cpu_info.cpu_stepping);
        ADD_INFO("core id\t\t: %lu\n", n);
        ADD_INFO("cpu cores\t: %lu\n", pal_control.cpu_info.cpu_num);
        double bogomips = pal_control.cpu_info.cpu_bogomips;
        // Apparently graphene snprintf cannot into floats.
        ADD_INFO("bogomips\t: %lu.%02lu\n",
                 (unsigned long)bogomips,
                 (unsigned long)(bogomips * 100.0 + 0.5) % 100);
#elif defined(__powerpc64__)
        ADD_INFO("processor\t: %lu\n", n);
        ADD_INFO("cpu\t\t: %s\n", pal_control.cpu_info.cpu);
        ADD_INFO("clock\t\t: %s\n", pal_control.cpu_info.clock);
        ADD_INFO("revision\t: %s\n", pal_control.cpu_info.revision);
#endif
        ADD_INFO("\n");
    }

#if defined(__powerpc64__)
    ADD_INFO("timebase\t: %s\n", pal_control.cpu_info.timebase);
    ADD_INFO("platform\t: %s\n", pal_control.cpu_info.platform);
    ADD_INFO("model\t\t: %s\n", pal_control.cpu_info.model);
    ADD_INFO("machine\t: %s\n", pal_control.cpu_info.machine);
    ADD_INFO("firmware\t: %s\n", pal_control.cpu_info.firmware);
    ADD_INFO("MMU\t\t: %s\n", pal_control.cpu_info.mmu);
#endif

#undef ADD_INFO

    struct shim_str_data* data = calloc(1, sizeof(struct shim_str_data));
    if (!data) {
        free(str);
        return -ENOMEM;
    }

    data->str          = str;
    data->len          = len;
    hdl->type          = TYPE_STR;
    hdl->flags         = flags & ~O_RDONLY;
    hdl->acc_mode      = MAY_READ;
    hdl->info.str.data = data;
    return 0;
}

struct pseudo_fs_ops fs_meminfo = {
    .mode = &proc_info_mode,
    .stat = &proc_info_stat,
    .open = &proc_meminfo_open,
};

struct pseudo_fs_ops fs_cpuinfo = {
    .mode = &proc_info_mode,
    .stat = &proc_info_stat,
    .open = &proc_cpuinfo_open,
};
