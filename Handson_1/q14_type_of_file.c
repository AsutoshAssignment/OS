/*
===========================================================================================================================
Name : 14_type_of_file.c
Author : Asutosh Panda
Description : Write a program to find the type of a file.
            a. Input should be taken from command line.
            b. program should be able to identify any type of a file.
Date: 26th Aug, 2025.
===========================================================================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("You need to pass a file name after a.out \n");
    }

    const char *file_name = argv[1];
    struct stat file_stats;
   
    if (lstat(file_name, &file_stats) == -1) {
        printf("lstat failed\n");
    }
    printf("%s is ", file_name);
    if (S_ISDIR(file_stats.st_mode)) {
        printf("a directory\n");
    } else if (S_ISREG(file_stats.st_mode)) {
        printf("a regular file\n");
    } else if (S_ISLNK(file_stats.st_mode)) {
        printf("a link\n");
    } else if (S_ISFIFO(file_stats.st_mode)) {
        printf("a named pipe\n");
    } else if (S_ISCHR(file_stats.st_mode)) {
        printf("a character device\n");
    } else if (S_ISBLK(file_stats.st_mode)) {
        printf("a block device\n");
    }  else if (S_ISSOCK(file_stats.st_mode)) {
        printf("a socket\n");
    } else {
        printf("of unknown type\n");
    }

    return 0;
}
/*
=====OUTPUT================================================================================================================
nani@Nani:~/handson1$ ./a.out
You need to pass a file name after a.out 
lstat failed
(null) is of unknown type
nani@Nani:~/handson1$ ./a.out file1.txt
file1.txt is a regular file

===========================================================================================================================
*/
