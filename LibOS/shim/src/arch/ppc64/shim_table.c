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

/*
 * shim_table-ppc64.c
 *
 * This file contains the system call table used by application libraries.
 */

#include <linux/sched.h>

#include "shim_internal.h"
#include "shim_table.h"
#include "shim_table-arch.h"

shim_fp shim_table[LIBOS_SYSCALL_BOUND] = {
    (shim_fp)0,//shim_do_restart_syscall,
    [__NR_exit]                   = (shim_fp)shim_do_exit,
    [__NR_fork]                   = (shim_fp)shim_do_fork,
    [__NR_read]                   = (shim_fp)shim_do_read,
    [__NR_write]                  = (shim_fp)shim_do_write,
    [__NR_open]                   = (shim_fp)shim_do_open,			/* 5 */
    [__NR_close]                  = (shim_fp)shim_do_close,
    [__NR_waitpid]                = (shim_fp)shim_do_waitpid,
    [__NR_creat]                  = (shim_fp)shim_do_creat,
    (shim_fp)0, // shim_do_link,
    [__NR_unlink]                 = (shim_fp)shim_do_unlink,			/* 10 */
    [__NR_execve]                 = (shim_fp)shim_do_execve,
    [__NR_chdir]                  = (shim_fp)shim_do_chdir,
    [__NR_time]                   = (shim_fp)shim_do_time,
    [__NR_mknod]                  = (shim_fp)shim_do_mknod,
    [__NR_chmod]                  = (shim_fp)shim_do_chmod,			/* 15 */
    (shim_fp)0, // shim_do_lchown,
    (shim_fp)0,
    [__NR_oldstat]                = (shim_fp)shim_do_stat,
    [__NR_lseek]                  = (shim_fp)shim_do_lseek,
    [__NR_getpid]                 = (shim_fp)shim_do_getpid,			/* 20 */
    (shim_fp)0, // shim_do_mount,
    (shim_fp)0,
    [__NR_setuid]                 = (shim_fp)shim_do_setuid,
    [__NR_getuid]                 = (shim_fp)shim_do_getuid,
    (shim_fp)0,					/* 25 */
    (shim_fp)0, // shim_do_ptrace,
    [__NR_alarm]                  = (shim_fp)shim_do_alarm,
    [__NR_oldfstat]               = (shim_fp)shim_do_fstat,
    [__NR_pause]                  = (shim_fp)shim_do_pause,
    (shim_fp)0, // shim_do_utime,		/* 30 */
    (shim_fp)0,
    (shim_fp)0,
    [__NR_access]                 = (shim_fp)shim_do_access,
    (shim_fp)0, // shim_do_nice
    (shim_fp)0,					/* 35 */
    (shim_fp)0, // shim_do_sync,
    [__NR_kill]                  = (shim_fp)shim_do_kill,
    [__NR_rename]                = (shim_fp)shim_do_rename,
    [__NR_mkdir]                 = (shim_fp)shim_do_mkdir,
    [__NR_rmdir]                 = (shim_fp)shim_do_rmdir,			/* 40 */
    [__NR_dup]                   = (shim_fp)shim_do_dup,
    [__NR_pipe]                  = (shim_fp)shim_do_pipe,
    (shim_fp)0, // shim_do_times,
    (shim_fp)0,
    [__NR_brk]                   = (shim_fp)shim_do_brk,			/* 45 */
    [__NR_setgid]                = (shim_fp)shim_do_setgid,
    [__NR_getgid]                = (shim_fp)shim_do_getgid,
    (shim_fp)0,//shim_do_signal
    [__NR_geteuid]               = (shim_fp)shim_do_geteuid,
    [__NR_getegid]               = (shim_fp)shim_do_getegid,			/* 50 */
    (shim_fp)0, // shim_do_acct,
    (shim_fp)0, // shim_do_umount2,
    (shim_fp)0,
    [__NR_ioctl]                 = (shim_fp)shim_do_ioctl,
    [__NR_fcntl]                 = (shim_fp)shim_do_fcntl,			/* 55 */
    (shim_fp)0,
    [__NR_setpgid]               = (shim_fp)shim_do_setpgid,
    (shim_fp)0,
    (shim_fp)0,
    [__NR_umask]                 = (shim_fp)shim_do_umask,			/* 60 */
    [__NR_chroot]                = (shim_fp)shim_do_chroot,
    (shim_fp)0, // shim_do_ustat,
    [__NR_dup2]                  = (shim_fp)shim_do_dup2,
    [__NR_getppid]               = (shim_fp)shim_do_getppid,
    [__NR_getpgrp]               = (shim_fp)shim_do_getpgrp,			/* 65 */
    [__NR_setsid]                = (shim_fp)shim_do_setsid,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0, // shim_do_setreuid,		/* 70 */
    (shim_fp)0, // shim_do_setregid,
    (shim_fp)0,
    [__NR_sigpending]           = (shim_fp)shim_do_rt_sigpending,
    [__NR_sethostname]          = (shim_fp)shim_do_sethostname,
    [__NR_setrlimit]            = (shim_fp)shim_do_setrlimit,			/* 75 */
    (shim_fp)0, // shim_do_getrlimit
    (shim_fp)0, // shim_do_getrusage,
    [__NR_gettimeofday]         = (shim_fp)shim_do_gettimeofday,
    (shim_fp)0, // shim_do_settimeofday,
    [__NR_getgroups]            = (shim_fp)shim_do_getgroups,			/* 80 */
    [__NR_setgroups]            = (shim_fp)shim_do_setgroups,
    [__NR_select]               = (shim_fp)shim_do_select,  // ppc_select??
    (shim_fp)0, // shim_do_symlink,
    [__NR_oldlstat]             = (shim_fp)shim_do_lstat,
    [__NR_readlink]             = (shim_fp)shim_do_readlink,			/* 85 */
    (shim_fp)0, // shim_do_uselib,
    (shim_fp)0, // shim_do_swapon,
    (shim_fp)0, // shim_do_reboot,
    (shim_fp)0,
    [__NR_mmap]                 = (shim_fp)shim_do_mmap,			/* 90 */
    [__NR_munmap]               = (shim_fp)shim_do_munmap,
    [__NR_truncate]             = (shim_fp)shim_do_truncate,
    [__NR_ftruncate]            = (shim_fp)shim_do_ftruncate,
    [__NR_fchmod]               = (shim_fp)shim_do_fchmod,
    [__NR_fchown]               = (shim_fp)shim_do_fchown,			/* 95 */
    [__NR_getpriority]          = (shim_fp)shim_do_getpriority,
    [__NR_setpriority]          = (shim_fp)shim_do_setpriority,
    (shim_fp)0,
    [__NR_statfs]               = (shim_fp)shim_do_statfs,
    [__NR_fstatfs]              = (shim_fp)shim_do_fstatfs,			/* 100 */
    (shim_fp)0,
    [__NR_socketcall]           = (shim_fp)shim_do_socketcall,
    (shim_fp)0, // shim_do_syslog,
    [__NR_setitimer]            = (shim_fp)shim_do_setitimer,
    [__NR_getitimer]            = (shim_fp)shim_do_getitimer,			/* 105 */
    [__NR_stat]                 = (shim_fp)shim_do_stat,
    [__NR_lstat]                = (shim_fp)shim_do_lstat,
    [__NR_fstat]                = (shim_fp)shim_do_fstat,
    [__NR_olduname]             = (shim_fp)shim_do_uname,
    (shim_fp)0,					/* 110 */
    (shim_fp)0, // shim_do_vhangup,
    (shim_fp)0,
    (shim_fp)0,
    [__NR_wait4]                = (shim_fp)shim_do_wait4,
    (shim_fp)0, // shim_do_swapoff,		/* 115 */
    (shim_fp)0, // shim_do_sysinfo,
    (shim_fp)0, // shim_do_ipc,
    [__NR_fsync]                = (shim_fp)shim_do_fsync,
    (shim_fp)0,
    [__NR_clone]                = (shim_fp)shim_do_clone,			/* 120 */
    [__NR_setdomainname]        = (shim_fp)shim_do_setdomainname,
    [__NR_uname]                = (shim_fp)shim_do_uname,  // newuname ?
    (shim_fp)0,
    (shim_fp)0, // shim_do_adjtimex,
    [__NR_mprotect]             = (shim_fp)shim_do_mprotect,			/* 125 */
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0, // shim_do_init_module,
    (shim_fp)0, // shim_do_delete_module,
    (shim_fp)0,					/* 130 */
    (shim_fp)0, // shim_do_quotactl,
    [__NR_getpgid]              = (shim_fp)shim_do_getpgid,
    [__NR_fchdir]               = (shim_fp)shim_do_fchdir,
    (shim_fp)0,
    (shim_fp)0, // shim_do_sysfs,		/* 135 */
    (shim_fp)0, // shim_do_personality,
    (shim_fp)0,
    (shim_fp)0, // shim_do_setfsuid,
    (shim_fp)0, // shim_do_setfsgid,
    [__NR__llseek]              = (shim_fp)shim_do__llseek,			/* 140 */
    [__NR_getdents]             = (shim_fp)shim_do_getdents,
    [__NR__newselect]           = (shim_fp)shim_do_select,
    (shim_fp)0, // shim_do_flock,
    [__NR_msync]                = (shim_fp)shim_do_msync,
    [__NR_readv]                = (shim_fp)shim_do_readv,			/* 145 */
    [__NR_writev]               = (shim_fp)shim_do_writev,
    [__NR_getsid]               = (shim_fp)shim_do_getsid,
    [__NR_fdatasync]            = (shim_fp)shim_do_fdatasync,
    (shim_fp)0, // sysctl
    (shim_fp)0, // shim_do_mlock,		/* 150 */
    (shim_fp)0, // shim_do_munlock,
    (shim_fp)0, // shim_do_mlockall,
    (shim_fp)0, // shim_do_munlockall,
    [__NR_sched_setparam]       = (shim_fp)shim_do_sched_setparam,
    [__NR_sched_getparam]       = (shim_fp)shim_do_sched_getparam,		/* 155 */
    [__NR_sched_setscheduler]   = (shim_fp)shim_do_sched_setscheduler,
    [__NR_sched_getscheduler]   = (shim_fp)shim_do_sched_getscheduler,
    [__NR_sched_yield]          = (shim_fp)shim_do_sched_yield,
    [__NR_sched_get_priority_max] = (shim_fp)shim_do_sched_get_priority_max,
    [__NR_sched_get_priority_min] = (shim_fp)shim_do_sched_get_priority_min,	/* 160 */
    [__NR_sched_rr_get_interval ] = (shim_fp)shim_do_sched_rr_get_interval,
    [__NR_nanosleep]            = (shim_fp)shim_do_nanosleep,
    (shim_fp)0, //shim_do_mremap,
    (shim_fp)0, //shim_do_setresuid,
    (shim_fp)0, // shim_do_getresuid,			/* 165 */
    (shim_fp)0,
    [__NR_poll]                 = (shim_fp)shim_do_poll,
    (shim_fp)0,
    (shim_fp)0, // shim_do_setresgid,
    (shim_fp)0, // shim_do_getresgid,			/* 170 */
    (shim_fp)0, // shim_do_prctl,
    [__NR_rt_sigreturn]         = (shim_fp)shim_do_rt_sigreturn,
    [__NR_rt_sigaction]         = (shim_fp)shim_do_rt_sigaction,
    [__NR_rt_sigprocmask]       = (shim_fp)shim_do_rt_sigprocmask,
    [__NR_rt_sigpending]        = (shim_fp)shim_do_rt_sigpending,		/* 175 */
    [__NR_rt_sigtimedwait]      = (shim_fp)shim_do_rt_sigtimedwait,
    (shim_fp)0, // shim_do_rt_sigqueueinfo,
    [__NR_rt_sigsuspend]        = (shim_fp)shim_do_rt_sigsuspend,
    [__NR_pread64]              = (shim_fp)shim_do_pread64,
    [__NR_pwrite64]             = (shim_fp)shim_do_pwrite64,			/* 180 */
    [__NR_chown]                = (shim_fp)shim_do_chown,
    [__NR_getcwd]               = (shim_fp)shim_do_getcwd,
    (shim_fp)0, // shim_do_capget,
    (shim_fp)0, // shim_do_capset,
    [__NR_sigaltstack]          = (shim_fp)shim_do_sigaltstack,		/* 185 */
    [__NR_sendfile]             = (shim_fp)shim_do_sendfile,
    (shim_fp)0,
    (shim_fp)0,
    [__NR_vfork]                = (shim_fp)shim_do_vfork,
    [__NR_ugetrlimit]           = (shim_fp)shim_do_getrlimit,			/* 190 */
    (shim_fp)0, // shim_do_readahead,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,					/* 195 */
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0, // shim_do_pciconfig_read,
    (shim_fp)0, // shim_do_pciconfig_write,
    (shim_fp)0, // shim_do_pciconfig_iobase,	/* 200 */
    (shim_fp)0,
    [__NR_getdents64]           = (shim_fp)shim_do_getdents64,
    (shim_fp)0, // shim_do_pivot_root,
    (shim_fp)0,
    [__NR_madvise]              = (shim_fp)shim_do_madvise,			/* 205 */
    [__NR_mincore]              = (shim_fp)shim_do_mincore,
    [__NR_gettid]               = (shim_fp)shim_do_gettid,
    [__NR_tkill]                = (shim_fp)shim_do_tkill,
    (shim_fp)0, // shim_do_setxattr,
    (shim_fp)0, // shim_do_lsetxattr,			/* 210 */
    (shim_fp)0, // shim_do_fsetxattr,
    (shim_fp)0, // shim_do_getxattr,
    (shim_fp)0, // shim_do_lgetxattr,
    (shim_fp)0, // shim_do_fgetxattr,
    (shim_fp)0, // shim_do_listxattr,		/* 215 */
    (shim_fp)0, // shim_do_llistxattr,
    (shim_fp)0, // shim_do_flistxattr,
    (shim_fp)0, // shim_do_removexattr,
    (shim_fp)0, // shim_do_lremovexattr,
    (shim_fp)0, // shim_do_fremovexattr,		/* 220 */
    [__NR_futex]                = (shim_fp)shim_do_futex,
    [__NR_sched_setaffinity]    = (shim_fp)shim_do_sched_setaffinity,
    [__NR_sched_getaffinity]    = (shim_fp)shim_do_sched_getaffinity,
    (shim_fp)0,
    (shim_fp)0,					/* 225 */
    (shim_fp)0,
    (shim_fp)0, // shim_do_io_setup,
    (shim_fp)0, // shim_do_io_destroy,
    (shim_fp)0, // shim_do_io_getevents,
    (shim_fp)0, // shim_do_io_submit,			/* 230 */
    (shim_fp)0, // shim_do_io_cancel,
    [__NR_set_tid_address]      = (shim_fp)shim_do_set_tid_address,
    (shim_fp)0, //shim_do_fadvise64,
    [__NR_exit_group]           = (shim_fp)shim_do_exit_group,
    (shim_fp)0, //shim_do_lookup_dcookie,	/* 235 */
    [__NR_epoll_create]         = (shim_fp)shim_do_epoll_create,
    [__NR_epoll_ctl]            = (shim_fp)shim_do_epoll_ctl,
    [__NR_epoll_wait]           = (shim_fp)shim_do_epoll_wait,
    (shim_fp)0, //shim_do_remap_file_pages,
    (shim_fp)0, //shim_do_timer_create,		/* 240 */
    (shim_fp)0, //shim_do_timer_settime,
    (shim_fp)0, //shim_do_timer_gettime,
    (shim_fp)0, //shim_do_timer_getoverrun,
    (shim_fp)0, //shim_do_timer_delete,
    (shim_fp)0, // shim_do_clock_settime,	/* 245 */
    [__NR_clock_gettime]        = (shim_fp)shim_do_clock_gettime,
    [__NR_clock_getres]         = (shim_fp)shim_do_clock_getres,
    [__NR_clock_nanosleep]      = (shim_fp)shim_do_clock_nanosleep,
    (shim_fp)0, // ppc64_swapcontext
    [__NR_tgkill]               = (shim_fp)shim_do_tgkill,			/* 250 */
    (shim_fp)0, //shim_do_utimes,
    [__NR_statfs64]             = (shim_fp)shim_do_statfs, // statfs64
    [__NR_fstatfs64]            = (shim_fp)shim_do_fstatfs,// fstatfs64
    (shim_fp)0,
    (shim_fp)0, // shim_do_rtas			/* 255 */
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0, //shim_do_migrate_pages,
    [__NR_mbind]                = (shim_fp)shim_do_mbind,
    (shim_fp)0, //shim_do_get_mempolicy,		/* 260 */
    (shim_fp)0, //shim_do_set_mempolicy,
    (shim_fp)0, //shim_do_mq_open,
    (shim_fp)0, //shim_do_mq_unlink,
    (shim_fp)0, //shim_do_mq_timedsend,
    (shim_fp)0, //shim_do_mq_timedreceive,		/* 265 */
    (shim_fp)0, //shim_do_mq_notify,
    (shim_fp)0, //shim_do_mq_getsetattr,
    (shim_fp)0, //shim_do_kexec_load,
    (shim_fp)0, //shim_do_add_key,
    (shim_fp)0, //shim_do_request_key,		/* 270 */
    (shim_fp)0, //shim_do_keyctl,
    [__NR_waitid]               = (shim_fp)shim_do_waitid,
    (shim_fp)0, //shim_do_ioprio_set,
    (shim_fp)0, //shim_do_ioprio_get,
    (shim_fp)0, //shim_do_inotify_init,		/* 275 */
    (shim_fp)0, //shim_do_inotify_add_watch,
    (shim_fp)0, //shim_do_inotify_rm_watch,
    (shim_fp)0,
    (shim_fp)0,
    [__NR_pselect6]             = (shim_fp)shim_do_pselect6,			/* 280 */
    [__NR_ppoll]                = (shim_fp)shim_do_ppoll,
    (shim_fp)0, //shim_do_unshare,
    (shim_fp)0, //shim_do_splice,
    (shim_fp)0, //shim_do_tee,
    (shim_fp)0, //shim_do_vmsplice,			/* 285 */
    [__NR_openat]               = (shim_fp)shim_do_openat,
    [__NR_mkdirat]              = (shim_fp)shim_do_mkdirat,
    [__NR_mknodat]              = (shim_fp)shim_do_mknodat,
    [__NR_fchownat]             = (shim_fp)shim_do_fchownat,
    (shim_fp)0, //shim_do_futimesat,			/* 290 */
    [__NR_newfstatat]           = (shim_fp)shim_do_newfstatat,
    [__NR_unlinkat]             = (shim_fp)shim_do_unlinkat,
    [__NR_renameat]             = (shim_fp)shim_do_renameat,
    (shim_fp)0, //shim_do_linkat,
    (shim_fp)0, //shim_do_symlinkat,			/* 295 */
    [__NR_readlinkat]           = (shim_fp)shim_do_readlinkat,
    [__NR_fchmodat]             = (shim_fp)shim_do_fchmodat,
    [__NR_faccessat]            = (shim_fp)shim_do_faccessat,
    [__NR_get_robust_list]      = (shim_fp)shim_do_get_robust_list,
    [__NR_set_robust_list]      = (shim_fp)shim_do_set_robust_list,		/* 300 */
    (shim_fp)0, //shim_do_move_pages,
    [__NR_getcpu]               = (shim_fp)shim_do_getcpu,
    [__NR_epoll_pwait]          = (shim_fp)shim_do_epoll_pwait,
    (shim_fp)0, //shim_do_utimensat,
    (shim_fp)0, //shim_do_signalfd,		/* 305 */
    (shim_fp)0, //shim_do_timerfd_create,
    [__NR_eventfd]              = (shim_fp)shim_do_eventfd,
    (shim_fp)0, //shim_do_sync_file_range,
    (shim_fp)0, //shim_do_fallocate,
    (shim_fp)0, //shim_do_subpage_prot,		/* 310 */
    (shim_fp)0, //shim_do_timerfd_settime,
    (shim_fp)0, //shim_do_timerfd_gettime,
    (shim_fp)0, //shim_do_signalfd4,
    [__NR_eventfd2]             = (shim_fp)shim_do_eventfd2,
    [__NR_epoll_create1]        = (shim_fp)shim_do_epoll_create1,		/* 315 */
    [__NR_dup3]                 = (shim_fp)shim_do_dup3,
    [__NR_pipe2]                = (shim_fp)shim_do_pipe2,
    (shim_fp)0, //shim_do_inotify_init1,
    (shim_fp)0, //shim_do_perf_event_open,
    (shim_fp)0, //shim_do_preadv,		/* 320 */
    (shim_fp)0, //shim_do_pwritev,
    (shim_fp)0, //shim_do_rt_tgsigqueueinfo,
    (shim_fp)0, //shim_do_fanotify_init,
    (shim_fp)0, //shim_do_fanotify_mark,
    [__NR_prlimit64]            = (shim_fp)shim_do_prlimit64,			/* 325 */
    [__NR_socket]               = (shim_fp)shim_do_socket,
    [__NR_bind]                 = (shim_fp)shim_do_bind,
    [__NR_connect]              = (shim_fp)shim_do_connect,
    [__NR_listen]               = (shim_fp)shim_do_listen,
    [__NR_accept]               = (shim_fp)shim_do_accept,			/* 330 */
    [__NR_getsockname]          = (shim_fp)shim_do_getsockname,
    [__NR_getpeername]          = (shim_fp)shim_do_getpeername,
    [__NR_socketpair]           = (shim_fp)shim_do_socketpair,
    [__NR_send]                 = (shim_fp)shim_do_send,
    [__NR_sendto]               = (shim_fp)shim_do_sendto,			/* 335 */
    [__NR_recv]                 = (shim_fp)shim_do_recv,
    [__NR_recvfrom]             = (shim_fp)shim_do_recvfrom,
    [__NR_shutdown]             = (shim_fp)shim_do_shutdown,
    [__NR_setsockopt]           = (shim_fp)shim_do_setsockopt,
    [__NR_getsockopt]           = (shim_fp)shim_do_getsockopt,		/* 340 */
    [__NR_sendmsg ]             = (shim_fp)shim_do_sendmsg,
    [__NR_recvmsg ]             = (shim_fp)shim_do_recvmsg,
    [__NR_recvmmsg]             = (shim_fp)shim_do_recvmmsg,
    [__NR_accept4]              = (shim_fp)shim_do_accept4,
    (shim_fp)0, //shim_do_name_to_handle_at,	/* 345 */
    (shim_fp)0, //shim_do_open_by_handle_at,
    (shim_fp)0, //shim_do_clock_adjtime,
    (shim_fp)0, //shim_do_sysnfs,
    [__NR_sendmmsg]             = (shim_fp)shim_do_sendmmsg,
    (shim_fp)0, // shim_do_setns,		/* 350 */
    (shim_fp)0, //shim_do_process_vm_readv,
    (shim_fp)0, //shim_do_process_vm_writev,
    (shim_fp)0, //shim_do_finit_module,
    (shim_fp)0, //shim_do_kcmp,
    (shim_fp)0, //shim_do_sched_setattr		/* 355 */
    (shim_fp)0, //shim_do_sched_getattr
    (shim_fp)0, //shim_do_renameat2,
    (shim_fp)0, //shim_do_seccomp,
    [__NR_getrandom ]           = (shim_fp)shim_do_getrandom,
    (shim_fp)0, //shim_do_memfd_create		/* 360 */
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,					/* 365 */
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,					/* 370 */
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,					/* 375 */
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,					/* 380 */
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,					/* 385 */
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,					/* 390*/
    (shim_fp)0,
    (shim_fp)0,//shim_do_semtimedop,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,					/* 395*/
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,					/* 400 */
    (shim_fp)0,
    (shim_fp)0,
    [__NR_clock_gettime64]      = (shim_fp)shim_do_clock_gettime64,
    (shim_fp)0,
    (shim_fp)0,					/* 405 */
    [__NR_clock_getres_time64]  = (shim_fp)shim_do_clock_getres_time64,
    [__NR_clock_nanosleep_time64] = (shim_fp)shim_do_clock_nanosleep_time64,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,					/* 410 */
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,					/* 415 */
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,//shim_do_semtimedop_time64,		/* 420 */
    (shim_fp)0,
    [__NR_futex_time64]         = (shim_fp)shim_do_futex_time64,
    (shim_fp)0,
    (shim_fp)0,
};

