/*
===========================================================================================================================
Name : 21_fork.c
Author : Asutosh Panda
Description : Write a program, call fork and print the parent and child process id.
Date: 28th Aug, 2025.
===========================================================================================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
 
    int pid = fork();
    if (pid == 0)
        printf("Child process = %d, Parent PID = %d\n", getpid(), getppid());
    else 
        printf("Parent process: = %d, Child PID = %d\n", getpid(), pid);
    return 0;
}
/*
=====OUTPUT================================================================================================================
nani@Nani:~/handson1$ ./a.out
Parent process: = 8157, Child PID = 8158
Child process = 8158, Parent PID = 8157
===========================================================================================================================
*/
