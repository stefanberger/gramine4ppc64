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

#include "libos_internal.h"
#include "libos_table.h"
#include "libos_table_arch.h"

libos_syscall_t libos_syscall_table[LIBOS_SYSCALL_BOUND] = {
    [__NR_restart_syscall]        = (libos_syscall_t)0,
    [__NR_exit]                   = (libos_syscall_t)libos_syscall_exit,
    [__NR_fork]                   = (libos_syscall_t)libos_syscall_fork,
    [__NR_read]                   = (libos_syscall_t)libos_syscall_read,
    [__NR_write]                  = (libos_syscall_t)libos_syscall_write,
    [__NR_open]                   = (libos_syscall_t)libos_syscall_open,		/* 5 */
    [__NR_close]                  = (libos_syscall_t)libos_syscall_close,
    [__NR_waitpid]                = (libos_syscall_t)libos_syscall_waitpid,
    [__NR_creat]                  = (libos_syscall_t)libos_syscall_creat,
    [__NR_link]                   = (libos_syscall_t)0,
    [__NR_unlink]                 = (libos_syscall_t)libos_syscall_unlink,		/* 10 */
    [__NR_execve]                 = (libos_syscall_t)libos_syscall_execve,
    [__NR_chdir]                  = (libos_syscall_t)libos_syscall_chdir,
    [__NR_time]                   = (libos_syscall_t)libos_syscall_time,
    [__NR_mknod]                  = (libos_syscall_t)libos_syscall_mknod,
    [__NR_chmod]                  = (libos_syscall_t)libos_syscall_chmod,		/* 15 */
    [__NR_lchown]                 = (libos_syscall_t)0,
    (libos_syscall_t)0,
    [__NR_oldstat]                = (libos_syscall_t)libos_syscall_stat,
    [__NR_lseek]                  = (libos_syscall_t)libos_syscall_lseek,
    [__NR_getpid]                 = (libos_syscall_t)libos_syscall_getpid,		/* 20 */
    [__NR_mount]                  = (libos_syscall_t)0,
    (libos_syscall_t)0,
    [__NR_setuid]                 = (libos_syscall_t)libos_syscall_setuid,
    [__NR_getuid]                 = (libos_syscall_t)libos_syscall_getuid,
    (libos_syscall_t)0,									/* 25 */
    [__NR_ptrace]                 = (libos_syscall_t)0,
    [__NR_alarm]                  = (libos_syscall_t)libos_syscall_alarm,
    [__NR_oldfstat]               = (libos_syscall_t)libos_syscall_fstat,
    [__NR_pause]                  = (libos_syscall_t)libos_syscall_pause,
    [__NR_utime]                  = (libos_syscall_t)0,					/* 30 */
    (libos_syscall_t)0,
    (libos_syscall_t)0,
    [__NR_access]                 = (libos_syscall_t)libos_syscall_access,
    [__NR_nice]                   = (libos_syscall_t)0,
    (libos_syscall_t)0,									/* 35 */
    [__NR_sync]                   = (libos_syscall_t)0,
    [__NR_kill]                   = (libos_syscall_t)libos_syscall_kill,
    [__NR_rename]                 = (libos_syscall_t)libos_syscall_rename,
    [__NR_mkdir]                  = (libos_syscall_t)libos_syscall_mkdir,
    [__NR_rmdir]                  = (libos_syscall_t)libos_syscall_rmdir,		/* 40 */
    [__NR_dup]                    = (libos_syscall_t)libos_syscall_dup,
    [__NR_pipe]                   = (libos_syscall_t)libos_syscall_pipe,
    [__NR_times]                  = (libos_syscall_t)0,
    (libos_syscall_t)0,
    [__NR_brk]                    = (libos_syscall_t)libos_syscall_brk,			/* 45 */
    [__NR_setgid]                 = (libos_syscall_t)libos_syscall_setgid,
    [__NR_getgid]                 = (libos_syscall_t)libos_syscall_getgid,
    [__NR_signal]                 = (libos_syscall_t)0,
    [__NR_geteuid]                = (libos_syscall_t)libos_syscall_geteuid,
    [__NR_getegid]                = (libos_syscall_t)libos_syscall_getegid,		/* 50 */
    [__NR_acct]                   = (libos_syscall_t)0,
    [__NR_umount2]                = (libos_syscall_t)0,
    (libos_syscall_t)0,
    [__NR_ioctl]                  = (libos_syscall_t)libos_syscall_ioctl,
    [__NR_fcntl]                  = (libos_syscall_t)libos_syscall_fcntl,		/* 55 */
    (libos_syscall_t)0,
    [__NR_setpgid]                = (libos_syscall_t)libos_syscall_setpgid,
    (libos_syscall_t)0,
    (libos_syscall_t)0,
    [__NR_umask]                  = (libos_syscall_t)libos_syscall_umask,		/* 60 */
    [__NR_chroot]                 = (libos_syscall_t)libos_syscall_chroot,
    [__NR_ustat]                  = (libos_syscall_t)0,
    [__NR_dup2]                   = (libos_syscall_t)libos_syscall_dup2,
    [__NR_getppid]                = (libos_syscall_t)libos_syscall_getppid,
    [__NR_getpgrp]                = (libos_syscall_t)libos_syscall_getpgrp,		/* 65 */
    [__NR_setsid]                 = (libos_syscall_t)libos_syscall_setsid,
    (libos_syscall_t)0,
    (libos_syscall_t)0,
    (libos_syscall_t)0,
    [__NR_setreuid]               = (libos_syscall_t)0,					/* 70 */
    [__NR_setregid]               = (libos_syscall_t)0,
    (libos_syscall_t)0,
    [__NR_sigpending]             = (libos_syscall_t)libos_syscall_rt_sigpending,
    [__NR_sethostname]            = (libos_syscall_t)libos_syscall_sethostname,
    [__NR_setrlimit]              = (libos_syscall_t)libos_syscall_setrlimit,		/* 75 */
    [__NR_getrlimit]              = (libos_syscall_t)0,
    [__NR_getrusage]              = (libos_syscall_t)0,
    [__NR_gettimeofday]           = (libos_syscall_t)libos_syscall_gettimeofday,
    [__NR_settimeofday]           = (libos_syscall_t)0,
    [__NR_getgroups]              = (libos_syscall_t)libos_syscall_getgroups,		/* 80 */
    [__NR_setgroups]              = (libos_syscall_t)libos_syscall_setgroups,
    [__NR_select]                 = (libos_syscall_t)libos_syscall_select,
    [__NR_symlink]                = (libos_syscall_t)0,
    [__NR_oldlstat]               = (libos_syscall_t)libos_syscall_lstat,
    [__NR_readlink]               = (libos_syscall_t)libos_syscall_readlink,		/* 85 */
    [__NR_uselib]                 = (libos_syscall_t)0,
    [__NR_swapon]                 = (libos_syscall_t)0,
    [__NR_reboot]                 = (libos_syscall_t)0,
    (libos_syscall_t)0,
    [__NR_mmap]                   = (libos_syscall_t)libos_syscall_mmap,		/* 90 */
    [__NR_munmap]                 = (libos_syscall_t)libos_syscall_munmap,
    [__NR_truncate]               = (libos_syscall_t)libos_syscall_truncate,
    [__NR_ftruncate]              = (libos_syscall_t)libos_syscall_ftruncate,
    [__NR_fchmod]                 = (libos_syscall_t)libos_syscall_fchmod,
    [__NR_fchown]                 = (libos_syscall_t)libos_syscall_fchown,		/* 95 */
    [__NR_getpriority]            = (libos_syscall_t)libos_syscall_getpriority,
    [__NR_setpriority]            = (libos_syscall_t)libos_syscall_setpriority,
    (libos_syscall_t)0,
    [__NR_statfs]                 = (libos_syscall_t)libos_syscall_statfs,
    [__NR_fstatfs]                = (libos_syscall_t)libos_syscall_fstatfs,		/* 100 */
    (libos_syscall_t)0,
    [__NR_socketcall]             = (libos_syscall_t)libos_syscall_socketcall,
    [__NR_syslog]                 = (libos_syscall_t)0,
    [__NR_setitimer]              = (libos_syscall_t)libos_syscall_setitimer,
    [__NR_getitimer]              = (libos_syscall_t)libos_syscall_getitimer,		/* 105 */
    [__NR_stat]                   = (libos_syscall_t)libos_syscall_stat,
    [__NR_lstat]                  = (libos_syscall_t)libos_syscall_lstat,
    [__NR_fstat]                  = (libos_syscall_t)libos_syscall_fstat,
    [__NR_olduname]               = (libos_syscall_t)libos_syscall_uname,
    (libos_syscall_t)0,									/* 110 */
    [__NR_vhangup]                = (libos_syscall_t)0,
    (libos_syscall_t)0,
    (libos_syscall_t)0,
    [__NR_wait4]                  = (libos_syscall_t)libos_syscall_wait4,
    [__NR_swapoff]                = (libos_syscall_t)0,					/* 115 */
    [__NR_sysinfo]                = (libos_syscall_t)libos_syscall_sysinfo,
    [__NR_ipc]                    = (libos_syscall_t)0,
    [__NR_fsync]                  = (libos_syscall_t)libos_syscall_fsync,
    (libos_syscall_t)0,
    [__NR_clone]                  = (libos_syscall_t)libos_syscall_clone,		/* 120 */
    [__NR_setdomainname]          = (libos_syscall_t)libos_syscall_setdomainname,
    [__NR_uname]                  = (libos_syscall_t)libos_syscall_uname,
    (libos_syscall_t)0,
    [__NR_adjtimex]               = (libos_syscall_t)0,
    [__NR_mprotect]               = (libos_syscall_t)libos_syscall_mprotect,		/* 125 */
    (libos_syscall_t)0,
    (libos_syscall_t)0,
    [__NR_init_module]            = (libos_syscall_t)0,
    [__NR_delete_module]          = (libos_syscall_t)0,
    (libos_syscall_t)0,									/* 130 */
    [__NR_quotactl]               = (libos_syscall_t)0,
    [__NR_getpgid]                = (libos_syscall_t)libos_syscall_getpgid,
    [__NR_fchdir]                 = (libos_syscall_t)libos_syscall_fchdir,
    (libos_syscall_t)0,
    [__NR_sysfs]                  = (libos_syscall_t)0,					/* 135 */
    [__NR_personality]            = (libos_syscall_t)0,
    (libos_syscall_t)0,
    [__NR_setfsuid]               = (libos_syscall_t)0,
    [__NR_setfsgid]               = (libos_syscall_t)0,
    [__NR__llseek]                = (libos_syscall_t)libos_syscall__llseek,		/* 140 */
    [__NR_getdents]               = (libos_syscall_t)libos_syscall_getdents,
    [__NR__newselect]             = (libos_syscall_t)libos_syscall_select,
    [__NR_flock]                  = (libos_syscall_t)0,
    [__NR_msync]                  = (libos_syscall_t)libos_syscall_msync,
    [__NR_readv]                  = (libos_syscall_t)libos_syscall_readv,		/* 145 */
    [__NR_writev]                 = (libos_syscall_t)libos_syscall_writev,
    [__NR_getsid]                 = (libos_syscall_t)libos_syscall_getsid,
    [__NR_fdatasync]              = (libos_syscall_t)libos_syscall_fdatasync,
    [__NR__sysctl]                = (libos_syscall_t)0,
    [__NR_mlock]                  = (libos_syscall_t)libos_syscall_mlock,		/* 150 */
    [__NR_munlock]                = (libos_syscall_t)libos_syscall_munlock,
    [__NR_mlockall]               = (libos_syscall_t)libos_syscall_mlockall,
    [__NR_munlockall]             = (libos_syscall_t)libos_syscall_munlockall,
    [__NR_sched_setparam]         = (libos_syscall_t)libos_syscall_sched_setparam,
    [__NR_sched_getparam]         = (libos_syscall_t)libos_syscall_sched_getparam,	/* 155 */
    [__NR_sched_setscheduler]     = (libos_syscall_t)libos_syscall_sched_setscheduler,
    [__NR_sched_getscheduler]     = (libos_syscall_t)libos_syscall_sched_getscheduler,
    [__NR_sched_yield]            = (libos_syscall_t)libos_syscall_sched_yield,
    [__NR_sched_get_priority_max] = (libos_syscall_t)libos_syscall_sched_get_priority_max,
    [__NR_sched_get_priority_min] = (libos_syscall_t)libos_syscall_sched_get_priority_min,	/* 160 */
    [__NR_sched_rr_get_interval ] = (libos_syscall_t)libos_syscall_sched_rr_get_interval,
    [__NR_nanosleep]              = (libos_syscall_t)libos_syscall_nanosleep,
    [__NR_mremap]                 = (libos_syscall_t)0,
    [__NR_setresuid]              = (libos_syscall_t)0,
    [__NR_getresuid]              = (libos_syscall_t)0,					/* 165 */
    (libos_syscall_t)0,
    [__NR_poll]                   = (libos_syscall_t)libos_syscall_poll,
    (libos_syscall_t)0,
    [__NR_setresgid]              = (libos_syscall_t)0,
    [__NR_getresgid]              = (libos_syscall_t)0,					/* 170 */
    [__NR_prctl]                  = (libos_syscall_t)0,
    [__NR_rt_sigreturn]           = (libos_syscall_t)libos_syscall_rt_sigreturn,
    [__NR_rt_sigaction]           = (libos_syscall_t)libos_syscall_rt_sigaction,
    [__NR_rt_sigprocmask]         = (libos_syscall_t)libos_syscall_rt_sigprocmask,
    [__NR_rt_sigpending]          = (libos_syscall_t)libos_syscall_rt_sigpending,	/* 175 */
    [__NR_rt_sigtimedwait]        = (libos_syscall_t)libos_syscall_rt_sigtimedwait,
    [__NR_rt_sigqueueinfo]        = (libos_syscall_t)0,
    [__NR_rt_sigsuspend]          = (libos_syscall_t)libos_syscall_rt_sigsuspend,
    [__NR_pread64]                = (libos_syscall_t)libos_syscall_pread64,
    [__NR_pwrite64]               = (libos_syscall_t)libos_syscall_pwrite64,		/* 180 */
    [__NR_chown]                  = (libos_syscall_t)libos_syscall_chown,
    [__NR_getcwd]                 = (libos_syscall_t)libos_syscall_getcwd,
    [__NR_capget]                 = (libos_syscall_t)0,
    [__NR_capset]                 = (libos_syscall_t)0,
    [__NR_sigaltstack]            = (libos_syscall_t)libos_syscall_sigaltstack,		/* 185 */
    [__NR_sendfile]               = (libos_syscall_t)libos_syscall_sendfile,
    (libos_syscall_t)0,
    (libos_syscall_t)0,
    [__NR_vfork]                  = (libos_syscall_t)libos_syscall_vfork,
    [__NR_ugetrlimit]             = (libos_syscall_t)libos_syscall_getrlimit,		/* 190 */
    [__NR_readahead]              = (libos_syscall_t)0,
    (libos_syscall_t)0,
    (libos_syscall_t)0,
    (libos_syscall_t)0,
    (libos_syscall_t)0,									/* 195 */
    (libos_syscall_t)0,
    (libos_syscall_t)0,
    [__NR_pciconfig_read]         = (libos_syscall_t)0,
    [__NR_pciconfig_write]        = (libos_syscall_t)0,
    [__NR_pciconfig_iobase]       = (libos_syscall_t)0,					/* 200 */
    (libos_syscall_t)0,
    [__NR_getdents64]             = (libos_syscall_t)libos_syscall_getdents64,
    [__NR_pivot_root]             = (libos_syscall_t)0,
    (libos_syscall_t)0,
    [__NR_madvise]                = (libos_syscall_t)libos_syscall_madvise,		/* 205 */
    [__NR_mincore]                = (libos_syscall_t)libos_syscall_mincore,
    [__NR_gettid]                 = (libos_syscall_t)libos_syscall_gettid,
    [__NR_tkill]                  = (libos_syscall_t)libos_syscall_tkill,
    [__NR_setxattr]               = (libos_syscall_t)0,
    [__NR_lsetxattr]              = (libos_syscall_t)0,					/* 210 */
    [__NR_fsetxattr]              = (libos_syscall_t)0,
    [__NR_getxattr]               = (libos_syscall_t)0,
    [__NR_lgetxattr]              = (libos_syscall_t)0,
    [__NR_fgetxattr]              = (libos_syscall_t)0,
    [__NR_listxattr]              = (libos_syscall_t)0,					/* 215 */
    [__NR_llistxattr]             = (libos_syscall_t)0,
    [__NR_flistxattr]             = (libos_syscall_t)0,
    [__NR_removexattr]            = (libos_syscall_t)0,
    [__NR_lremovexattr]           = (libos_syscall_t)0,
    [__NR_fremovexattr]           = (libos_syscall_t)0,					/* 220 */
    [__NR_futex]                  = (libos_syscall_t)libos_syscall_futex,
    [__NR_sched_setaffinity]      = (libos_syscall_t)libos_syscall_sched_setaffinity,
    [__NR_sched_getaffinity]      = (libos_syscall_t)libos_syscall_sched_getaffinity,
    (libos_syscall_t)0,
    (libos_syscall_t)0,									/* 225 */
    (libos_syscall_t)0,
    (libos_syscall_t)0, // libos_syscall_io_setup,
    (libos_syscall_t)0, // libos_syscall_io_destroy,
    (libos_syscall_t)0, // libos_syscall_io_getevents,
    (libos_syscall_t)0, // libos_syscall_io_submit,					/* 230 */
    (libos_syscall_t)0, // libos_syscall_io_cancel,
    [__NR_set_tid_address]        = (libos_syscall_t)libos_syscall_set_tid_address,
    (libos_syscall_t)0, //libos_syscall_fadvise64,
    [__NR_exit_group]             = (libos_syscall_t)libos_syscall_exit_group,
    (libos_syscall_t)0, //libos_syscall_lookup_dcookie,	/* 235 */
    [__NR_epoll_create]           = (libos_syscall_t)libos_syscall_epoll_create,
    [__NR_epoll_ctl]              = (libos_syscall_t)libos_syscall_epoll_ctl,
    [__NR_epoll_wait]             = (libos_syscall_t)libos_syscall_epoll_wait,
    (libos_syscall_t)0, //libos_syscall_remap_file_pages,
    (libos_syscall_t)0, //libos_syscall_timer_create,					/* 240 */
    (libos_syscall_t)0, //libos_syscall_timer_settime,
    (libos_syscall_t)0, //libos_syscall_timer_gettime,
    (libos_syscall_t)0, //libos_syscall_timer_getoverrun,
    (libos_syscall_t)0, //libos_syscall_timer_delete,
    (libos_syscall_t)0, // libos_syscall_clock_settime,					/* 245 */
    [__NR_clock_gettime]          = (libos_syscall_t)libos_syscall_clock_gettime,
    [__NR_clock_getres]           = (libos_syscall_t)libos_syscall_clock_getres,
    [__NR_clock_nanosleep]        = (libos_syscall_t)libos_syscall_clock_nanosleep,
    (libos_syscall_t)0, // ppc64_swapcontext
    [__NR_tgkill]                 = (libos_syscall_t)libos_syscall_tgkill,		/* 250 */
    (libos_syscall_t)0, //libos_syscall_utimes,
    [__NR_statfs64]               = (libos_syscall_t)libos_syscall_statfs64,
    [__NR_fstatfs64]              = (libos_syscall_t)libos_syscall_fstatfs64,
    (libos_syscall_t)0,
    (libos_syscall_t)0, // libos_syscall_rtas						/* 255 */
    (libos_syscall_t)0,
    (libos_syscall_t)0,
    (libos_syscall_t)0, //libos_syscall_migrate_pages,
    [__NR_mbind]                  = (libos_syscall_t)libos_syscall_mbind,
    (libos_syscall_t)0, //libos_syscall_get_mempolicy,					/* 260 */
    (libos_syscall_t)0, //libos_syscall_set_mempolicy,
    (libos_syscall_t)0, //libos_syscall_mq_open,
    (libos_syscall_t)0, //libos_syscall_mq_unlink,
    (libos_syscall_t)0, //libos_syscall_mq_timedsend,
    (libos_syscall_t)0, //libos_syscall_mq_timedreceive,				/* 265 */
    (libos_syscall_t)0, //libos_syscall_mq_notify,
    (libos_syscall_t)0, //libos_syscall_mq_getsetattr,
    (libos_syscall_t)0, //libos_syscall_kexec_load,
    (libos_syscall_t)0, //libos_syscall_add_key,
    (libos_syscall_t)0, //libos_syscall_request_key,					/* 270 */
    (libos_syscall_t)0, //libos_syscall_keyctl,
    [__NR_waitid]                 = (libos_syscall_t)libos_syscall_waitid,
    (libos_syscall_t)0, //libos_syscall_ioprio_set,
    (libos_syscall_t)0, //libos_syscall_ioprio_get,
    (libos_syscall_t)0, //libos_syscall_inotify_init,					/* 275 */
    (libos_syscall_t)0, //libos_syscall_inotify_add_watch,
    (libos_syscall_t)0, //libos_syscall_inotify_rm_watch,
    (libos_syscall_t)0,
    (libos_syscall_t)0,
    [__NR_pselect6]               = (libos_syscall_t)libos_syscall_pselect6,		/* 280 */
    [__NR_ppoll]                  = (libos_syscall_t)libos_syscall_ppoll,
    (libos_syscall_t)0, //libos_syscall_unshare,
    (libos_syscall_t)0, //libos_syscall_splice,
    (libos_syscall_t)0, //libos_syscall_tee,
    (libos_syscall_t)0, //libos_syscall_vmsplice,					/* 285 */
    [__NR_openat]                 = (libos_syscall_t)libos_syscall_openat,
    [__NR_mkdirat]                = (libos_syscall_t)libos_syscall_mkdirat,
    [__NR_mknodat]                = (libos_syscall_t)libos_syscall_mknodat,
    [__NR_fchownat]               = (libos_syscall_t)libos_syscall_fchownat,
    (libos_syscall_t)0, //libos_syscall_futimesat,					/* 290 */
    [__NR_newfstatat]             = (libos_syscall_t)libos_syscall_newfstatat,
    [__NR_unlinkat]               = (libos_syscall_t)libos_syscall_unlinkat,
    [__NR_renameat]               = (libos_syscall_t)libos_syscall_renameat,
    (libos_syscall_t)0, //libos_syscall_linkat,
    (libos_syscall_t)0, //libos_syscall_symlinkat,					/* 295 */
    [__NR_readlinkat]             = (libos_syscall_t)libos_syscall_readlinkat,
    [__NR_fchmodat]               = (libos_syscall_t)libos_syscall_fchmodat,
    [__NR_faccessat]              = (libos_syscall_t)libos_syscall_faccessat,
    [__NR_get_robust_list]        = (libos_syscall_t)libos_syscall_get_robust_list,
    [__NR_set_robust_list]        = (libos_syscall_t)libos_syscall_set_robust_list,	/* 300 */
    (libos_syscall_t)0, //libos_syscall_move_pages,
    [__NR_getcpu]                 = (libos_syscall_t)libos_syscall_getcpu,
    [__NR_epoll_pwait]            = (libos_syscall_t)libos_syscall_epoll_pwait,
    (libos_syscall_t)0, //libos_syscall_utimensat,
    (libos_syscall_t)0, //libos_syscall_signalfd,					/* 305 */
    (libos_syscall_t)0, //libos_syscall_timerfd_create,
    [__NR_eventfd]                = (libos_syscall_t)libos_syscall_eventfd,
    (libos_syscall_t)0, //libos_syscall_sync_file_range,
    (libos_syscall_t)libos_syscall_fallocate,
    (libos_syscall_t)0, //libos_syscall_subpage_prot,					/* 310 */
    (libos_syscall_t)0, //libos_syscall_timerfd_settime,
    (libos_syscall_t)0, //libos_syscall_timerfd_gettime,
    (libos_syscall_t)0, //libos_syscall_signalfd4,
    [__NR_eventfd2]               = (libos_syscall_t)libos_syscall_eventfd2,
    [__NR_epoll_create1]          = (libos_syscall_t)libos_syscall_epoll_create1,	/* 315 */
    [__NR_dup3]                   = (libos_syscall_t)libos_syscall_dup3,
    [__NR_pipe2]                  = (libos_syscall_t)libos_syscall_pipe2,
    (libos_syscall_t)0, //libos_syscall_inotify_init1,
    (libos_syscall_t)0, //libos_syscall_perf_event_open,
    (libos_syscall_t)0, //libos_syscall_preadv,						/* 320 */
    (libos_syscall_t)0, //libos_syscall_pwritev,
    (libos_syscall_t)0, //libos_syscall_rt_tgsigqueueinfo,
    (libos_syscall_t)0, //libos_syscall_fanotify_init,
    (libos_syscall_t)0, //libos_syscall_fanotify_mark,
    [__NR_prlimit64]              = (libos_syscall_t)libos_syscall_prlimit64,		/* 325 */
    [__NR_socket]                 = (libos_syscall_t)libos_syscall_socket,
    [__NR_bind]                   = (libos_syscall_t)libos_syscall_bind,
    [__NR_connect]                = (libos_syscall_t)libos_syscall_connect,
    [__NR_listen]                 = (libos_syscall_t)libos_syscall_listen,
    [__NR_accept]                 = (libos_syscall_t)libos_syscall_accept,		/* 330 */
    [__NR_getsockname]            = (libos_syscall_t)libos_syscall_getsockname,
    [__NR_getpeername]            = (libos_syscall_t)libos_syscall_getpeername,
    [__NR_socketpair]             = (libos_syscall_t)libos_syscall_socketpair,
    [__NR_send]                   = (libos_syscall_t)libos_syscall_send,
    [__NR_sendto]                 = (libos_syscall_t)libos_syscall_sendto,		/* 335 */
    [__NR_recv]                   = (libos_syscall_t)libos_syscall_recv,
    [__NR_recvfrom]               = (libos_syscall_t)libos_syscall_recvfrom,
    [__NR_shutdown]               = (libos_syscall_t)libos_syscall_shutdown,
    [__NR_setsockopt]             = (libos_syscall_t)libos_syscall_setsockopt,
    [__NR_getsockopt]             = (libos_syscall_t)libos_syscall_getsockopt,		/* 340 */
    [__NR_sendmsg ]               = (libos_syscall_t)libos_syscall_sendmsg,
    [__NR_recvmsg ]               = (libos_syscall_t)libos_syscall_recvmsg,
    [__NR_recvmmsg]               = (libos_syscall_t)libos_syscall_recvmmsg,
    [__NR_accept4]                = (libos_syscall_t)libos_syscall_accept4,
    (libos_syscall_t)0, //libos_syscall_name_to_handle_at,				/* 345 */
    (libos_syscall_t)0, //libos_syscall_open_by_handle_at,
    (libos_syscall_t)0, //libos_syscall_clock_adjtime,
    (libos_syscall_t)0, //libos_syscall_sysnfs,
    [__NR_sendmmsg]               = (libos_syscall_t)libos_syscall_sendmmsg,
    (libos_syscall_t)0, // libos_syscall_setns,						/* 350 */
    (libos_syscall_t)0, //libos_syscall_process_vm_readv,
    (libos_syscall_t)0, //libos_syscall_process_vm_writev,
    (libos_syscall_t)0, //libos_syscall_finit_module,
    (libos_syscall_t)0, //libos_syscall_kcmp,
    (libos_syscall_t)0, //libos_syscall_sched_setattr					/* 355 */
    (libos_syscall_t)0, //libos_syscall_sched_getattr
    (libos_syscall_t)0, //libos_syscall_renameat2,
    (libos_syscall_t)0, //libos_syscall_seccomp,
    [__NR_getrandom ]             = (libos_syscall_t)libos_syscall_getrandom,
    (libos_syscall_t)0, //libos_syscall_memfd_create					/* 360 */
    (libos_syscall_t)0,
    (libos_syscall_t)0,
    (libos_syscall_t)0,
    (libos_syscall_t)0,
    (libos_syscall_t)0,									/* 365 */
    (libos_syscall_t)0,
    (libos_syscall_t)0,
    (libos_syscall_t)0,
    (libos_syscall_t)0,
    (libos_syscall_t)0,									/* 370 */
    (libos_syscall_t)0,
    (libos_syscall_t)0,
    (libos_syscall_t)0,
    (libos_syscall_t)0,
    (libos_syscall_t)0,									/* 375 */
    (libos_syscall_t)0,
    (libos_syscall_t)0,
    [__NR_mlock2]                 = (libos_syscall_t)libos_syscall_mlock2,
    (libos_syscall_t)0,
    (libos_syscall_t)0,									/* 380 */
    (libos_syscall_t)0,
    (libos_syscall_t)0,
    (libos_syscall_t)0,
    (libos_syscall_t)0,
    (libos_syscall_t)0,									/* 385 */
    (libos_syscall_t)0,
    (libos_syscall_t)0,
    (libos_syscall_t)0,
    (libos_syscall_t)0,
    (libos_syscall_t)0,									/* 390*/
    (libos_syscall_t)0,
    (libos_syscall_t)0,//libos_syscall_semtimedop,
    (libos_syscall_t)0,
    (libos_syscall_t)0,
    (libos_syscall_t)0,									/* 395*/
    (libos_syscall_t)0,
    (libos_syscall_t)0,
    (libos_syscall_t)0,
    (libos_syscall_t)0,
    (libos_syscall_t)0,									/* 400 */
    (libos_syscall_t)0,
    (libos_syscall_t)0,
    [__NR_clock_gettime64]        = (libos_syscall_t)libos_syscall_clock_gettime64,
    (libos_syscall_t)0,
    (libos_syscall_t)0,									/* 405 */
    [__NR_clock_getres_time64]    = (libos_syscall_t)libos_syscall_clock_getres_time64,
    [__NR_clock_nanosleep_time64] = (libos_syscall_t)libos_syscall_clock_nanosleep_time64,
    (libos_syscall_t)0,
    (libos_syscall_t)0,
    (libos_syscall_t)0,									/* 410 */
    (libos_syscall_t)0,
    (libos_syscall_t)0,
    (libos_syscall_t)0,
    (libos_syscall_t)0,
    (libos_syscall_t)0,									/* 415 */
    (libos_syscall_t)0,
    (libos_syscall_t)0,
    (libos_syscall_t)0,
    (libos_syscall_t)0,
    (libos_syscall_t)0,//libos_syscall_semtimedop_time64,				/* 420 */
    (libos_syscall_t)0,
    [__NR_futex_time64]           = (libos_syscall_t)libos_syscall_futex_time64,
    (libos_syscall_t)0,
    (libos_syscall_t)0,
};

