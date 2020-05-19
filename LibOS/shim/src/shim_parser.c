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
 * shim_parser.c
 *
 * This file contains code for parsing system call arguments for debug purpose.
 */

#include <asm/fcntl.h>
#include <asm/ioctls.h>
#include <asm/mman.h>
#include <asm/unistd.h>
#include <asm/termbits.h>
#include <errno.h>
#include <linux/fcntl.h>
#include <linux/futex.h>
#include <linux/in.h>
#include <linux/in6.h>
#include <linux/sched.h>
#include <linux/stat.h>
#include <linux/un.h>
#include <linux/wait.h>
#include <pal.h>
#include <pal_error.h>
#include <shim_internal.h>
#include <shim_table.h>
#include <shim_thread.h>
#include <shim_tcb.h>
#include <shim_utils.h>

static void parse_open_flags(va_list*);
static void parse_open_mode(va_list*);
static void parse_access_mode(va_list*);
static void parse_clone_flags(va_list*);
static void parse_mmap_prot(va_list*);
static void parse_mmap_flags(va_list*);
static void parse_exec_args(va_list*);
static void parse_exec_envp(va_list*);
static void parse_pipe_fds(va_list*);
static void parse_signum(va_list*);
static void parse_sigmask(va_list*);
static void parse_sigprocmask_how(va_list*);
static void parse_timespec(va_list*);
static void parse_sockaddr(va_list*);
static void parse_domain(va_list*);
static void parse_socktype(va_list*);
static void parse_futexop(va_list*);
static void parse_ioctlop(va_list*);
static void parse_fcntlop(va_list*);
static void parse_seek(va_list*);
static void parse_at_fdcwd(va_list*);
static void parse_wait_option(va_list*);

