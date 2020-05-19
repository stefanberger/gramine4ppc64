/* Copyright (C) 2020 IBM Corporation
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
 * shim_sysdep-ppc64.h
 */

#ifndef _SHIM_SYSDEP_H_
#define _SHIM_SYSDEP_H_

/* This is to help old header files where various syscall numbers are not
   available.  */
#if !defined __NR_semtimedop
#define __NR_semtimedop 392
#endif
#if !defined __NR_semget
#define __NR_semget 393
#endif
#if !defined __NR_semctl
#define __NR_semctl 394
#endif
#if !defined __NR_shmget
#define __NR_shmget 395
#endif
#if !defined __NR_shmctl
#define __NR_shmctl 396
#endif
#if !defined __NR_shmat
#define __NR_shmat 397
#endif
#if !defined __NR_shmdt
#define __NR_shmdt 398
#endif
#if !defined __NR_msgget
#define __NR_msgget 399
#endif
#if !defined __NR_msgsnd
#define __NR_msgsnd 400
#endif
#if !defined __NR_msgrcv
#define __NR_msgrcv 401
#endif
#if !defined __NR_msgctl
#define __NR_msgctl 402
#endif

#endif /* _SHIM_SYSDEP_H_ */
