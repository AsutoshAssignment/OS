/*
===========================================================================================================================
Name : 3_create_file.c
Author : Asutosh Panda
Description : Write a program to create a file and print the file descriptor value. Use creat ( ) system call

Date: 20th August, 2025.
===========================================================================================================================
*/

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    int fd = creat("newfile.txt",0777);

    if(fd == -1) {   
        perror("error");
        return 1;
    }

    printf("File descriptor = %d\n", fd);

    if(close(fd) == -1){
        perror("error");
        return 1;
    }

    return 0;
}
/*
=====OUTPUT================================================================================================================
nani@Nani:~/handson1$ ./a.out
File descriptor = 3
===========================================================================================================================
*/