struct parser_table {
    int slow;
    int stop;
    void (*parser[6])(va_list*);
} syscall_parser_table[/*LIBOS_SYSCALL_BOUND*/] =
    {
        [__NR_read]  = {.slow = 1, .parser = {NULL}}, /* read */
        [__NR_write] = {.slow = 1, .parser = {NULL}}, /* write */
        [__NR_open]  = {.slow = 1,                    /* open */
         .parser = {NULL, &parse_open_flags, &parse_open_mode}},
        [__NR_close] = {.slow = 0, .parser = {NULL}},          /* close */
        [__NR_stat]  = {.slow = 0, .parser = {NULL}},          /* stat */
        [__NR_fstat] = {.slow = 0, .parser = {NULL}},          /* fstat */
        [__NR_lstat] = {.slow = 0, .parser = {NULL}},          /* lstat */
        [__NR_poll] = {.slow = 1, .parser = {NULL}},           /* poll */
        [__NR_lseek] = {.slow = 0, .parser = {NULL, NULL, &parse_seek}}, /* lseek */
#ifdef __NR__llseek
        [__NR__llseek] = {.slow = 1, .parser = {NULL}},
#endif
        [__NR_mmap] = {.slow   = 1,                                     /* mmap */
         .parser = {NULL, NULL, &parse_mmap_prot, &parse_mmap_flags}},
        [__NR_mprotect] = {.slow   = 1, /* mprotect */
         .parser = {NULL, NULL, &parse_mmap_prot}},
        [__NR_munmap] = {.slow = 1, .parser = {NULL}},         /* munmap */
        [__NR_brk] = {.slow = 0, .parser = {NULL}},            /* brk */
        [__NR_rt_sigaction] = {.slow = 0, .parser = {&parse_signum}}, /* rt_sigaction */
        [__NR_rt_sigprocmask] = {.slow   = 0,                         /* rt_sigprocmask */
         .parser = {&parse_sigprocmask_how, &parse_sigmask, &parse_sigmask}},
        [__NR_rt_sigreturn] = {.slow = 0, .parser = {NULL}},                 /* rt_sigreturn */
        [__NR_ioctl] = {.slow = 1, .parser = {NULL, &parse_ioctlop}}, /* ioctl */
        [__NR_pread64] = {.slow = 1, .parser = {NULL}},               /* pread64 */
        [__NR_pwrite64] = {.slow = 0, .parser = {NULL}},              /* pwrite64 */
        [__NR_readv] = {.slow = 1, .parser = {NULL}},                 /* readv */
        [__NR_writev] = {.slow = 0, .parser = {NULL}},                /* writev */
        [__NR_access] = {.slow   = 0,                                  /* access */
         .parser = {NULL, &parse_access_mode}},
        [__NR_pipe] = {.slow   = 0, /* pipe */
         .parser = {&parse_pipe_fds}},
        [__NR_select] = {.slow = 1, .parser = {NULL}},                           /* select */
        [__NR_sched_yield] = {.slow = 0, .parser = {NULL}},                           /* sched_yield */
        [__NR_mremap] = {.slow = 0, .parser = {NULL}},                           /* mremap */
        [__NR_msync] = {.slow = 0, .parser = {NULL}},                           /* msync */
        [__NR_mincore] = {.slow = 0, .parser = {NULL}},                           /* mincore */
        [__NR_madvise] = {.slow = 0, .parser = {NULL}},                           /* madvise */
        [__NR_shmget] = {.slow = 0, .parser = {NULL}},                           /* shmget */
        [__NR_shmat] = {.slow = 0, .parser = {NULL}},                           /* shmat */
        [__NR_shmctl] = {.slow = 0, .parser = {NULL}},                           /* shmctl */
        [__NR_dup] = {.slow = 0, .parser = {NULL}},                           /* dup */
        [__NR_dup2] = {.slow = 0, .parser = {NULL}},                           /* dup2 */
        [__NR_pause] = {.slow = 1, .parser = {NULL}},                           /* pause */
        [__NR_nanosleep] = {.slow = 1, .parser = {&parse_timespec}},                /* nanosleep */
        [__NR_getitimer] = {.slow = 0, .parser = {NULL}},                           /* getitimer */
        [__NR_alarm] = {.slow = 0, .parser = {NULL}},                           /* alarm */
        [__NR_setitimer] = {.slow = 0, .parser = {NULL}},                           /* setitimer */
        [__NR_getpid] = {.slow = 0, .parser = {NULL}},                           /* getpid */
        [__NR_sendfile] = {.slow = 0, .parser = {NULL}},                           /* sendfile */
        [__NR_socket] = {.slow = 0, .parser = {&parse_domain, &parse_socktype}}, /* socket */

        [__NR_connect] = {.slow = 1, .parser = {NULL, &parse_sockaddr}}, /* connect */
        [__NR_accept] = {.slow = 1, .parser = {NULL}},                  /* accept */
#ifdef __NR_send
        [__NR_send] = {.slow = 0, .parser = {NULL}},
#endif
        [__NR_sendto] = {.slow = 0, .parser = {NULL}},                  /* sendto */
        [__NR_recvfrom] = {.slow = 0, .parser = {NULL}},                  /* recvfrom */
#ifdef __NR_recv
        [__NR_recv] = {.slow = 0, .parser = {NULL}},
#endif
        [__NR_sendmsg] = {.slow = 0, .parser = {NULL}},                  /* sendmsg */
        [__NR_recvmsg] = {.slow = 1, .parser = {NULL}},                  /* recvmsg */
        [__NR_shutdown] = {.slow = 0, .parser = {NULL}},                  /* shutdown */
        [__NR_bind] = {.slow = 0, .parser = {NULL}},                  /* bind */
        [__NR_listen] = {.slow = 0, .parser = {NULL}},                  /* listen */
        [__NR_getsockname] = {.slow = 0, .parser = {NULL}},                  /* getsockname */
        [__NR_getpeername] = {.slow = 0, .parser = {NULL}},                  /* getpeername */
        [__NR_socketpair] = {.slow   = 0,                                   /* socketpair */
         .stop   = 3,
         .parser = {&parse_domain, &parse_socktype, NULL, &parse_pipe_fds}},
        [__NR_setsockopt] = {.slow = 0, .parser = {NULL}},               /* setsockopt */
        [__NR_getsockopt] = {.slow = 0, .parser = {NULL}},               /* getsockopt */
        [__NR_clone] = {.slow = 1, .parser = {&parse_clone_flags}}, /* clone */
        [__NR_fork] = {.slow = 1, .parser = {NULL}},               /* fork */
        [__NR_vfork] = {.slow = 1, .parser = {NULL}},               /* vfork */
        [__NR_execve] = {.slow   = 1,                                /* execve */
         .parser = {NULL, &parse_exec_args, &parse_exec_envp}},
        [__NR_exit] = {.slow = 0, .parser = {NULL}},                                 /* exit */
        [__NR_wait4] = {.slow = 1, .parser = {NULL, NULL, &parse_wait_option, NULL}}, /* wait4 */
#ifdef __NR_waitpid
        [__NR_waitpid] = {.slow = 1, .parser = {NULL}},
#endif
        [__NR_kill] = {.slow   = 0,                                                  /* kill */
         .parser = {NULL, &parse_signum, }},
        [__NR_uname] = {.slow = 0, .parser = {NULL}},                      /* uname */
        [__NR_semget] = {.slow = 0, .parser = {NULL}},                      /* semget */
#ifdef __NR_semop
        [__NR_semop] = {.slow = 1, .parser = {NULL}},                      /* semop */
#endif
        [__NR_semctl] = {.slow = 0, .parser = {NULL}},                      /* semctl */
        [__NR_shmdt] = {.slow = 0, .parser = {NULL}},                      /* shmdt */
        [__NR_msgget] = {.slow = 1, .parser = {NULL}},                      /* msgget */
        [__NR_msgsnd] = {.slow = 1, .parser = {NULL}},                      /* msgsnd */
        [__NR_msgrcv] = {.slow = 1, .parser = {NULL}},                      /* msgrcv */
        [__NR_msgctl] = {.slow = 1, .parser = {NULL}},                      /* msgctl */
        [__NR_fcntl] = {.slow = 0, .parser = {NULL, &parse_fcntlop}},      /* fcntl */
        [__NR_flock] = {.slow = 0, .parser = {NULL}},                      /* flock */
        [__NR_fsync] = {.slow = 0, .parser = {NULL}},                      /* fsync */
        [__NR_fdatasync] = {.slow = 0, .parser = {NULL}},                      /* fdatasync */
        [__NR_truncate] = {.slow = 0, .parser = {NULL}},                      /* truncate */
        [__NR_ftruncate] = {.slow = 0, .parser = {NULL}},                      /* ftruncate */
        [__NR_getdents] = {.slow = 0, .parser = {NULL}},                      /* getdents */
        [__NR_getcwd] = {.slow = 0, .parser = {NULL}},                      /* getcwd */
        [__NR_chdir] = {.slow = 0, .parser = {NULL}},                      /* chdir */
        [__NR_fchdir] = {.slow = 0, .parser = {NULL}},                      /* fchdir */
        [__NR_rename] = {.slow = 0, .parser = {NULL}},                      /* rename */
        [__NR_mkdir] = {.slow = 0, .parser = {NULL}},                      /* mkdir */
        [__NR_rmdir] = {.slow = 0, .parser = {NULL}},                      /* rmdir */
        [__NR_creat] = {.slow = 0, .parser = {NULL, &parse_open_mode}},    /* creat */
        [__NR_link] = {.slow = 0, .parser = {NULL}},                      /* link */
        [__NR_unlink] = {.slow = 0, .parser = {NULL}},                      /* unlink */
        [__NR_symlink] = {.slow = 0, .parser = {NULL}},                      /* symlink */
        [__NR_readlink] = {.slow = 0, .parser = {NULL}},                      /* readlink */
        [__NR_chmod] = {.slow = 0, .parser = {NULL}},                      /* chmod */
        [__NR_fchmod] = {.slow = 0, .parser = {NULL}},                      /* fchmod */
        [__NR_chown] = {.slow = 0, .parser = {NULL}},                      /* chown */
        [__NR_fchown] = {.slow = 0, .parser = {NULL}},                      /* fchown */
        [__NR_lchown] = {.slow = 0, .parser = {NULL}},                      /* lchown */
        [__NR_umask] = {.slow = 0, .parser = {NULL}},                      /* umask */
        [__NR_gettimeofday] = {.slow = 0, .parser = {NULL}},                      /* gettimeofday */
        [__NR_getrlimit] = {.slow = 0, .parser = {NULL}},                      /* getrlimit */
        [__NR_getrusage] = {.slow = 0, .parser = {NULL}},                      /* getrusage */
        [__NR_sysinfo] = {.slow = 0, .parser = {NULL}},                      /* sysinfo */
        [__NR_times] = {.slow = 0, .parser = {NULL}},                      /* times */
        [__NR_ptrace] = {.slow = 0, .parser = {NULL}},                      /* ptrace */
        [__NR_getuid] = {.slow = 0, .parser = {NULL}},                      /* getuid */
        [__NR_syslog] = {.slow = 0, .parser = {NULL}},                      /* syslog */
        [__NR_getgid] = {.slow = 0, .parser = {NULL}},                      /* getgid */
        [__NR_setuid] = {.slow = 0, .parser = {NULL}},                      /* setuid */
        [__NR_setgid] = {.slow = 0, .parser = {NULL}},                      /* setgid */
        [__NR_geteuid] = {.slow = 0, .parser = {NULL}},                      /* geteuid */
        [__NR_getegid] = {.slow = 0, .parser = {NULL}},                      /* getegid */
        [__NR_setpgid] = {.slow = 0, .parser = {NULL}},                      /* setpgid */
        [__NR_getppid] = {.slow = 0, .parser = {NULL}},                      /* getppid */
        [__NR_getpgrp] = {.slow = 0, .parser = {NULL}},                      /* getpgrp */
        [__NR_setsid] = {.slow = 0, .parser = {NULL}},                      /* setsid */
        [__NR_setreuid] = {.slow = 0, .parser = {NULL}},                      /* setreuid */
        [__NR_setregid] = {.slow = 0, .parser = {NULL}},                      /* setregid */
        [__NR_getgroups] = {.slow = 0, .parser = {NULL}},                      /* getgroups */
        [__NR_setgroups] = {.slow = 0, .parser = {NULL}},                      /* setgroups */
        [__NR_setresuid] = {.slow = 0, .parser = {NULL}},                      /* setresuid */
        [__NR_getresuid] = {.slow = 0, .parser = {NULL}},                      /* getresuid */
        [__NR_setresgid] = {.slow = 0, .parser = {NULL}},                      /* setresgid */
        [__NR_getresgid] = {.slow = 0, .parser = {NULL}},                      /* getresgid */
        [__NR_getpgid] = {.slow = 0, .parser = {NULL}},                      /* getpgid */
        [__NR_setfsuid] = {.slow = 0, .parser = {NULL}},                      /* setfsuid */
        [__NR_setfsgid] = {.slow = 0, .parser = {NULL}},                      /* setfsgid */
        [__NR_getsid] = {.slow = 0, .parser = {NULL}},                      /* getsid */
        [__NR_capget] = {.slow = 0, .parser = {NULL}},                      /* capget */
        [__NR_capset] = {.slow = 0, .parser = {NULL}},                      /* capset */
        [__NR_rt_sigpending] = {.slow = 0, .parser = {NULL}},                      /* rt_sigpending */
        [__NR_rt_sigtimedwait] = {.slow = 0, .parser = {NULL}},                      /* rt_sigtimedwait */
        [__NR_rt_sigqueueinfo] = {.slow = 0, .parser = {NULL}},                      /* rt_sigqueueinfo */
        [__NR_rt_sigsuspend] = {.slow = 1, .parser = {NULL}},                      /* rt_sigsuspend */
        [__NR_sigaltstack] = {.slow = 0, .parser = {NULL}},                      /* sigaltstack */
        [__NR_utime] = {.slow = 0, .parser = {NULL}},                      /* utime */
        [__NR_mknod] = {.slow = 0, .parser = {NULL, &parse_open_mode}},                      /* mknod */
        [__NR_uselib] = {.slow = 0, .parser = {NULL}},                      /* uselib */
        [__NR_personality] = {.slow = 0, .parser = {NULL}},                      /* personality */
        [__NR_ustat] = {.slow = 0, .parser = {NULL}},                      /* ustat */
        [__NR_statfs] = {.slow = 0, .parser = {NULL}},                      /* statfs */
        [__NR_fstatfs] = {.slow = 0, .parser = {NULL}},                      /* fstatfs */
        [__NR_sysfs] = {.slow = 0, .parser = {NULL}},                      /* sysfs */
        [__NR_getpriority] = {.slow = 0, .parser = {NULL}},                      /* getpriority */
        [__NR_setpriority] = {.slow = 0, .parser = {NULL}},                      /* setpriority */
        [__NR_sched_setparam] = {.slow = 0, .parser = {NULL}},                      /* sched_setparam */
        [__NR_sched_getparam] = {.slow = 0, .parser = {NULL}},                      /* sched_getparam */
        [__NR_sched_setscheduler] = {.slow = 0, .parser = {NULL}},                      /* sched_setscheduler */
        [__NR_sched_getscheduler] = {.slow = 0, .parser = {NULL}},                      /* sched_getscheduler */
        [__NR_sched_get_priority_max] = {.slow = 0, .parser = {NULL}},                      /* sched_get_priority_max */
        [__NR_sched_get_priority_min] = {.slow = 0, .parser = {NULL}},                      /* sched_get_priority_min */
        [__NR_sched_rr_get_interval] = {.slow = 0, .parser = {NULL}},                      /* sched_rr_get_interval */
        [__NR_mlock] = {.slow = 0, .parser = {NULL}},                      /* mlock */
        [__NR_munlock] = {.slow = 0, .parser = {NULL}},                      /* munlock */
        [__NR_mlockall] = {.slow = 0, .parser = {NULL}},                      /* mlockall */
        [__NR_munlockall] = {.slow = 0, .parser = {NULL}},                      /* munlockall */
        [__NR_vhangup] = {.slow = 0, .parser = {NULL}},                      /* vhangup */
#ifdef __NR_modify_ldt
        [__NR_modify_ldt] = {.slow = 0, .parser = {NULL}},                      /* modify_ldt */
#endif
        [__NR_pivot_root] = {.slow = 0, .parser = {NULL}},                      /* pivot_root */
        [__NR__sysctl] = {.slow = 0, .parser = {NULL}},                      /* _sysctl */
        [__NR_prctl] = {.slow = 0, .parser = {NULL}},                      /* prctl */
#ifdef __NR_arch_prctl
        [__NR_arch_prctl] = {.slow = 0, .parser = {NULL}},                      /* arch_prctl */
#endif
        [__NR_adjtimex] = {.slow = 0, .parser = {NULL}},                      /* adjtimex */
        [__NR_setrlimit] = {.slow = 0, .parser = {NULL}},                      /* setrlimit */
        [__NR_chroot] = {.slow = 0, .parser = {NULL}},                      /* chroot */
        [__NR_sync] = {.slow = 0, .parser = {NULL}},                      /* sync */
        [__NR_acct] = {.slow = 0, .parser = {NULL}},                      /* acct */
        [__NR_settimeofday] = {.slow = 0, .parser = {NULL}},                      /* settimeofday */
        [__NR_mount] = {.slow = 0, .parser = {NULL}},                      /* mount */
        [__NR_umount2] = {.slow = 0, .parser = {NULL}},                      /* umount2 */
        [__NR_swapon] = {.slow = 0, .parser = {NULL}},                      /* swapon */
        [__NR_swapoff] = {.slow = 0, .parser = {NULL}},                      /* swapoff */
        [__NR_reboot] = {.slow = 0, .parser = {NULL}},                      /* reboot */
        [__NR_sethostname] = {.slow = 0, .parser = {NULL}},                      /* sethostname */
        [__NR_setdomainname] = {.slow = 0, .parser = {NULL}},                      /* setdomainname */
        [__NR_iopl] = {.slow = 0, .parser = {NULL}},                      /* iopl */
        [__NR_ioperm] = {.slow = 0, .parser = {NULL}},                      /* ioperm */
        [__NR_create_module] = {.slow = 0, .parser = {NULL}},                      /* create_module */
        [__NR_init_module] = {.slow = 0, .parser = {NULL}},                      /* init_module */
        [__NR_delete_module] = {.slow = 0, .parser = {NULL}},                      /* delete_module */
        [__NR_get_kernel_syms] = {.slow = 0, .parser = {NULL}},                      /* get_kernel_syms */
        [__NR_query_module] = {.slow = 0, .parser = {NULL}},                      /* query_module */
        [__NR_quotactl] = {.slow = 0, .parser = {NULL}},                      /* quotactl */
        [__NR_nfsservctl] = {.slow = 0, .parser = {NULL}},                      /* nfsservctl */
        [__NR_getpmsg] = {.slow = 0, .parser = {NULL}},                      /* getpmsg */
        [__NR_putpmsg] = {.slow = 0, .parser = {NULL}},                      /* putpmsg */
        [__NR_afs_syscall] = {.slow = 0, .parser = {NULL}},                      /* afs_syscall */
        [__NR_tuxcall] = {.slow = 0, .parser = {NULL}},                      /* tuxcall */
#ifdef __NR_security
        [__NR_security] = {.slow = 0, .parser = {NULL}},                      /* security */
#endif
        [__NR_gettid] = {.slow = 0, .parser = {NULL}},                      /* gettid */
        [__NR_readahead] = {.slow = 0, .parser = {NULL}},                      /* readahead */
        [__NR_setxattr] = {.slow = 0, .parser = {NULL}},                      /* setxattr */
        [__NR_lsetxattr] = {.slow = 0, .parser = {NULL}},                      /* lsetxattr */
        [__NR_fsetxattr] = {.slow = 0, .parser = {NULL}},                      /* fsetxattr */
        [__NR_getxattr] = {.slow = 0, .parser = {NULL}},                      /* getxattr */
        [__NR_lgetxattr] = {.slow = 0, .parser = {NULL}},                      /* lgetxattr */
        [__NR_fgetxattr] = {.slow = 0, .parser = {NULL}},                      /* fgetxattr */
        [__NR_listxattr] = {.slow = 0, .parser = {NULL}},                      /* listxattr */
        [__NR_llistxattr] = {.slow = 0, .parser = {NULL}},                      /* llistxattr */
        [__NR_flistxattr] = {.slow = 0, .parser = {NULL}},                      /* flistxattr */
        [__NR_removexattr] = {.slow = 0, .parser = {NULL}},                      /* removexattr */
        [__NR_lremovexattr] = {.slow = 0, .parser = {NULL}},                      /* lremovexattr */
        [__NR_fremovexattr] = {.slow = 0, .parser = {NULL}},                      /* fremovexattr */
        [__NR_tkill] = {.slow = 0, .parser = {NULL, &parse_signum}},       /* tkill */
        [__NR_time] = {.slow = 0, .parser = {NULL}},                      /* time */
        [__NR_futex] = {.slow = 1, .parser = {NULL, &parse_futexop}},      /* futex */
        [__NR_sched_setaffinity] = {.slow = 0, .parser = {NULL}},                      /* sched_setaffinity */
        [__NR_sched_getaffinity] = {.slow = 0, .parser = {NULL}},                      /* sched_getaffinity */
#ifdef __NR_set_thread_area
        [__NR_set_thread_area] = {.slow = 0, .parser = {NULL}},                      /* set_thread_area */
#endif
        [__NR_io_setup] = {.slow = 0, .parser = {NULL}},                      /* io_setup */
        [__NR_io_destroy] = {.slow = 0, .parser = {NULL}},                      /* io_destroy */
        [__NR_io_getevents] = {.slow = 0, .parser = {NULL}},                      /* io_getevents */
        [__NR_io_submit] = {.slow = 0, .parser = {NULL}},                      /* io_submit */
        [__NR_io_cancel] = {.slow = 0, .parser = {NULL}},                      /* io_cancel */
#ifdef __NR_get_thread_area
        [__NR_get_thread_area] = {.slow = 0, .parser = {NULL}},                      /* get_thread_area */
#endif
        [__NR_lookup_dcookie] = {.slow = 0, .parser = {NULL}},                      /* lookup_dcookie */
        [__NR_epoll_create] = {.slow = 0, .parser = {NULL}},                      /* epoll_create */
#ifdef __NR_epoll_ctl_old
        [__NR_epoll_ctl_old] = {.slow = 0, .parser = {NULL}},                      /* epoll_ctl_old */
#endif
#ifdef __NR_epoll_wait_old
        [__NR_epoll_wait_old] = {.slow = 0, .parser = {NULL}},                      /* epoll_wait_old */
#endif
        [__NR_remap_file_pages] = {.slow = 0, .parser = {NULL}},                      /* remap_file_pages */
        [__NR_getdents64] = {.slow = 0, .parser = {NULL}},                      /* getdents64 */
        [__NR_set_tid_address] = {.slow = 0, .parser = {NULL}},                      /* set_tid_address */
        [__NR_restart_syscall] = {.slow = 0, .parser = {NULL}},                      /* restart_syscall */
        [__NR_semtimedop] = {.slow = 0, .parser = {NULL}},                      /* semtimedop */
        [__NR_fadvise64] = {.slow = 0, .parser = {NULL}},                      /* fadvise64 */
        [__NR_timer_create] = {.slow = 0, .parser = {NULL}},                      /* timer_create */
        [__NR_timer_settime] = {.slow = 0, .parser = {NULL}},                      /* timer_settime */
        [__NR_timer_gettime] = {.slow = 0, .parser = {NULL}},                      /* timer_gettime */
        [__NR_timer_getoverrun] = {.slow = 0, .parser = {NULL}},                      /* timer_getoverrun */
        [__NR_timer_delete] = {.slow = 0, .parser = {NULL}},                      /* timer_delete */
        [__NR_clock_settime] = {.slow = 0, .parser = {NULL}},                      /* clock_settime */
        [__NR_clock_gettime] = {.slow = 0, .parser = {NULL}},                      /* clock_gettime */
        [__NR_clock_getres] = {.slow = 0, .parser = {NULL}},                      /* clock_getres */
        [__NR_clock_nanosleep] = {.slow = 0, .parser = {NULL}},                      /* clock_nanosleep */
        [__NR_exit_group] = {.slow = 0, .parser = {NULL}},                      /* exit_group */
        [__NR_epoll_wait] = {.slow = 1, .parser = {NULL}},                      /* epoll_wait */
        [__NR_epoll_ctl] = {.slow = 0, .parser = {NULL}},                      /* epoll_ctl */
        [__NR_tgkill] = {.slow = 0, .parser = {NULL, NULL, &parse_signum}}, /* tgkill */
        [__NR_utimes] = {.slow = 0, .parser = {NULL}},                      /* utimes */
#ifdef __NR_vserver
        [__NR_vserver] = {.slow = 0, .parser = {NULL}},                      /* vserver */
#endif
        [__NR_mbind] = {.slow = 0, .parser = {NULL}},                      /* mbind */
        [__NR_set_mempolicy] = {.slow = 0, .parser = {NULL}},                      /* set_mempolicy */
        [__NR_get_mempolicy] = {.slow = 0, .parser = {NULL}},                      /* get_mempolicy */
        [__NR_mq_open] = {.slow = 0, .parser = {NULL}},                      /* mq_open */
        [__NR_mq_unlink] = {.slow = 0, .parser = {NULL}},                      /* mq_unlink */
        [__NR_mq_timedsend] = {.slow = 0, .parser = {NULL}},                      /* mq_timedsend */
        [__NR_mq_timedreceive] = {.slow = 0, .parser = {NULL}},                      /* mq_timedreceive */
        [__NR_mq_notify] = {.slow = 0, .parser = {NULL}},                      /* mq_notify */
        [__NR_mq_getsetattr] = {.slow = 0, .parser = {NULL}},                      /* mq_getsetattr */
        [__NR_kexec_load] = {.slow = 0, .parser = {NULL}},                      /* kexec_load */
        [__NR_waitid] = {.slow = 1, .parser = {NULL}},                      /* waitid */
        [__NR_add_key] = {.slow = 0, .parser = {NULL}},                      /* add_key */
        [__NR_request_key] = {.slow = 0, .parser = {NULL}},                      /* request_key */
        [__NR_keyctl] = {.slow = 0, .parser = {NULL}},                      /* keyctl */
        [__NR_ioprio_set] = {.slow = 0, .parser = {NULL}},                      /* ioprio_set */
        [__NR_ioprio_get] = {.slow = 0, .parser = {NULL}},                      /* ioprio_get */
        [__NR_inotify_init] = {.slow = 0, .parser = {NULL}},                      /* inotify_init */
        [__NR_inotify_add_watch] = {.slow = 0, .parser = {NULL}},                      /* inotify_add_watch */
        [__NR_inotify_rm_watch] = {.slow = 0, .parser = {NULL}},                      /* inotify_rm_watch */
        [__NR_migrate_pages] = {.slow = 0, .parser = {NULL}},                      /* migrate_pages */
        [__NR_openat] = {.slow   = 0,
         .parser = {&parse_at_fdcwd, NULL, &parse_open_flags, &parse_open_mode}}, /* openat */
        [__NR_mkdirat] = {.slow = 0, .parser = {&parse_at_fdcwd}}, /* mkdirat */
        [__NR_mknodat] = {.slow = 0, .parser = {&parse_at_fdcwd, NULL, &parse_open_mode}}, /* mknodat */
        [__NR_fchownat] = {.slow = 0, .parser = {&parse_at_fdcwd}}, /* fchownat */
        [__NR_futimesat] = {.slow = 0, .parser = {&parse_at_fdcwd}}, /* futimesat */
        [__NR_newfstatat] = {.slow = 0, .parser = {&parse_at_fdcwd}}, /* newfstatat */
        [__NR_unlinkat] = {.slow = 0, .parser = {&parse_at_fdcwd}}, /* unlinkat */
        [__NR_renameat] = {.slow = 0, .parser = {&parse_at_fdcwd}}, /* renameat */
        [__NR_linkat] = {.slow = 0, .parser = {&parse_at_fdcwd}}, /* linkat */
        [__NR_symlinkat] = {.slow = 0, .parser = {&parse_at_fdcwd}}, /* symlinkat */
        [__NR_readlinkat] = {.slow = 0, .parser = {&parse_at_fdcwd}}, /* readlinkat */
        [__NR_fchmodat] = {.slow = 0, .parser = {&parse_at_fdcwd}}, /* fchmodat */
        [__NR_faccessat] = {.slow = 0, .parser = {&parse_at_fdcwd}}, /* faccessat */
        [__NR_pselect6] = {.slow = 1, .parser = {NULL}}, /* pselect6 */
        [__NR_ppoll] = {.slow = 1, .parser = {NULL}}, /* ppoll */
        [__NR_unshare] = {.slow = 0, .parser = {NULL}}, /* unshare */
        [__NR_set_robust_list] = {.slow = 0, .parser = {NULL}}, /* set_robust_list */
        [__NR_get_robust_list] = {.slow = 0, .parser = {NULL}}, /* get_robust_list */
        [__NR_splice] = {.slow = 0, .parser = {NULL}}, /* splice */
        [__NR_tee] = {.slow = 0, .parser = {NULL}}, /* tee */
#ifdef __NR_sync_file_range
        [__NR_sync_file_range] = {.slow = 0, .parser = {NULL}}, /* sync_file_range */
#endif
        [__NR_vmsplice] = {.slow = 0, .parser = {NULL}}, /* vmsplice */
        [__NR_move_pages] = {.slow = 0, .parser = {NULL}}, /* move_pages */
        [__NR_utimensat] = {.slow = 0, .parser = {NULL}}, /* utimensat */
        [__NR_epoll_pwait] = {.slow = 1, .parser = {NULL}}, /* epoll_pwait */
        [__NR_signalfd] = {.slow = 0, .parser = {NULL}}, /* signalfd */
        [__NR_timerfd_create] = {.slow = 0, .parser = {NULL}}, /* timerfd_create */
        [__NR_eventfd] = {.slow = 0, .parser = {NULL}}, /* eventfd */
        [__NR_fallocate] = {.slow = 0, .parser = {NULL}}, /* fallocate */
        [__NR_timerfd_settime] = {.slow = 0, .parser = {NULL}}, /* timerfd_settime */
        [__NR_timerfd_gettime] = {.slow = 0, .parser = {NULL}}, /* timerfd_gettime */
        [__NR_accept4] = {.slow = 1, .parser = {NULL}}, /* accept4 */
        [__NR_signalfd4] = {.slow = 0, .parser = {NULL}}, /* signalfd4 */
        [__NR_eventfd2] = {.slow = 0, .parser = {NULL}}, /* eventfd2 */
        [__NR_epoll_create1] = {.slow = 0, .parser = {NULL}}, /* epoll_create1 */
        [__NR_dup3] = {.slow = 0, .parser = {NULL}}, /* dup3 */
        [__NR_pipe2] = {.slow = 0, .parser = {NULL}}, /* pipe2 */
        [__NR_inotify_init1] = {.slow = 0, .parser = {NULL}}, /* inotify_init1 */
        [__NR_preadv] = {.slow = 0, .parser = {NULL}}, /* preadv */
        [__NR_pwritev] = {.slow = 0, .parser = {NULL}}, /* pwritev */
        [__NR_rt_tgsigqueueinfo] = {.slow = 0, .parser = {NULL}}, /* rt_tgsigqueueinfo */
        [__NR_perf_event_open] = {.slow = 0, .parser = {NULL}}, /* perf_event_open */
        [__NR_recvmmsg] = {.slow = 0, .parser = {NULL}}, /* recvmmsg */
#ifdef __NR_getrandom
        [__NR_getrandom] = {.slow = 0, .parser = {NULL}}, /* getrandom */
#endif

#ifndef __powerpc64__
        [LIBOS_SYSCALL_BASE] = {.slow = 0, .parser = {NULL}},

        {.slow = 1, .parser = {NULL}}, /* checkpoint */
        {.slow = 1, .parser = {NULL}}, /* restore */
        {.slow = 1, .parser = {NULL}}, /* msgpersist */
        {.slow = 1, .parser = {NULL}}, /* benchmark_ipc */
        {.slow = 1, .parser = {NULL}}, /* send_rpc */
        {.slow = 1, .parser = {NULL}}, /* recv_rpc */
#endif
};