noreturn void libos_syscall_table_dispatch(PAL_CONTEXT *context) {
    /* after a restart_syscall we need to adjust nip to be able to resume properly */
    context->gpregs.nip = context->gpregs.link;

    unsigned long syscallnr = pal_context_get_syscall(context);
#if 0
    log_debug("Calling Syscall %lu.  r3=0x%lx r4=0x%lx r5=0x%lx r6=0x%lx r7=0x%lx r8=0x%lx",
          syscallnr, ALL_SYSCALL_ARGS(context));
    log_debug("%s: SYSCALLNR: %ld, FUNCTION to return to: 0x%lx", __func__, syscallnr, context->gpregs.link);
    if (syscallnr == __NR_fork || syscallnr == __NR_vfork || syscallnr == __NR_clone) {
        log_debug("SYSCALLNR: %ld  SP = 0x%lx, IP = 0x%lx, R2 = 0x%lx, R12 = 0x%lx, R13 = 0x%lx",
                  syscallnr, context->gpregs.gpr[1], context->gpregs.nip,
                  context->gpregs.gpr[2], context->gpregs.gpr[12], context->gpregs.gpr[13]);

        log_debug("vrsave: 0x%lx fpscr: 0x%lx vscr: 0x%08lx%08lx",
                  context->xtregs.vrsave, context->xtregs.fpscr,
                  context->xtregs.vscr.a, context->xtregs.vscr.b);
    }
#endif

    switch (syscallnr) {
    case __NR_fork:
        log_debug("fork syscall (%ld) not supported. HALTING", syscallnr);
        while(1);
        break;
    }

    libos_emulate_syscall(context);
}

