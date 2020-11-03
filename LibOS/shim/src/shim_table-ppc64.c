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
#include "stack.h"

/* prototypes for functions called from assembly only */
int64_t shim_table_dispatch(uint64_t param1, uint64_t param2, uint64_t param3,
                            uint64_t param4, uint64_t param5, uint64_t param6,
                            uint64_t syscallnr, uint64_t *cr);
void shim_set_context_regs(struct shim_regs* shim_regs);

shim_fp shim_table[LIBOS_SYSCALL_BOUND] = {
    (shim_fp)__shim_restart_syscall,
    (shim_fp)__shim_exit,
    (shim_fp)__shim_fork,
    (shim_fp)__shim_read,
    (shim_fp)__shim_write,
    (shim_fp)__shim_open,			/* 5 */
    (shim_fp)__shim_close,
    (shim_fp)__shim_waitpid,
    (shim_fp)__shim_creat,
    (shim_fp)__shim_link,
    (shim_fp)__shim_unlink,			/* 10 */
    (shim_fp)__shim_execve,
    (shim_fp)__shim_chdir,
    (shim_fp)__shim_time,
    (shim_fp)__shim_mknod,
    (shim_fp)__shim_chmod,			/* 15 */
    (shim_fp)__shim_lchown,
    (shim_fp)0,
    (shim_fp)__shim_stat,
    (shim_fp)__shim_lseek,
    (shim_fp)__shim_getpid,			/* 20 */
    (shim_fp)__shim_mount,
    (shim_fp)0,
    (shim_fp)__shim_setuid,
    (shim_fp)__shim_getuid,
    (shim_fp)0,					/* 25 */
    (shim_fp)__shim_ptrace,
    (shim_fp)__shim_alarm,
    (shim_fp)__shim_fstat,
    (shim_fp)__shim_pause,
    (shim_fp)__shim_utime,			/* 30 */
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)__shim_access,
    (shim_fp)0,//__shim_nice
    (shim_fp)0,					/* 35 */
    (shim_fp)__shim_sync,
    (shim_fp)__shim_kill,
    (shim_fp)__shim_rename,
    (shim_fp)__shim_mkdir,
    (shim_fp)__shim_rmdir,			/* 40 */
    (shim_fp)__shim_dup,
    (shim_fp)__shim_pipe,
    (shim_fp)__shim_times,
    (shim_fp)0,
    (shim_fp)__shim_brk,			/* 45 */
    (shim_fp)__shim_setgid,
    (shim_fp)__shim_getgid,
    (shim_fp)0,//__shim_signal
    (shim_fp)__shim_geteuid,
    (shim_fp)__shim_getegid,			/* 50 */
    (shim_fp)__shim_acct,
    (shim_fp)__shim_umount2,
    (shim_fp)0,
    (shim_fp)__shim_ioctl,
    (shim_fp)__shim_fcntl,			/* 55 */
    (shim_fp)0,
    (shim_fp)__shim_setpgid,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)__shim_umask,			/* 60 */
    (shim_fp)__shim_chroot,
    (shim_fp)__shim_ustat,
    (shim_fp)__shim_dup2,
    (shim_fp)__shim_getppid,
    (shim_fp)__shim_getpgrp,			/* 65 */
    (shim_fp)__shim_setsid,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)__shim_setreuid,			/* 70 */
    (shim_fp)__shim_setregid,
    (shim_fp)0,
    (shim_fp)__shim_sigpending,
    (shim_fp)__shim_sethostname,
    (shim_fp)__shim_setrlimit,			/* 75 */
    (shim_fp)0,
    (shim_fp)__shim_getrusage,
    (shim_fp)__shim_gettimeofday,
    (shim_fp)__shim_settimeofday,
    (shim_fp)__shim_getgroups,			/* 80 */
    (shim_fp)__shim_setgroups,
    (shim_fp)__shim_select,  // ppc_select??
    (shim_fp)__shim_symlink,
    (shim_fp)__shim_lstat,
    (shim_fp)__shim_readlink,			/* 85 */
    (shim_fp)__shim_uselib,
    (shim_fp)__shim_swapon,
    (shim_fp)__shim_reboot,
    (shim_fp)0,
    (shim_fp)__shim_mmap,			/* 90 */
    (shim_fp)__shim_munmap,
    (shim_fp)__shim_truncate,
    (shim_fp)__shim_ftruncate,
    (shim_fp)__shim_fchmod,
    (shim_fp)__shim_fchown,			/* 95 */
    (shim_fp)__shim_getpriority,
    (shim_fp)__shim_setpriority,
    (shim_fp)0,
    (shim_fp)__shim_statfs,
    (shim_fp)__shim_fstatfs,			/* 100 */
    (shim_fp)0,
    (shim_fp)__shim_socketcall,
    (shim_fp)__shim_syslog,
    (shim_fp)__shim_setitimer,
    (shim_fp)__shim_getitimer,			/* 105 */
    (shim_fp)__shim_stat,
    (shim_fp)__shim_lstat,
    (shim_fp)__shim_fstat,
    (shim_fp)__shim_uname,
    (shim_fp)0,					/* 110 */
    (shim_fp)__shim_vhangup,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)__shim_wait4,
    (shim_fp)__shim_swapoff,			/* 115 */
    (shim_fp)__shim_sysinfo,
    (shim_fp)__shim_ipc,
    (shim_fp)__shim_fsync,
    (shim_fp)0,
    (shim_fp)__shim_clone,			/* 120 */
    (shim_fp)__shim_setdomainname,
    (shim_fp)__shim_uname,  // newuname ?
    (shim_fp)0,
    (shim_fp)__shim_adjtimex,
    (shim_fp)__shim_mprotect,			/* 125 */
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)__shim_init_module,
    (shim_fp)__shim_delete_module,
    (shim_fp)0,					/* 130 */
    (shim_fp)__shim_quotactl,
    (shim_fp)__shim_getpgid,
    (shim_fp)__shim_fchdir,
    (shim_fp)0,
    (shim_fp)__shim_sysfs,			/* 135 */
    (shim_fp)__shim_personality,
    (shim_fp)0,
    (shim_fp)__shim_setfsuid,
    (shim_fp)__shim_setfsgid,
    (shim_fp)__shim__llseek,			/* 140 */
    (shim_fp)__shim_getdents,
    (shim_fp)__shim_select,
    (shim_fp)__shim_flock,
    (shim_fp)__shim_msync,
    (shim_fp)__shim_readv,			/* 145 */
    (shim_fp)__shim_writev,
    (shim_fp)__shim_getsid,
    (shim_fp)__shim_fdatasync,
    (shim_fp)0, // sysctl
    (shim_fp)__shim_mlock,			/* 150 */
    (shim_fp)__shim_munlock,
    (shim_fp)__shim_mlockall,
    (shim_fp)__shim_munlockall,
    (shim_fp)__shim_sched_setparam,
    (shim_fp)__shim_sched_getparam,		/* 155 */
    (shim_fp)__shim_sched_setscheduler,
    (shim_fp)__shim_sched_getscheduler,
    (shim_fp)__shim_sched_yield,
    (shim_fp)__shim_sched_get_priority_max,
    (shim_fp)__shim_sched_get_priority_min,	/* 160 */
    (shim_fp)__shim_sched_rr_get_interval,
    (shim_fp)__shim_nanosleep,
    (shim_fp)__shim_mremap,
    (shim_fp)__shim_setresuid,
    (shim_fp)__shim_getresuid,			/* 165 */
    (shim_fp)0,
    (shim_fp)__shim_poll,
    (shim_fp)0,
    (shim_fp)__shim_setresgid,
    (shim_fp)__shim_getresgid,			/* 170 */
    (shim_fp)__shim_prctl,
    (shim_fp)__shim_rt_sigreturn,
    (shim_fp)__shim_rt_sigaction,
    (shim_fp)__shim_rt_sigprocmask,
    (shim_fp)__shim_rt_sigpending,		/* 175 */
    (shim_fp)__shim_rt_sigtimedwait,
    (shim_fp)__shim_rt_sigqueueinfo,
    (shim_fp)__shim_rt_sigsuspend,
    (shim_fp)__shim_pread64,
    (shim_fp)__shim_pwrite64,			/* 180 */
    (shim_fp)__shim_chown,
    (shim_fp)__shim_getcwd,
    (shim_fp)__shim_capget,
    (shim_fp)__shim_capset,
    (shim_fp)__shim_sigaltstack,		/* 185 */
    (shim_fp)__shim_sendfile,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)__shim_vfork,
    (shim_fp)__shim_getrlimit,			/* 190 */
    (shim_fp)__shim_readahead,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,					/* 195 */
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)0,//__shim_pciconfig_read,
    (shim_fp)0,//__shim_pciconfig_write,
    (shim_fp)0,//__shim_pciconfig_iobase,	/* 200 */
    (shim_fp)0,
    (shim_fp)__shim_getdents64,
    (shim_fp)__shim_pivot_root,
    (shim_fp)0,
    (shim_fp)__shim_madvise,			/* 205 */
    (shim_fp)__shim_mincore,
    (shim_fp)__shim_gettid,
    (shim_fp)__shim_tkill,
    (shim_fp)__shim_setxattr,
    (shim_fp)__shim_lsetxattr,			/* 210 */
    (shim_fp)__shim_fsetxattr,
    (shim_fp)__shim_getxattr,
    (shim_fp)__shim_lgetxattr,
    (shim_fp)__shim_fgetxattr,
    (shim_fp)__shim_listxattr,			/* 215 */
    (shim_fp)__shim_llistxattr,
    (shim_fp)__shim_flistxattr,
    (shim_fp)__shim_removexattr,
    (shim_fp)__shim_lremovexattr,
    (shim_fp)__shim_fremovexattr,		/* 220 */
    (shim_fp)__shim_futex,
    (shim_fp)__shim_sched_setaffinity,
    (shim_fp)__shim_sched_getaffinity,
    (shim_fp)0,
    (shim_fp)0,					/* 225 */
    (shim_fp)0,
    (shim_fp)__shim_io_setup,
    (shim_fp)__shim_io_destroy,
    (shim_fp)__shim_io_getevents,
    (shim_fp)__shim_io_submit,			/* 230 */
    (shim_fp)__shim_io_cancel,
    (shim_fp)__shim_set_tid_address,
    (shim_fp)__shim_fadvise64,
    (shim_fp)__shim_exit_group,
    (shim_fp)__shim_lookup_dcookie,		/* 235 */
    (shim_fp)__shim_epoll_create,
    (shim_fp)__shim_epoll_ctl,
    (shim_fp)__shim_epoll_wait,
    (shim_fp)__shim_remap_file_pages,
    (shim_fp)__shim_timer_create,		/* 240 */
    (shim_fp)__shim_timer_settime,
    (shim_fp)__shim_timer_gettime,
    (shim_fp)__shim_timer_getoverrun,
    (shim_fp)__shim_timer_delete,
    (shim_fp)__shim_clock_settime,		/* 245 */
    (shim_fp)__shim_clock_gettime,
    (shim_fp)__shim_clock_getres,
    (shim_fp)__shim_clock_nanosleep,
    (shim_fp)0,// ppc64_swapcontext
    (shim_fp)__shim_tgkill,			/* 250 */
    (shim_fp)__shim_utimes,
    (shim_fp)__shim_statfs, // statfs64
    (shim_fp)__shim_fstatfs,// fstatfs64
    (shim_fp)0,
    (shim_fp)0, // __shim_rtas			/* 255 */
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)__shim_migrate_pages,
    (shim_fp)__shim_mbind,
    (shim_fp)__shim_get_mempolicy,		/* 260 */
    (shim_fp)__shim_set_mempolicy,
    (shim_fp)__shim_mq_open,
    (shim_fp)__shim_mq_unlink,
    (shim_fp)__shim_mq_timedsend,
    (shim_fp)__shim_mq_timedreceive,		/* 265 */
    (shim_fp)__shim_mq_notify,
    (shim_fp)__shim_mq_getsetattr,
    (shim_fp)0,//__shim_kexec_load,
    (shim_fp)0,//__shim_add_key,
    (shim_fp)0,//__shim_request_key,		/* 270 */
    (shim_fp)0,//__shim_keyctl,
    (shim_fp)__shim_waitid,
    (shim_fp)__shim_ioprio_set,
    (shim_fp)__shim_ioprio_get,
    (shim_fp)__shim_inotify_init,		/* 275 */
    (shim_fp)__shim_inotify_add_watch,
    (shim_fp)__shim_inotify_rm_watch,
    (shim_fp)0,
    (shim_fp)0,
    (shim_fp)__shim_pselect6,			/* 280 */
    (shim_fp)__shim_ppoll,
    (shim_fp)__shim_unshare,
    (shim_fp)__shim_splice,
    (shim_fp)__shim_tee,
    (shim_fp)__shim_vmsplice,			/* 285 */
    (shim_fp)__shim_openat,
    (shim_fp)__shim_mkdirat,
    (shim_fp)__shim_mknodat,
    (shim_fp)__shim_fchownat,
    (shim_fp)__shim_futimesat,			/* 290 */
    (shim_fp)__shim_newfstatat,
    (shim_fp)__shim_unlinkat,
    (shim_fp)__shim_renameat,
    (shim_fp)__shim_linkat,
    (shim_fp)__shim_symlinkat,			/* 295 */
    (shim_fp)__shim_readlinkat,
    (shim_fp)__shim_fchmodat,
    (shim_fp)__shim_faccessat,
    (shim_fp)__shim_get_robust_list,
    (shim_fp)__shim_set_robust_list,		/* 300 */
    (shim_fp)__shim_move_pages,
    (shim_fp)__shim_getcpu,
    (shim_fp)__shim_epoll_pwait,
    (shim_fp)__shim_utimensat,
    (shim_fp)__shim_signalfd,			/* 305 */
    (shim_fp)__shim_timerfd_create,
    (shim_fp)__shim_eventfd,
    (shim_fp)__shim_sync_file_range,
    (shim_fp)__shim_fallocate,
    (shim_fp)0,//__shim_subpage_prot,		/* 310 */
    (shim_fp)__shim_timerfd_settime,
    (shim_fp)__shim_timerfd_gettime,
    (shim_fp)__shim_signalfd4,
    (shim_fp)__shim_eventfd2,
    (shim_fp)__shim_epoll_create1,		/* 315 */
    (shim_fp)__shim_dup3,
    (shim_fp)__shim_pipe2,
    (shim_fp)__shim_inotify_init1,
    (shim_fp)__shim_perf_event_open,
    (shim_fp)__shim_preadv,			/* 320 */
    (shim_fp)__shim_pwritev,
    (shim_fp)__shim_rt_tgsigqueueinfo,
    (shim_fp)__shim_fanotify_init,
    (shim_fp)__shim_fanotify_mark,
    (shim_fp)__shim_prlimit64,			/* 325 */
    (shim_fp)__shim_socket,
    (shim_fp)__shim_bind,
    (shim_fp)__shim_connect,
    (shim_fp)__shim_listen,
    (shim_fp)__shim_accept,			/* 330 */
    (shim_fp)__shim_getsockname,
    (shim_fp)__shim_getpeername,
    (shim_fp)__shim_socketpair,
    (shim_fp)__shim_send,
    (shim_fp)__shim_sendto,			/* 335 */
    (shim_fp)__shim_recv,
    (shim_fp)__shim_recvfrom,
    (shim_fp)__shim_shutdown,
    (shim_fp)__shim_setsockopt,
    (shim_fp)__shim_getsockopt,			/* 340 */
    (shim_fp)__shim_sendmsg,
    (shim_fp)__shim_recvmsg,
    (shim_fp)__shim_recvmmsg,
    (shim_fp)__shim_accept4,
    (shim_fp)__shim_name_to_handle_at,		/* 345 */
    (shim_fp)__shim_open_by_handle_at,
    (shim_fp)__shim_clock_adjtime,
    (shim_fp)0,//__shim_sysnfs,
    (shim_fp)__shim_sendmmsg,
    (shim_fp)__shim_setns,			/* 350 */
    (shim_fp)0,//__shim_process_vm_readv,
    (shim_fp)0,//__shim_process_vm_writev,
    (shim_fp)0,//__shim_finit_module,
    (shim_fp)0,//__shim_kcmp,
    (shim_fp)0,//__shim_sched_setattr		/* 355 */
    (shim_fp)0,//__shim_sched_getattr
    (shim_fp)0,//__shim_renameat2,
    (shim_fp)0,//__shim_seccomp,
    (shim_fp)__shim_getrandom,
    (shim_fp)0,//__shim_memfd_create		/* 360 */
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
    (shim_fp)__shim_semtimedop,
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
    (shim_fp)__shim_clock_gettime64,
    (shim_fp)0,
    (shim_fp)0,					/* 405 */
    (shim_fp)__shim_clock_getres_time64,
    (shim_fp)0,
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
    (shim_fp)0,					/* 420 */
    (shim_fp)0,
    (shim_fp)__shim_futex_time64,
    (shim_fp)0,
    (shim_fp)0,
};