static inline int is_pointer(const char* type) {
    return type[strlen(type) - 1] == '*' || !strcmp_static(type, "long") ||
           !strcmp_static(type, "unsigned long");
}

#define PRINTF(fmt, ...)                \
    do {                                \
        debug_printf(fmt, __VA_ARGS__); \
    } while (0)
#define PUTS(str)        \
    do {                 \
        debug_puts(str); \
    } while (0)
#define PUTCH(ch)        \
    do {                 \
        debug_putch(ch); \
    } while (0)
#define VPRINTF(fmt, ap)        \
    do {                        \
        debug_vprintf(fmt, ap); \
    } while (0)

static inline void parse_string_arg(va_list* ap) {
    va_list ap_test_arg;
    va_copy(ap_test_arg,*ap);
    const char* test_arg = va_arg(ap_test_arg, const char*);
    if (!test_user_string(test_arg)) {
        PRINTF("\"%s\"", va_arg(*ap, const char *));
    } else {
        /* invalid memory region, print arg as ptr not string */
        PRINTF("\"(invalid-addr %p)\"", va_arg(*ap, const char *));
    }
    va_end(ap_test_arg);
}

static inline void parse_pointer_arg(va_list* ap) {
    PRINTF("%p", va_arg(*ap, void *));
}

static inline void parse_integer_arg(va_list* ap) {
    PRINTF("%d", va_arg(*ap, int));
}

