#include <stdio.h>
#include <stddef.h>
#if ORIGINAL
#include <sys/stat.h>
#else
#include "stat.h"
#endif

void main(void) {
    struct stat s;
    printf("%zu  %zu\n", offsetof(struct stat, st_dev), sizeof(s.st_dev));
    printf("%zu  %zu\n", offsetof(struct stat, st_ino), sizeof(s.st_ino));
    printf("%zu  %zu\n", offsetof(struct stat, st_nlink), sizeof(s.st_nlink));
    printf("%zu  %zu\n", offsetof(struct stat, st_mode), sizeof(s.st_mode));
    printf("%zu  %zu\n", offsetof(struct stat, st_uid), sizeof(s.st_uid));
    printf("%zu  %zu\n", offsetof(struct stat, st_gid), sizeof(s.st_gid));
    printf("%zu  %zu\n", offsetof(struct stat, st_rdev), sizeof(s.st_rdev));
    printf("%zu  %zu\n", offsetof(struct stat, st_size), sizeof(s.st_size));
    printf("%zu  %zu\n", offsetof(struct stat, st_blksize), sizeof(s.st_blksize));
    printf("%zu  %zu\n", offsetof(struct stat, st_blocks), sizeof(s.st_blocks));
    printf("%zu  %zu\n", offsetof(struct stat, st_atime), sizeof(s.st_atime));
    printf("%zu  %zu\n", offsetof(struct stat, st_mtime), sizeof(s.st_mtime));
    printf("%zu  %zu\n", offsetof(struct stat, st_ctime), sizeof(s.st_ctime));
    printf("%zu\n", sizeof(struct stat));
}