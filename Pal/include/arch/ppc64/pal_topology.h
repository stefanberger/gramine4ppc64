/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2022 Intel Corporation
 *                    Michał Kowalczyk <mkow@invisiblethingslab.com>
 * Copyright (C) 2020 IBM Corporation
 */

#ifndef PAL_TOPOLOGY_H
#define PAL_TOPOLOGY_H

// TODO: drop after removing PAL_NUM from here
typedef uint64_t PAL_NUM;

/* Used to represent plain integers (only numeric values) */
#define PAL_SYSFS_INT_FILESZ 16
/* Used to represent buffers having numeric values with text. E.g "1024576K" */
#define PAL_SYSFS_BUF_FILESZ 64
/* Used to represent cpumaps like "00000000,ffffffff,00000000,ffffffff" */
#define PAL_SYSFS_MAP_FILESZ 256

enum {
    HUGEPAGES_2M = 0,
    HUGEPAGES_1G,
    HUGEPAGES_MAX,
};

typedef struct PAL_CORE_CACHE_INFO_ {
    char shared_cpu_map[PAL_SYSFS_MAP_FILESZ];
    char level[PAL_SYSFS_INT_FILESZ];
    char type[PAL_SYSFS_BUF_FILESZ];
    char size[PAL_SYSFS_BUF_FILESZ];
    char coherency_line_size[PAL_SYSFS_INT_FILESZ];
    char number_of_sets[PAL_SYSFS_INT_FILESZ];
#if 0
    char physical_line_partition[PAL_SYSFS_INT_FILESZ]; // on x86_64
#endif
} PAL_CORE_CACHE_INFO;

typedef struct PAL_CORE_TOPO_INFO_ {
    /* [0] element is uninitialized because core 0 is always online */
    char is_logical_core_online[PAL_SYSFS_INT_FILESZ];
    char core_id[PAL_SYSFS_INT_FILESZ];
    char core_siblings[PAL_SYSFS_MAP_FILESZ];
    char thread_siblings[PAL_SYSFS_MAP_FILESZ];
    PAL_CORE_CACHE_INFO* cache; /* Array of size cache_indices_cnt, owned by this struct */
} PAL_CORE_TOPO_INFO;

typedef struct PAL_NUMA_HUGEPAGE_INFO_ {
    char nr_hugepages[PAL_SYSFS_INT_FILESZ];
} PAL_NUMA_HUGEPAGE_INFO;

typedef struct PAL_NUMA_TOPO_INFO_ {
    char cpumap[PAL_SYSFS_MAP_FILESZ];
    char distance[PAL_SYSFS_INT_FILESZ];
    PAL_NUMA_HUGEPAGE_INFO hugepages[HUGEPAGES_MAX];
} PAL_NUMA_TOPO_INFO;

struct pal_topo_info {
   /* Number of logical cores available on the host. */
    size_t online_logical_cores_cnt;

    char online_logical_cores[PAL_SYSFS_BUF_FILESZ];

    /* Array of "logical core -> socket" mappings; has online_logical_cores_cnt elements. */
    size_t* cpu_to_socket;

    /* Array of logical core topology info, owned by this struct. Has online_logical_cores_cnt
     * elements. */
    PAL_CORE_TOPO_INFO* core_topology;

    /* Max number of logical cores available on the host. */
    size_t possible_logical_cores_cnt;

    char possible_logical_cores[PAL_SYSFS_BUF_FILESZ];

    /* Number of physical cores in a socket (physical package). */
    size_t physical_cores_per_socket;

    /* Number of nodes available on the host. */
    size_t online_nodes_cnt;

    char online_nodes[PAL_SYSFS_BUF_FILESZ];

    /* Array of numa topology info, owned by this struct. Has online_nodes_cnt elements. */
    PAL_NUMA_TOPO_INFO* numa_topology;

    /* Number of cache levels (such as L2 or L3) available on the host. */
    PAL_NUM cache_indices_cnt;
};

#endif /* PAL_TOPOLOGY_H */