static inline void parse_syscall_args(va_list* ap) {
    const char* arg_type = va_arg(*ap, const char*);

    if (!strcmp_static(arg_type, "const char *") || !strcmp_static(arg_type, "const char*"))
        parse_string_arg(ap);
    else if (is_pointer(arg_type))
        parse_pointer_arg(ap);
    else
        parse_integer_arg(ap);
}

static inline void skip_syscall_args(va_list* ap) {
    const char* arg_type = va_arg(*ap, const char*);

    if (!strcmp_static(arg_type, "const char *") || !strcmp_static(arg_type, "const char*"))
        va_arg(*ap, const char*);
    else if (is_pointer(arg_type))
        va_arg(*ap, void*);
    else
        va_arg(*ap, int);
}

void sysparser_printf(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    VPRINTF(fmt, ap);
    va_end(ap);
}

void parse_syscall_before(int sysno, const char* name, int nr, ...) {
    if (!debug_handle)
        return;

    struct parser_table* parser = &syscall_parser_table[sysno];

    if (!parser->slow && !parser->stop)
        return;

    va_list ap;
    va_start(ap, nr);

    PRINTF("---- shim_%s(", name);

    for (int i = 0; i < nr; i++) {
        if (parser->stop && parser->stop == i)
            goto dotdotdot;

        if (i)
            PUTCH(',');

        if (parser->parser[i]) {
            const char* type = va_arg(ap, const char*);
            __UNUSED(type);  // type not needed on this path
            (*parser->parser[i])(&ap);
        } else {
            parse_syscall_args(&ap);
        }
    }

    PUTCH(')');
dotdotdot:
    PRINTF(" ... %s\n", name);
    va_end(ap);
}