#define CR0_SO (0x80000000 >> 3)  /* summary overflow; indicates syscall error */

int64_t shim_table_dispatch(uint64_t param1, uint64_t param2, uint64_t param3,
                            uint64_t param4, uint64_t param5, uint64_t param6,
                            uint64_t syscallnr, uint64_t* cr) {
    typedef uint64_t(syscallfn_t)(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);

    //debug("Calling Syscall %lu.  r3=0x%lx r4=0x%lx r5=0x%lx r6=0x%lx r7=0x%lx\n", syscallnr, param1, param2, param3, param4, param5);

    if (syscallnr >= LIBOS_SYSCALL_BOUND || shim_table[syscallnr] == NULL) {
        *cr |= CR0_SO;
        /* a few are not supported on ppc64 rather than 'not implemented' */
        if (syscallnr == 76 /*__NR_getrlimit */)
            return ENOSYS;
        debug("Syscall %lu not implemented\n", syscallnr);
        debug("STOP\n");while(1);
        return ENOSYS;
    }

    *cr &= ~CR0_SO;

    syscallfn_t* sfn = (syscallfn_t*)shim_table[syscallnr];

    int64_t res = sfn(param1, param2, param3, param4, param5, param6);
    if (syscallnr == 120) {
        //debug("shim_table_dispatch: RETURN FROM CLONE: %ld\n", res);
    }

    if (res < 0) {
        res = -res;
        *cr |= CR0_SO;
    }

    return res;
}