noreturn void shim_table_dispatch(PAL_CONTEXT *context) {
    /* after a restart_syscall we need to adjust nip to be able to resume properly */
    context->gpregs.nip = context->gpregs.link;

    unsigned long syscallnr = pal_context_get_syscall(context);
#if 0
    log_debug("Calling Syscall %lu.  r3=0x%lx r4=0x%lx r5=0x%lx r6=0x%lx r7=0x%lx r8=0x%lx",
          syscallnr, ALL_SYSCALL_ARGS(context));
    log_debug("%s: SYSCALLNR: %ld, FUNCTION to return to: 0x%lx", __func__, syscallnr, context->gpregs.link);
#endif
    if (syscallnr == __NR_fork || syscallnr == __NR_vfork || syscallnr == __NR_clone) {
        log_debug("SYSCALLNR: %ld  SP = 0x%lx, IP = 0x%lx, R2 = 0x%lx, R12 = 0x%lx, R13 = 0x%lx",
                  syscallnr, context->gpregs.gpr[1], context->gpregs.nip,
                  context->gpregs.gpr[2], context->gpregs.gpr[12], context->gpregs.gpr[13]);

        log_debug("vrsave: 0x%lx fpscr: 0x%lx vscr: 0x%08lx%08lx",
                  context->xtregs.vrsave, context->xtregs.fpscr,
                  context->xtregs.vscr.a, context->xtregs.vscr.b);
    }

    switch (syscallnr) {
    case __NR_fork:
        log_debug("fork syscall (%ld) not supported. HALTING", syscallnr);
        while(1);
        break;
    }

    shim_emulate_syscall(context);
}