void parse_syscall_after(int sysno, const char* name, int nr, ...) {
    if (!debug_handle)
        return;

    struct parser_table* parser = &syscall_parser_table[sysno];

    va_list ap;
    va_start(ap, nr);

    const char* ret_type = va_arg(ap, const char*);

    if (parser->slow || parser->stop)
        PRINTF("---- return from shim_%s(...", name);
    else
        PRINTF("---- shim_%s(", name);

    unsigned long ret_ptr = 0;
    int ret_val = 0;

    if (is_pointer(ret_type))
        ret_ptr = (unsigned long)va_arg(ap, void*);
    else
        ret_val = va_arg(ap, int);

    if (!parser->slow || parser->stop)
        for (int i = 0; i < nr; i++) {
            if (parser->stop && i < parser->stop) {
                skip_syscall_args(&ap);
                continue;
            }

            if (i)
                PUTCH(',');

            if (parser->parser[i]) {
                const char* type = va_arg(ap, const char*);
                __UNUSED(type);  // type not needed on this path
                (*parser->parser[i])(&ap);
            } else {
                parse_syscall_args(&ap);
            }
        }

    if (is_pointer(ret_type)) {
        if ((uint64_t)ret_ptr < (uint64_t)-4095L)
            PRINTF(") = 0x%08lx\n", ret_ptr);
        else
            PRINTF(") = %ld\n", (long)ret_ptr);
    } else {
        PRINTF(") = %d\n", ret_val);
    }

    va_end(ap);
}