void shim_set_context_regs(struct shim_regs* shim_regs) {
    SHIM_TCB_SET(context.regs, shim_regs);

    if (shim_regs) {
        uint64_t syscallnr = shim_regs->pt_regs.gpr[9];

        if (syscallnr != 2 && syscallnr != 189 && syscallnr != 120)
            return;

        debug("SYSCALLNR: %ld  SP = 0x%lx, IP = 0x%lx, R2 = 0x%lx, R12 = 0x%lx, R13 = 0x%lx\n",
              syscallnr, shim_regs->pt_regs.gpr[1], shim_regs->pt_regs.nip,
              shim_regs->pt_regs.gpr[2], shim_regs->pt_regs.gpr[12], shim_regs->pt_regs.gpr[13]);

        switch (syscallnr) {
        case 2: /* fork */
            debug("fork syscall (%ld) not supported. HALTING\n", syscallnr); while(1);
            break;
        case 189: /* vfork */
            // set r3 to 0 for the child and make sure cr.SO is cleared properly
            shim_regs->pt_regs.gpr[3] = 0;
            shim_regs->pt_regs.ccr &= ~CR0_SO;
            break;
        case 120: /* clone */
            // set r3 to 0 for the child and make sure cr.SO is cleared properly
            shim_regs->pt_regs.gpr[3] = 0;
            shim_regs->pt_regs.ccr &= ~CR0_SO;
            break;
        }
    }
}
