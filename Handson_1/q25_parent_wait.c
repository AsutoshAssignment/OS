/*
===========================================================================================================================
Name : 25_parent_wait.c
Author : Asutosh Panda
Description : Write a program to create three child processes. The parent should wait for a particular child (use
waitpid system call).
Date: 30th Aug, 2025.
===========================================================================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int pid1, pid2, pid3;
    int status;   

    if ((pid1 = fork()) == 0) {
        printf("Child 1 PID: %d\n", getpid());
        exit(0);
    } 
    if ((pid2 = fork()) == 0) {
        printf("Child 2 PID: %d\n", getpid());
        exit(0);
    } 

    if ((pid3 = fork()) == 0) {
        printf("Child 3 PID: %d\n", getpid());
        exit(0);
    } 

    if (waitpid(pid1, &status, 0) == -1) {
        printf("wait error");
        exit(1);
    }

    printf("Parent process PID: %d exiting\n", getpid());
    return 0;
}
/*
=====OUTPUT================================================================================================================
nani@Nani:~/handson1$ ./a.out
Child 1 PID: 3683
Child 2 PID: 3684
Child 3 PID: 3685
Parent process PID: 3682 exiting
===========================================================================================================================
*/