static void parse_open_flags(va_list* ap) {
    int flags = va_arg(*ap, int);

    if (flags & O_WRONLY) {
        PUTS("O_WRONLY");
        flags &= ~O_WRONLY;
    } else if (flags & O_RDWR) {
        PUTS("O_RDWR");
        flags &= ~O_RDWR;
    } else {
        PUTS("O_RDONLY");
    }

    if (flags & O_APPEND) {
        PUTS("|O_APPEND");
        flags &= ~O_APPEND;
    }
    if (flags & O_CREAT) {
        PUTS("|O_CREAT");
        flags &= ~O_CREAT;
    }
    if (flags & O_TRUNC) {
        PUTS("|O_TRUNC");
        flags &= ~O_TRUNC;
    }
    if (flags & O_EXCL) {
        PUTS("|O_EXCL");
        flags &= ~O_EXCL;
    }

    if (flags)
        PRINTF("|%o", flags);
}

static void parse_open_mode(va_list* ap) {
    PRINTF("%04o", va_arg(*ap, mode_t));
}

static void parse_access_mode(va_list* ap) {
    int mode = va_arg(*ap, int);

    PUTS("F_OK");

    if (mode) {
        if (mode & R_OK)
            PUTS("|R_OK");
        if (mode & W_OK)
            PUTS("|W_OK");
        if (mode & X_OK)
            PUTS("|X_OK");
    }
}

