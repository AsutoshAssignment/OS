/*
===========================================================================================================================
Name : 9_file_info.c
Author : Asutosh Panda
Description : Write a program to print the following information about a given file.
            a. inode
            b. number of hard links
            c. uid
            d. gid
            e. size
            f. block size
            g. number of blocks
            h. time of last access
            i. time of last modification
            j. time of last change
Date: 21st Aug, 2025.
===========================================================================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

int main() {    

    struct stat file_statistics;
    if (stat("file1.txt", &file_statistics) == -1) {
        printf("stat failed\n");
    }

    printf("Inode number: %ld\n", file_statistics.st_ino);
    printf("Number of hard links: %ld\n", file_statistics.st_nlink);
    printf("User ID (UID): %d\n", file_statistics.st_uid);
    printf("Group ID (GID): %d\n", file_statistics.st_gid);
    printf("Size: %ld bytes\n", file_statistics.st_size);
    printf("Block size: %ld bytes\n", file_statistics.st_blksize);
    printf("Number of blocks : %ld\n", file_statistics.st_blocks);
    printf("Time of last access: %s", ctime(&file_statistics.st_atime));
    printf("Time of last modification: %s", ctime(&file_statistics.st_mtime));
    printf("Time of last change: %s", ctime(&file_statistics.st_ctime));

    return 0;
}
/*
=====OUTPUT================================================================================================================

nani@Nani:~/handson1$ cc q9_file_info.c
nani@Nani:~/handson1$ ./a.out
Inode number: 3962055
Number of hard links: 1
User ID (UID): 1000
Group ID (GID): 1000
Size: 56 bytes
Block size: 4096 bytes
Number of blocks : 8
Time of last access: Tue Sep  2 14:35:14 2025
Time of last modification: Tue Sep  2 14:35:11 2025
Time of last change: Tue Sep  2 14:35:11 2025

===========================================================================================================================
*/
