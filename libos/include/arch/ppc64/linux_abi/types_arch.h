/* SPDX-License-Identifier: LGPL-3.0-or-later */
/* Copyright (C) 2023 Intel Corporation
 *                    Michał Kowalczyk <mkow@invisiblethingslab.com>
 * Copyright (C) 2023 IBM Corporation
 *                    Stefan Berger <stefanb@linux.ibm.com>
 */

#pragma once

typedef unsigned int  uid_t;
typedef unsigned int  gid_t;
typedef int           pid_t;
typedef unsigned int  mode_t;
typedef long          off_t;
typedef long long     loff_t;
typedef unsigned int  dev_t;
typedef unsigned long ino_t;
typedef int           clockid_t;


#define SEMOP		1
#define SEMGET		2
#define SEMCTL		3
#define SEMTIMEDOP	4

#define MSGSND		11
#define MSGRCV		12
#define MSGGET		13
#define MSGCTL		14