static void parse_clone_flags(va_list* ap) {
    int flags = va_arg(*ap, int);

#define FLG(n) \
    { "CLONE_" #n, CLONE_##n, }
    const struct {
        const char* name;
        int flag;
    } all_flags[] = {
        FLG(VM),
        FLG(FS),
        FLG(FILES),
        FLG(SIGHAND),
        FLG(PTRACE),
        FLG(VFORK),
        FLG(PARENT),
        FLG(THREAD),
        FLG(NEWNS),
        FLG(SYSVSEM),
        FLG(SETTLS),
        FLG(PARENT_SETTID),
        FLG(CHILD_CLEARTID),
        FLG(DETACHED),
        FLG(UNTRACED),
        FLG(CHILD_SETTID),
        FLG(NEWUTS),
        FLG(NEWIPC),
        FLG(NEWUSER),
        FLG(NEWPID),
        FLG(NEWNET),
        FLG(IO),
    };
#undef FLG

    bool printed = false;
    for (size_t i = 0; i < ARRAY_SIZE(all_flags); i++)
        if (flags & all_flags[i].flag) {
            if (printed)
                PUTCH('|');
            else
                printed = true;
            PUTS(all_flags[i].name);
            flags &= ~all_flags[i].flag;
        }

#define CLONE_SIGNAL_MASK 0xff
    int exit_signal = flags & CLONE_SIGNAL_MASK;
    flags &= ~CLONE_SIGNAL_MASK;
    if (exit_signal) {
        if (exit_signal >= 0 && exit_signal <= NUM_KNOWN_SIGS)
            PRINTF("|%s", signal_name(exit_signal));
        else
            PRINTF("|[SIG %d]", exit_signal);
    }

    if (flags)
        PRINTF("|0x%x", flags);
}

static void parse_mmap_prot(va_list* ap) {
    int prot   = va_arg(*ap, int);
    int nflags = 0;

    if (prot == PROT_NONE) {
        PUTS("PROT_NONE");
        return;
    }

    if (prot & PROT_READ) {
        if (nflags++)
            PUTS("|");
        PUTS("PROT_READ");
    }

    if (prot & PROT_WRITE) {
        if (nflags++)
            PUTS("|");
        PUTS("PROT_WRITE");
    }

    if (prot & PROT_EXEC) {
        if (nflags++)
            PUTS("|");

        PUTS("PROT_EXEC");
    }
}

static void parse_mmap_flags(va_list* ap) {
    int flags = va_arg(*ap, int);

    if (flags & MAP_SHARED) {
        PUTS("MAP_SHARED");
        flags &= ~MAP_SHARED;
    }

    if (flags & MAP_PRIVATE) {
        PUTS("MAP_PRIVATE");
        flags &= ~MAP_PRIVATE;
    }

    if (flags & MAP_ANONYMOUS) {
        PUTS("|MAP_ANON");
        flags &= ~MAP_ANONYMOUS;
    }

    if (flags & MAP_FILE) {
        PUTS("|MAP_FILE");
        flags &= ~MAP_FILE;
    }

    if (flags & MAP_FIXED) {
        PUTS("|MAP_FIXED");
        flags &= ~MAP_FIXED;
    }

#ifdef CONFIG_MMAP_ALLOW_UNINITIALIZED
    if (flags & MAP_UNINITIALIZED) {
        PUTS("|MAP_UNINITIALIZED");
        flags &= ~MAP_UNINITIALIZED;
    }
#endif

    if (flags)
        PRINTF("|0x%x", flags);
}

static void parse_exec_args(va_list* ap) {
    const char** args = va_arg(*ap, const char**);

    PUTS("[");

    for (;; args++) {
        if (test_user_memory(args, sizeof(*args), false)) {
            PRINTF("(invalid-argv %p)", args);
            break;
        }
        if (*args == NULL)
            break;
        if (test_user_string(*args)) {
            PRINTF("(invalid-addr %p),", *args);
            continue;
        }
        PUTS(*args);
        PUTS(",");
    }

    PUTS("]");
}

static void parse_exec_envp(va_list* ap) {
    const char** envp = va_arg(*ap, const char**);

    if (!envp) {
        PUTS("NULL");
        return;
    }

    PUTS("[");

    int cnt = 0;
    for (; cnt < 2; cnt++, envp++) {
        if (test_user_memory(envp, sizeof(*envp), false)) {
            PRINTF("(invalid-envp %p)", envp);
            break;
        }
        if (*envp == NULL)
            break;
        if (test_user_string(*envp)) {
            PRINTF("(invalid-addr %p),", *envp);
            continue;
        }
        PUTS(*envp);
        PUTS(",");
    }

    if (cnt > 2)
        PRINTF("(%d more)", cnt);

    PUTS("]");
}

static void parse_pipe_fds(va_list* ap) {
    int* fds = va_arg(*ap, int*);

    if (test_user_memory(fds, 2 * sizeof(*fds), false)) {
        PRINTF("[invalid-addr %p]", fds);
        return;
    }
    PRINTF("[%d, %d]", fds[0], fds[1]);
}

#define S(sig) #sig

const char* const siglist[NUM_KNOWN_SIGS + 1] = {
    S(SIGUNUSED), S(SIGHUP),  S(SIGINT),    S(SIGQUIT), S(SIGILL),   S(SIGTRAP),   S(SIGABRT),
    S(SIGBUS),    S(SIGFPE),  S(SIGKILL),   S(SIGUSR1), S(SIGSEGV),  S(SIGUSR2),   S(SIGPIPE),
    S(SIGALRM),   S(SIGTERM), S(SIGSTKFLT), S(SIGCHLD), S(SIGCONT),  S(SIGSTOP),   S(SIGTSTP),
    S(SIGTTIN),   S(SIGTTOU), S(SIGURG),    S(SIGXCPU), S(SIGXFSZ),  S(SIGVTALRM), S(SIGPROF),
    S(SIGWINCH),  S(SIGIO),   S(SIGPWR),    S(SIGSYS),  S(SIGRTMIN),
};

static void parse_signum(va_list* ap) {
    int signum = va_arg(*ap, int);

    if (signum >= 0 && signum <= NUM_KNOWN_SIGS)
        PUTS(signal_name(signum));
    else
        PRINTF("[SIG %d]", signum);
}

static void parse_sigmask(va_list* ap) {
    __sigset_t* sigset = va_arg(*ap, __sigset_t*);

    if (!sigset) {
        PUTS("NULL");
        return;
    }

    if (test_user_memory(sigset, sizeof(*sigset), false)) {
        PRINTF("(invalid-addr %p)", sigset);
        return;
    }

    PUTS("[");

    for (size_t signum = 1; signum <= sizeof(sigset) * 8; signum++)
        if (__sigismember(sigset, signum)) {
            PUTS(signal_name(signum));
            PUTS(",");
        }

    PUTS("]");
}

static void parse_sigprocmask_how(va_list* ap) {
    int how = va_arg(*ap, int);

    switch (how) {
        case SIG_BLOCK:
            PUTS("BLOCK");
            break;
        case SIG_UNBLOCK:
            PUTS("UNBLOCK");
            break;
        case SIG_SETMASK:
            PUTS("SETMASK");
            break;
        default:
            PUTS("<unknown>");
            break;
    }
}

static void parse_timespec(va_list* ap) {
    const struct timespec* tv = va_arg(*ap, const struct timespec*);

    if (!tv) {
        PUTS("NULL");
        return;
    }

    if (test_user_memory((void*)tv, sizeof(*tv), false)) {
        PRINTF("(invalid-addr %p)", tv);
        return;
    }

    PRINTF("[%ld,%ld]", tv->tv_sec, tv->tv_nsec);
}

static void parse_sockaddr(va_list* ap) {
    const struct sockaddr* addr = va_arg(*ap, const struct sockaddr*);

    if (!addr) {
        PUTS("NULL");
        return;
    }

    if (test_user_memory((void*)addr, sizeof(*addr), false)) {
        PRINTF("(invalid-addr %p)", addr);
        return;
    }

    switch (addr->sa_family) {
        case AF_INET: {
            struct sockaddr_in* a = (void*)addr;
            unsigned char* ip     = (void*)&a->sin_addr.s_addr;
            PRINTF("{family=INET,ip=%u.%u.%u.%u,port=htons(%u)}", ip[0], ip[1], ip[2], ip[3],
                   __ntohs(a->sin_port));
            break;
        }

        case AF_INET6: {
            struct sockaddr_in6* a = (void*)addr;
            unsigned short* ip     = (void*)&a->sin6_addr.s6_addr;
            PRINTF(
                "{family=INET,ip=[%x:%x:%x:%x:%x:%x:%x:%x],"
                "port=htons(%u)}",
                __ntohs(ip[0]), __ntohs(ip[1]), __ntohs(ip[2]), __ntohs(ip[3]), __ntohs(ip[4]),
                __ntohs(ip[5]), __ntohs(ip[6]), __ntohs(ip[7]), __ntohs(a->sin6_port));
            break;
        }

        case AF_UNIX: {
            struct sockaddr_un* a = (void*)addr;
            PRINTF("{family=UNIX,path=%s}", a->sun_path);
            break;
        }

        default:
            PUTS("UNKNOWN");
            break;
    }
}

static void parse_domain(va_list* ap) {
    int domain = va_arg(*ap, int);

#define PF_UNSPEC    0  /* Unspecified.  */
#define PF_INET      2  /* IP protocol family.  */
#define PF_AX25      3  /* Amateur Radio AX.25.  */
#define PF_IPX       4  /* Novell Internet Protocol.  */
#define PF_APPLETALK 5  /* Appletalk DDP.  */
#define PF_ATMPVC    8  /* ATM PVCs.  */
#define PF_X25       9  /* Reserved for X.25 project.  */
#define PF_INET6     10 /* IP version 6.  */
#define PF_NETLINK   16
#define PF_PACKET    17 /* Packet family.  */

    switch (domain) {
        case PF_UNSPEC:
            PUTS("UNSPEC");
            break;
        case PF_UNIX:
            PUTS("UNIX");
            break;
        case PF_INET:
            PUTS("INET");
            break;
        case PF_INET6:
            PUTS("INET6");
            break;
        case PF_IPX:
            PUTS("IPX");
            break;
        case PF_NETLINK:
            PUTS("NETLINK");
            break;
        case PF_X25:
            PUTS("X25");
            break;
        case PF_AX25:
            PUTS("AX25");
            break;
        case PF_ATMPVC:
            PUTS("ATMPVC");
            break;
        case PF_APPLETALK:
            PUTS("APPLETALK");
            break;
        case PF_PACKET:
            PUTS("PACKET");
            break;
        default:
            PUTS("UNKNOWN");
            break;
    }
}

static void parse_socktype(va_list* ap) {
    int socktype = va_arg(*ap, int);

    if (socktype & SOCK_NONBLOCK) {
        socktype &= ~SOCK_NONBLOCK;
        PUTS("SOCK_NONBLOCK|");
    }

    if (socktype & SOCK_CLOEXEC) {
        socktype &= ~SOCK_CLOEXEC;
        PUTS("SOCK_CLOEXEC|");
    }

#define SOCK_RAW       3  /* Raw protocol interface.  */
#define SOCK_RDM       4  /* Reliably-delivered messages.  */
#define SOCK_SEQPACKET 5  /* Sequenced, reliable, connection-based, */
#define SOCK_DCCP      6  /* Datagram Congestion Control Protocol.  */
#define SOCK_PACKET    10 /* Linux specific way of getting packets */

    switch (socktype) {
        case SOCK_STREAM:
            PUTS("STREAM");
            break;
        case SOCK_DGRAM:
            PUTS("DGRAM");
            break;
        case SOCK_SEQPACKET:
            PUTS("SEQPACKET");
            break;
        case SOCK_RAW:
            PUTS("RAW");
            break;
        case SOCK_RDM:
            PUTS("RDM");
            break;
        case SOCK_PACKET:
            PUTS("PACKET");
            break;
        default:
            PUTS("UNKNOWN");
            break;
    }
}

static void parse_futexop(va_list* ap) {
    int op = va_arg(*ap, int);

#ifdef FUTEX_PRIVATE_FLAG
    if (op & FUTEX_PRIVATE_FLAG) {
        PUTS("FUTEX_PRIVATE|");
        op &= ~FUTEX_PRIVATE_FLAG;
    }
#endif

#ifdef FUTEX_CLOCK_REALTIME
    if (op & FUTEX_CLOCK_REALTIME) {
        PUTS("FUTEX_CLOCK_REALTIME|");
        op &= ~FUTEX_CLOCK_REALTIME;
    }
#endif

    op &= FUTEX_CMD_MASK;

    switch (op) {
        case FUTEX_WAIT:
            PUTS("FUTEX_WAIT");
            break;
        case FUTEX_WAIT_BITSET:
            PUTS("FUTEX_WAIT_BITSET");
            break;
        case FUTEX_WAKE:
            PUTS("FUTEX_WAKE");
            break;
        case FUTEX_WAKE_BITSET:
            PUTS("FUTEX_WAKE_BITSET");
            break;
        case FUTEX_FD:
            PUTS("FUTEX_FD");
            break;
        case FUTEX_REQUEUE:
            PUTS("FUTEX_REQUEUE");
            break;
        case FUTEX_CMP_REQUEUE:
            PUTS("FUTEX_CMP_REQUEUE");
            break;
        case FUTEX_WAKE_OP:
            PUTS("FUTEX_WAKE_OP");
            break;
        default:
            PRINTF("OP %d", op);
            break;
    }
}

static void parse_fcntlop(va_list* ap) {
    int op = va_arg(*ap, int);

    switch (op) {
        case F_DUPFD:
            PUTS("F_DUPFD");
            break;
        case F_GETFD:
            PUTS("F_GETFD");
            break;
        case F_SETFD:
            PUTS("F_SETFD");
            break;
        case F_GETFL:
            PUTS("F_GETFL");
            break;
        case F_SETFL:
            PUTS("F_SETFL");
            break;
        case F_GETLK:
            PUTS("F_GETLK");
            break;
        case F_SETLK:
            PUTS("F_SETLK");
            break;
        case F_SETLKW:
            PUTS("F_SETLKW");
            break;
        case F_SETOWN:
            PUTS("F_SETOWN");
            break;
        case F_GETOWN:
            PUTS("F_GETOWN");
            break;
        case F_SETSIG:
            PUTS("F_SETSIG");
            break;
        case F_GETSIG:
            PUTS("F_GETSIG");
            break;
        case F_GETLK64:
            PUTS("F_GETLK64");
            break;
        case F_SETLK64:
            PUTS("F_SETLK64");
            break;
        case F_SETLKW64:
            PUTS("F_SETLKW64");
            break;
        case F_SETOWN_EX:
            PUTS("F_SETOWN_EX");
            break;
        case F_GETOWN_EX:
            PUTS("F_GETOWN_EX");
            break;
        case F_GETOWNER_UIDS:
            PUTS("F_GETOWNER_UIDS");
            break;
        default:
            PRINTF("OP %d", op);
            break;
    }
}

static void parse_ioctlop(va_list* ap) {
#if defined (__i386__) || defined(__x86_64__)
    int op = va_arg(*ap, int);
#elif defined(__powerpc64__)
    long unsigned int op = va_arg(*ap, long unsigned int);
#endif

    if (op >= TCGETS && op <= TIOCVHANGUP) {
        const char* opnames[] = {
            "TCGETS",       /* 0x5401 */ "TCSETS",       /* 0x5402 */
            "TCSETSW",      /* 0x5403 */ "TCSETSF",      /* 0x5404 */
            "TCGETA",       /* 0x5405 */ "TCSETA",       /* 0x5406 */
            "TCSETAW",      /* 0x5407 */ "TCSETAF",      /* 0x5408 */
            "TCSBRK",       /* 0x5409 */ "TCXONC",       /* 0x540A */
            "TCFLSH",       /* 0x540B */ "TIOCEXCL",     /* 0x540C */
            "TIOCNXCL",     /* 0x540D */ "TIOCSCTTY",    /* 0x540E */
            "TIOCGPGRP",    /* 0x540F */ "TIOCSPGRP",    /* 0x5410 */
            "TIOCOUTQ",     /* 0x5411 */ "TIOCSTI",      /* 0x5412 */
            "TIOCGWINSZ",   /* 0x5413 */ "TIOCSWINSZ",   /* 0x5414 */
            "TIOCMGET",     /* 0x5415 */ "TIOCMBIS",     /* 0x5416 */
            "TIOCMBIC",     /* 0x5417 */ "TIOCMSET",     /* 0x5418 */
            "TIOCGSOFTCAR", /* 0x5419 */ "TIOCSSOFTCAR", /* 0x541A */
            "FIONREAD",     /* 0x541B */ "TIOCLINUX",    /* 0x541C */
            "TIOCCONS",     /* 0x541D */ "TIOCGSERIAL",  /* 0x541E */
            "TIOCSSERIAL",  /* 0x541F */ "TIOCPKT",      /* 0x5420 */
            "FIONBIO",      /* 0x5421 */ "TIOCNOTTY",    /* 0x5422 */
            "TIOCSETD",     /* 0x5423 */ "TIOCGETD",     /* 0x5424 */
            "TCSBRKP",      /* 0x5425 */ "",
            "TIOCSBRK",     /* 0x5427 */ "TIOCCBRK",   /* 0x5428 */
            "TIOCGSID",     /* 0x5429 */ "TCGETS2",    /* 0x542A */
            "TCSETS2",      /* 0x542B */ "TCSETSW2",   /* 0x542C */
            "TCSETSF2",     /* 0x542D */ "TIOCGRS485", /* 0x542E */
            "TIOCSRS485",   /* 0x542F */ "TIOCGPTN",   /* 0x5430 */
            "TIOCSPTLCK",   /* 0x5431 */ "TCGETX",     /* 0x5432 */
            "TCSETX",       /* 0x5433 */ "TCSETXF",    /* 0x5434 */
            "TCSETXW",      /* 0x5435 */ "TIOCSIG",    /* 0x5436 */
            "TIOCVHANGUP",                             /* 0x5437 */
        };
        PUTS(opnames[op - TCGETS]);
        return;
    }

    if (op >= FIONCLEX && op <= TIOCSERSETMULTI) {
        const char* opnames[] = {
            "FIONCLEX",        /* 0x5450 */ "FIOCLEX",         /* 0x5451 */
            "FIOASYNC",        /* 0x5452 */ "TIOCSERCONFIG",   /* 0x5453 */
            "TIOCSERGWILD",    /* 0x5454 */ "TIOCSERSWILD",    /* 0x5455 */
            "TIOCGLCKTRMIOS",  /* 0x5456 */ "TIOCSLCKTRMIOS",  /* 0x5457 */
            "TIOCSERGSTRUCT",  /* 0x5458 */ "TIOCSERGETLSR",   /* 0x5459 */
            "TIOCSERGETMULTI", /* 0x545A */ "TIOCSERSETMULTI", /* 0x545B */
        };
        PUTS(opnames[op - FIONCLEX]);
        return;
    }

#define TIOCMIWAIT  0x545C /* wait for a change on serial input line(s) */
#define TIOCGICOUNT 0x545D /* read serial port __inline__ interrupt counts */

#if defined(__i386__) || defined(__x86_64__)
    PRINTF("OP 0x%04x", op);
#elif defined(__powerpc64__)
    PRINTF("OP 0x%04lx", op);
#else
#error Unsupported architecture
#endif
}

static void parse_seek(va_list* ap) {
    int seek = va_arg(*ap, int);

    switch (seek) {
        case SEEK_CUR:
            PUTS("SEEK_CUR");
            break;
        case SEEK_SET:
            PUTS("SEEK_SET");
            break;
        case SEEK_END:
            PUTS("SEEK_END");
            break;
        default:
            PRINTF("%d", seek);
            break;
    }
}

static void parse_at_fdcwd(va_list* ap) {
    int fd = va_arg(*ap, int);

    switch (fd) {
        case AT_FDCWD:
            PUTS("AT_FDCWD");
            break;
        default:
            PRINTF("%d", fd);
            break;
    }
}

static void parse_wait_option(va_list* ap) {
    int option = va_arg(*ap, int);

    if (option & WNOHANG)
        PUTS("WNOHANG");
}
