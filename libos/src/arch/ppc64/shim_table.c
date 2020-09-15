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
    [__NR_restart_syscall]        = (shim_fp)0,
    [__NR_exit]                   = (shim_fp)libos_syscall_exit,
    [__NR_fork]                   = (shim_fp)libos_syscall_fork,
    [__NR_read]                   = (shim_fp)libos_syscall_read,
    [__NR_write]                  = (shim_fp)libos_syscall_write,
    [__NR_open]                   = (shim_fp)libos_syscall_open,		/* 5 */
    [__NR_close]                  = (shim_fp)libos_syscall_close,
    [__NR_waitpid]                = (shim_fp)libos_syscall_waitpid,
    [__NR_creat]                  = (shim_fp)libos_syscall_creat,
    [__NR_link]                   = (shim_fp)0,
    [__NR_unlink]                 = (shim_fp)libos_syscall_unlink,		/* 10 */
    [__NR_execve]                 = (shim_fp)libos_syscall_execve,
    [__NR_chdir]                  = (shim_fp)libos_syscall_chdir,
    [__NR_time]                   = (shim_fp)libos_syscall_time,
    [__NR_mknod]                  = (shim_fp)libos_syscall_mknod,
    [__NR_chmod]                  = (shim_fp)libos_syscall_chmod,		/* 15 */
    [__NR_lchown]                 = (shim_fp)0,
    (shim_fp)0,
    [__NR_oldstat]                = (shim_fp)libos_syscall_stat,
    [__NR_lseek]                  = (shim_fp)libos_syscall_lseek,
    [__NR_getpid]                 = (shim_fp)libos_syscall_getpid,		/* 20 */
    [__NR_mount]                  = (shim_fp)0,
    (shim_fp)0,
    [__NR_setuid]                 = (shim_fp)libos_syscall_setuid,
    [__NR_getuid]                 = (shim_fp)libos_syscall_getuid,
    (shim_fp)0,									/* 25 */
    [__NR_ptrace]                 = (shim_fp)0,
    [__NR_alarm]                  = (shim_fp)libos_syscall_alarm,
    [__NR_oldfstat]               = (shim_fp)libos_syscall_fstat,
    [__NR_pause]                  = (shim_fp)libos_syscall_pause,
    [__NR_utime]                  = (shim_fp)0,					/* 30 */
    (shim_fp)0,
    (shim_fp)0,
    [__NR_access]                 = (shim_fp)libos_syscall_access,
    [__NR_nice]                   = (shim_fp)0,
    (shim_fp)0,									/* 35 */
    [__NR_sync]                   = (shim_fp)0,
    [__NR_kill]                   = (shim_fp)libos_syscall_kill,
    [__NR_rename]                 = (shim_fp)libos_syscall_rename,
    [__NR_mkdir]                  = (shim_fp)libos_syscall_mkdir,
    [__NR_rmdir]                  = (shim_fp)libos_syscall_rmdir,		/* 40 */
    [__NR_dup]                    = (shim_fp)libos_syscall_dup,
    [__NR_pipe]                   = (shim_fp)libos_syscall_pipe,
    [__NR_times]                  = (shim_fp)0,
    (shim_fp)0,
    [__NR_brk]                    = (shim_fp)libos_syscall_brk,			/* 45 */
    [__NR_setgid]                 = (shim_fp)libos_syscall_setgid,
    [__NR_getgid]                 = (shim_fp)libos_syscall_getgid,
    [__NR_signal]                 = (shim_fp)0,
    [__NR_geteuid]                = (shim_fp)libos_syscall_geteuid,
    [__NR_getegid]                = (shim_fp)libos_syscall_getegid,		/* 50 */
    [__NR_acct]                   = (shim_fp)0,
    [__NR_umount2]                = (shim_fp)0,
    (shim_fp)0,
    [__NR_ioctl]                  = (shim_fp)libos_syscall_ioctl,
    [__NR_fcntl]                  = (shim_fp)libos_syscall_fcntl,		/* 55 */
    (shim_fp)0,
    [__NR_setpgid]                = (shim_fp)libos_syscall_setpgid,
    (shim_fp)0,
    (shim_fp)0,
    [__NR_umask]                  = (shim_fp)libos_syscall_umask,		/* 60 */
    [__NR_chroot]                 = (shim_fp)libos_syscall_chroot,
    [__NR_ustat]                  = (shim_fp)0,
    [__NR_dup2]                   = (shim_fp)libos_syscall_dup2,
    [__NR_getppid]                = (shim_fp)libos_syscall_getppid,
    [__NR_getpgrp]                = (shim_fp)libos_syscall_getpgrp,		/* 65 */
    [__NR_setsid]                 = (shim_fp)libos_syscall_setsid,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,
    [__NR_setreuid]               = (shim_fp)0,					/* 70 */
    [__NR_setregid]               = (shim_fp)0,
    (shim_fp)0,
    [__NR_sigpending]             = (shim_fp)libos_syscall_rt_sigpending,
    [__NR_sethostname]            = (shim_fp)libos_syscall_sethostname,
    [__NR_setrlimit]              = (shim_fp)libos_syscall_setrlimit,		/* 75 */
    [__NR_getrlimit]              = (shim_fp)0,
    [__NR_getrusage]              = (shim_fp)0,
    [__NR_gettimeofday]           = (shim_fp)libos_syscall_gettimeofday,
    [__NR_settimeofday]           = (shim_fp)0,
    [__NR_getgroups]              = (shim_fp)libos_syscall_getgroups,		/* 80 */
    [__NR_setgroups]              = (shim_fp)libos_syscall_setgroups,
    [__NR_select]                 = (shim_fp)libos_syscall_select,
    [__NR_symlink]                = (shim_fp)0,
    [__NR_oldlstat]               = (shim_fp)libos_syscall_lstat,
    [__NR_readlink]               = (shim_fp)libos_syscall_readlink,		/* 85 */
    [__NR_uselib]                 = (shim_fp)0,
    [__NR_swapon]                 = (shim_fp)0,
    [__NR_reboot]                 = (shim_fp)0,
    (shim_fp)0,
    [__NR_mmap]                   = (shim_fp)libos_syscall_mmap,		/* 90 */
    [__NR_munmap]                 = (shim_fp)libos_syscall_munmap,
    [__NR_truncate]               = (shim_fp)libos_syscall_truncate,
    [__NR_ftruncate]              = (shim_fp)libos_syscall_ftruncate,
    [__NR_fchmod]                 = (shim_fp)libos_syscall_fchmod,
    [__NR_fchown]                 = (shim_fp)libos_syscall_fchown,		/* 95 */
    [__NR_getpriority]            = (shim_fp)libos_syscall_getpriority,
    [__NR_setpriority]            = (shim_fp)libos_syscall_setpriority,
    (shim_fp)0,
    [__NR_statfs]                 = (shim_fp)libos_syscall_statfs,
    [__NR_fstatfs]                = (shim_fp)libos_syscall_fstatfs,		/* 100 */
    (shim_fp)0,
    [__NR_socketcall]             = (shim_fp)libos_syscall_socketcall,
    [__NR_syslog]                 = (shim_fp)0,
    [__NR_setitimer]              = (shim_fp)libos_syscall_setitimer,
    [__NR_getitimer]              = (shim_fp)libos_syscall_getitimer,		/* 105 */
    [__NR_stat]                   = (shim_fp)libos_syscall_stat,
    [__NR_lstat]                  = (shim_fp)libos_syscall_lstat,
    [__NR_fstat]                  = (shim_fp)libos_syscall_fstat,
    [__NR_olduname]               = (shim_fp)libos_syscall_uname,
    (shim_fp)0,									/* 110 */
    [__NR_vhangup]                = (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,
    [__NR_wait4]                  = (shim_fp)libos_syscall_wait4,
    [__NR_swapoff]                = (shim_fp)0,					/* 115 */
    [__NR_sysinfo]                = (shim_fp)libos_syscall_sysinfo,
    [__NR_ipc]                    = (shim_fp)0,
    [__NR_fsync]                  = (shim_fp)libos_syscall_fsync,
    (shim_fp)0,
    [__NR_clone]                  = (shim_fp)libos_syscall_clone,		/* 120 */
    [__NR_setdomainname]          = (shim_fp)libos_syscall_setdomainname,
    [__NR_uname]                  = (shim_fp)libos_syscall_uname,
    (shim_fp)0,
    [__NR_adjtimex]               = (shim_fp)0,
    [__NR_mprotect]               = (shim_fp)libos_syscall_mprotect,		/* 125 */
    (shim_fp)0,
    (shim_fp)0,
    [__NR_init_module]            = (shim_fp)0,
    [__NR_delete_module]          = (shim_fp)0,
    (shim_fp)0,									/* 130 */
    [__NR_quotactl]               = (shim_fp)0,
    [__NR_getpgid]                = (shim_fp)libos_syscall_getpgid,
    [__NR_fchdir]                 = (shim_fp)libos_syscall_fchdir,
    (shim_fp)0,
    [__NR_sysfs]                  = (shim_fp)0,					/* 135 */
    [__NR_personality]            = (shim_fp)0,
    (shim_fp)0,
    [__NR_setfsuid]               = (shim_fp)0,
    [__NR_setfsgid]               = (shim_fp)0,
    [__NR__llseek]                = (shim_fp)libos_syscall__llseek,		/* 140 */
    [__NR_getdents]               = (shim_fp)libos_syscall_getdents,
    [__NR__newselect]             = (shim_fp)libos_syscall_select,
    [__NR_flock]                  = (shim_fp)0,
    [__NR_msync]                  = (shim_fp)libos_syscall_msync,
    [__NR_readv]                  = (shim_fp)libos_syscall_readv,		/* 145 */
    [__NR_writev]                 = (shim_fp)libos_syscall_writev,
    [__NR_getsid]                 = (shim_fp)libos_syscall_getsid,
    [__NR_fdatasync]              = (shim_fp)libos_syscall_fdatasync,
    [__NR__sysctl]                = (shim_fp)0,
    [__NR_mlock]                  = (shim_fp)libos_syscall_mlock,		/* 150 */
    [__NR_munlock]                = (shim_fp)libos_syscall_munlock,
    [__NR_mlockall]               = (shim_fp)libos_syscall_mlockall,
    [__NR_munlockall]             = (shim_fp)libos_syscall_munlockall,
    [__NR_sched_setparam]         = (shim_fp)libos_syscall_sched_setparam,
    [__NR_sched_getparam]         = (shim_fp)libos_syscall_sched_getparam,	/* 155 */
    [__NR_sched_setscheduler]     = (shim_fp)libos_syscall_sched_setscheduler,
    [__NR_sched_getscheduler]     = (shim_fp)libos_syscall_sched_getscheduler,
    [__NR_sched_yield]            = (shim_fp)libos_syscall_sched_yield,
    [__NR_sched_get_priority_max] = (shim_fp)libos_syscall_sched_get_priority_max,
    [__NR_sched_get_priority_min] = (shim_fp)libos_syscall_sched_get_priority_min,	/* 160 */
    [__NR_sched_rr_get_interval ] = (shim_fp)libos_syscall_sched_rr_get_interval,
    [__NR_nanosleep]              = (shim_fp)libos_syscall_nanosleep,
    [__NR_mremap]                 = (shim_fp)0,
    [__NR_setresuid]              = (shim_fp)0,
    [__NR_getresuid]              = (shim_fp)0,					/* 165 */
    (shim_fp)0,
    [__NR_poll]                   = (shim_fp)libos_syscall_poll,
    (shim_fp)0,
    [__NR_setresgid]              = (shim_fp)0,
    [__NR_getresgid]              = (shim_fp)0,					/* 170 */
    [__NR_prctl]                  = (shim_fp)0,
    [__NR_rt_sigreturn]           = (shim_fp)libos_syscall_rt_sigreturn,
    [__NR_rt_sigaction]           = (shim_fp)libos_syscall_rt_sigaction,
    [__NR_rt_sigprocmask]         = (shim_fp)libos_syscall_rt_sigprocmask,
    [__NR_rt_sigpending]          = (shim_fp)libos_syscall_rt_sigpending,	/* 175 */
    [__NR_rt_sigtimedwait]        = (shim_fp)libos_syscall_rt_sigtimedwait,
    [__NR_rt_sigqueueinfo]        = (shim_fp)0,
    [__NR_rt_sigsuspend]          = (shim_fp)libos_syscall_rt_sigsuspend,
    [__NR_pread64]                = (shim_fp)libos_syscall_pread64,
    [__NR_pwrite64]               = (shim_fp)libos_syscall_pwrite64,		/* 180 */
    [__NR_chown]                  = (shim_fp)libos_syscall_chown,
    [__NR_getcwd]                 = (shim_fp)libos_syscall_getcwd,
    [__NR_capget]                 = (shim_fp)0,
    [__NR_capset]                 = (shim_fp)0,
    [__NR_sigaltstack]            = (shim_fp)libos_syscall_sigaltstack,		/* 185 */
    [__NR_sendfile]               = (shim_fp)libos_syscall_sendfile,
    (shim_fp)0,
    (shim_fp)0,
    [__NR_vfork]                  = (shim_fp)libos_syscall_vfork,
    [__NR_ugetrlimit]             = (shim_fp)libos_syscall_getrlimit,		/* 190 */
    [__NR_readahead]              = (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,									/* 195 */
    (shim_fp)0,
    (shim_fp)0,
    [__NR_pciconfig_read]         = (shim_fp)0,
    [__NR_pciconfig_write]        = (shim_fp)0,
    [__NR_pciconfig_iobase]       = (shim_fp)0,					/* 200 */
    (shim_fp)0,
    [__NR_getdents64]             = (shim_fp)libos_syscall_getdents64,
    [__NR_pivot_root]             = (shim_fp)0,
    (shim_fp)0,
    [__NR_madvise]                = (shim_fp)libos_syscall_madvise,		/* 205 */
    [__NR_mincore]                = (shim_fp)libos_syscall_mincore,
    [__NR_gettid]                 = (shim_fp)libos_syscall_gettid,
    [__NR_tkill]                  = (shim_fp)libos_syscall_tkill,
    [__NR_setxattr]               = (shim_fp)0,
    [__NR_lsetxattr]              = (shim_fp)0,					/* 210 */
    [__NR_fsetxattr]              = (shim_fp)0,
    [__NR_getxattr]               = (shim_fp)0,
    [__NR_lgetxattr]              = (shim_fp)0,
    [__NR_fgetxattr]              = (shim_fp)0,
    [__NR_listxattr]              = (shim_fp)0,					/* 215 */
    [__NR_llistxattr]             = (shim_fp)0,
    [__NR_flistxattr]             = (shim_fp)0,
    [__NR_removexattr]            = (shim_fp)0,
    [__NR_lremovexattr]           = (shim_fp)0,
    [__NR_fremovexattr]           = (shim_fp)0,					/* 220 */
    [__NR_futex]                  = (shim_fp)libos_syscall_futex,
    [__NR_sched_setaffinity]      = (shim_fp)libos_syscall_sched_setaffinity,
    [__NR_sched_getaffinity]      = (shim_fp)libos_syscall_sched_getaffinity,
    (shim_fp)0,
    (shim_fp)0,									/* 225 */
    (shim_fp)0,
    (shim_fp)0, // libos_syscall_io_setup,
    (shim_fp)0, // libos_syscall_io_destroy,
    (shim_fp)0, // libos_syscall_io_getevents,
    (shim_fp)0, // libos_syscall_io_submit,					/* 230 */
    (shim_fp)0, // libos_syscall_io_cancel,
    [__NR_set_tid_address]        = (shim_fp)libos_syscall_set_tid_address,
    (shim_fp)0, //libos_syscall_fadvise64,
    [__NR_exit_group]             = (shim_fp)libos_syscall_exit_group,
    (shim_fp)0, //libos_syscall_lookup_dcookie,	/* 235 */
    [__NR_epoll_create]           = (shim_fp)libos_syscall_epoll_create,
    [__NR_epoll_ctl]              = (shim_fp)libos_syscall_epoll_ctl,
    [__NR_epoll_wait]             = (shim_fp)libos_syscall_epoll_wait,
    (shim_fp)0, //libos_syscall_remap_file_pages,
    (shim_fp)0, //libos_syscall_timer_create,					/* 240 */
    (shim_fp)0, //libos_syscall_timer_settime,
    (shim_fp)0, //libos_syscall_timer_gettime,
    (shim_fp)0, //libos_syscall_timer_getoverrun,
    (shim_fp)0, //libos_syscall_timer_delete,
    (shim_fp)0, // libos_syscall_clock_settime,					/* 245 */
    [__NR_clock_gettime]          = (shim_fp)libos_syscall_clock_gettime,
    [__NR_clock_getres]           = (shim_fp)libos_syscall_clock_getres,
    [__NR_clock_nanosleep]        = (shim_fp)libos_syscall_clock_nanosleep,
    (shim_fp)0, // ppc64_swapcontext
    [__NR_tgkill]                 = (shim_fp)libos_syscall_tgkill,		/* 250 */
    (shim_fp)0, //libos_syscall_utimes,
    [__NR_statfs64]               = (shim_fp)libos_syscall_statfs64, // statfs64
    [__NR_fstatfs64]              = (shim_fp)libos_syscall_fstatfs64,// fstatfs64
    (shim_fp)0,
    (shim_fp)0, // libos_syscall_rtas						/* 255 */
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0, //libos_syscall_migrate_pages,
    [__NR_mbind]                  = (shim_fp)libos_syscall_mbind,
    (shim_fp)0, //libos_syscall_get_mempolicy,					/* 260 */
    (shim_fp)0, //libos_syscall_set_mempolicy,
    (shim_fp)0, //libos_syscall_mq_open,
    (shim_fp)0, //libos_syscall_mq_unlink,
    (shim_fp)0, //libos_syscall_mq_timedsend,
    (shim_fp)0, //libos_syscall_mq_timedreceive,				/* 265 */
    (shim_fp)0, //libos_syscall_mq_notify,
    (shim_fp)0, //libos_syscall_mq_getsetattr,
    (shim_fp)0, //libos_syscall_kexec_load,
    (shim_fp)0, //libos_syscall_add_key,
    (shim_fp)0, //libos_syscall_request_key,					/* 270 */
    (shim_fp)0, //libos_syscall_keyctl,
    [__NR_waitid]                 = (shim_fp)libos_syscall_waitid,
    (shim_fp)0, //libos_syscall_ioprio_set,
    (shim_fp)0, //libos_syscall_ioprio_get,
    (shim_fp)0, //libos_syscall_inotify_init,					/* 275 */
    (shim_fp)0, //libos_syscall_inotify_add_watch,
    (shim_fp)0, //libos_syscall_inotify_rm_watch,
    (shim_fp)0,
    (shim_fp)0,
    [__NR_pselect6]               = (shim_fp)libos_syscall_pselect6,		/* 280 */
    [__NR_ppoll]                  = (shim_fp)libos_syscall_ppoll,
    (shim_fp)0, //libos_syscall_unshare,
    (shim_fp)0, //libos_syscall_splice,
    (shim_fp)0, //libos_syscall_tee,
    (shim_fp)0, //libos_syscall_vmsplice,					/* 285 */
    [__NR_openat]                 = (shim_fp)libos_syscall_openat,
    [__NR_mkdirat]                = (shim_fp)libos_syscall_mkdirat,
    [__NR_mknodat]                = (shim_fp)libos_syscall_mknodat,
    [__NR_fchownat]               = (shim_fp)libos_syscall_fchownat,
    (shim_fp)0, //libos_syscall_futimesat,					/* 290 */
    [__NR_newfstatat]             = (shim_fp)libos_syscall_newfstatat,
    [__NR_unlinkat]               = (shim_fp)libos_syscall_unlinkat,
    [__NR_renameat]               = (shim_fp)libos_syscall_renameat,
    (shim_fp)0, //libos_syscall_linkat,
    (shim_fp)0, //libos_syscall_symlinkat,					/* 295 */
    [__NR_readlinkat]             = (shim_fp)libos_syscall_readlinkat,
    [__NR_fchmodat]               = (shim_fp)libos_syscall_fchmodat,
    [__NR_faccessat]              = (shim_fp)libos_syscall_faccessat,
    [__NR_get_robust_list]        = (shim_fp)libos_syscall_get_robust_list,
    [__NR_set_robust_list]        = (shim_fp)libos_syscall_set_robust_list,	/* 300 */
    (shim_fp)0, //libos_syscall_move_pages,
    [__NR_getcpu]                 = (shim_fp)libos_syscall_getcpu,
    [__NR_epoll_pwait]            = (shim_fp)libos_syscall_epoll_pwait,
    (shim_fp)0, //libos_syscall_utimensat,
    (shim_fp)0, //libos_syscall_signalfd,					/* 305 */
    (shim_fp)0, //libos_syscall_timerfd_create,
    [__NR_eventfd]                = (shim_fp)libos_syscall_eventfd,
    (shim_fp)0, //libos_syscall_sync_file_range,
    (shim_fp)libos_syscall_fallocate,
    (shim_fp)0, //libos_syscall_subpage_prot,					/* 310 */
    (shim_fp)0, //libos_syscall_timerfd_settime,
    (shim_fp)0, //libos_syscall_timerfd_gettime,
    (shim_fp)0, //libos_syscall_signalfd4,
    [__NR_eventfd2]               = (shim_fp)libos_syscall_eventfd2,
    [__NR_epoll_create1]          = (shim_fp)libos_syscall_epoll_create1,	/* 315 */
    [__NR_dup3]                   = (shim_fp)libos_syscall_dup3,
    [__NR_pipe2]                  = (shim_fp)libos_syscall_pipe2,
    (shim_fp)0, //libos_syscall_inotify_init1,
    (shim_fp)0, //libos_syscall_perf_event_open,
    (shim_fp)0, //libos_syscall_preadv,						/* 320 */
    (shim_fp)0, //libos_syscall_pwritev,
    (shim_fp)0, //libos_syscall_rt_tgsigqueueinfo,
    (shim_fp)0, //libos_syscall_fanotify_init,
    (shim_fp)0, //libos_syscall_fanotify_mark,
    [__NR_prlimit64]              = (shim_fp)libos_syscall_prlimit64,		/* 325 */
    [__NR_socket]                 = (shim_fp)libos_syscall_socket,
    [__NR_bind]                   = (shim_fp)libos_syscall_bind,
    [__NR_connect]                = (shim_fp)libos_syscall_connect,
    [__NR_listen]                 = (shim_fp)libos_syscall_listen,
    [__NR_accept]                 = (shim_fp)libos_syscall_accept,		/* 330 */
    [__NR_getsockname]            = (shim_fp)libos_syscall_getsockname,
    [__NR_getpeername]            = (shim_fp)libos_syscall_getpeername,
    [__NR_socketpair]             = (shim_fp)libos_syscall_socketpair,
    [__NR_send]                   = (shim_fp)libos_syscall_send,
    [__NR_sendto]                 = (shim_fp)libos_syscall_sendto,		/* 335 */
    [__NR_recv]                   = (shim_fp)libos_syscall_recv,
    [__NR_recvfrom]               = (shim_fp)libos_syscall_recvfrom,
    [__NR_shutdown]               = (shim_fp)libos_syscall_shutdown,
    [__NR_setsockopt]             = (shim_fp)libos_syscall_setsockopt,
    [__NR_getsockopt]             = (shim_fp)libos_syscall_getsockopt,		/* 340 */
    [__NR_sendmsg ]               = (shim_fp)libos_syscall_sendmsg,
    [__NR_recvmsg ]               = (shim_fp)libos_syscall_recvmsg,
    [__NR_recvmmsg]               = (shim_fp)libos_syscall_recvmmsg,
    [__NR_accept4]                = (shim_fp)libos_syscall_accept4,
    (shim_fp)0, //libos_syscall_name_to_handle_at,				/* 345 */
    (shim_fp)0, //libos_syscall_open_by_handle_at,
    (shim_fp)0, //libos_syscall_clock_adjtime,
    (shim_fp)0, //libos_syscall_sysnfs,
    [__NR_sendmmsg]               = (shim_fp)libos_syscall_sendmmsg,
    (shim_fp)0, // libos_syscall_setns,						/* 350 */
    (shim_fp)0, //libos_syscall_process_vm_readv,
    (shim_fp)0, //libos_syscall_process_vm_writev,
    (shim_fp)0, //libos_syscall_finit_module,
    (shim_fp)0, //libos_syscall_kcmp,
    (shim_fp)0, //libos_syscall_sched_setattr					/* 355 */
    (shim_fp)0, //libos_syscall_sched_getattr
    (shim_fp)0, //libos_syscall_renameat2,
    (shim_fp)0, //libos_syscall_seccomp,
    [__NR_getrandom ]             = (shim_fp)libos_syscall_getrandom,
    (shim_fp)0, //libos_syscall_memfd_create					/* 360 */
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,									/* 365 */
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,									/* 370 */
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,									/* 375 */
    (shim_fp)0,
    (shim_fp)0,
    [__NR_mlock2]                 = (shim_fp)libos_syscall_mlock2,
    (shim_fp)0,
    (shim_fp)0,									/* 380 */
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,									/* 385 */
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,									/* 390*/
    (shim_fp)0,
    (shim_fp)0,//libos_syscall_semtimedop,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,									/* 395*/
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,									/* 400 */
    (shim_fp)0,
    (shim_fp)0,
    [__NR_clock_gettime64]      = (shim_fp)libos_syscall_clock_gettime64,
    (shim_fp)0,
    (shim_fp)0,									/* 405 */
    [__NR_clock_getres_time64]  = (shim_fp)libos_syscall_clock_getres_time64,
    [__NR_clock_nanosleep_time64] = (shim_fp)libos_syscall_clock_nanosleep_time64,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,									/* 410 */
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,									/* 415 */
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,//libos_syscall_semtimedop_time64,				/* 420 */
    (shim_fp)0,
    [__NR_futex_time64]         = (shim_fp)libos_syscall_futex_time64,
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

