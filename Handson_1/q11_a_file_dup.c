/*
===========================================================================================================================
Name : 11_a_file_dup.c
Author : Asutosh Panda
Description : WWrite a program to open a file, duplicate the file descriptor and append the file with both the
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
   
    int fd1 = open("source.txt", O_WRONLY);
    if (fd1 == -1) {
        printf("failed to open\n");
    }

    printf("Original file descriptor: %d\n", fd1);
    int fd2 = dup(fd1);
    if (fd2 == -1) {
        printf("failed to duplicate\n");
    }

    printf("Duplicate file descriptor: %d\n", fd2);
    
    char *msg1 = "This is from the original file" ;   
    if (write(fd1, msg1,strlen(msg1 )) == -1) {
        printf("Failed to write original\n");
    }

    char *msg2 = "This is from the duplicate file"; 
    if (write(fd2, msg2,strlen(msg2 )) == -1) {
        printf("Failed to write duplicate\n");
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
nani@Nani:~/handson1$ cat source.txt
This is from the original file
This is from the duplicate file
===========================================================================================================================
*/
