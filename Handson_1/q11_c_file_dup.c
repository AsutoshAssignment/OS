/*
===========================================================================================================================
Name : 11_c_file_duplicate.c
Author : Asutosh Panda
Description : Write a program to open a file, duplicate the file descriptor and append the file with both the
descriptors and check whether the file is updated properly or not.
            a. use dup
            b. use dup2
            c. use fcntl
Date: 24th Aug, 2025.
===========================================================================================================================
*/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main() {
   
    int fd1 = open("source_c.txt", O_WRONLY);
    if (fd1 == -1) {
        printf("Failed to open\n");
    }

    printf("Original file descriptor: %d\n", fd1);
    int fd2 = fcntl(fd1, F_DUPFD, 0);
    if (fd2 == -1) {
        printf("Failed to duplicate\n");
    }
    
    printf("Duplicate file descriptor: %d\n", fd2);

    const char *msg1 = "This was written using original file.\n";
    if (write(fd1, msg1, strlen(msg1)) == -1) {
        printf("original file write failed");
    }
    const char *msg2 = "This was written using duplicate.\n";
    if (write(fd2, msg2, strlen(msg2)) == -1) {
        printf("duplicate file write failed");
    }
    close(fd1);
    close(fd2);
    return 0;
}
/*
=====OUTPUT================================================================================================================
nani@Nani:~/handson1$ ./a.out
Original file descriptor: 3
Duplicate file descriptor: 4
nani@Nani:~/handson1$ cat source_c.txt
This was written using original file.
This was written using duplicate.
===========================================================================================================================
*/
