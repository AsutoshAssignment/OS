/*
===========================================================================================================================
Name : 24_orphan.c
Author : Asutosh Panda
Description : Write a program to create an orphan process.
Date: 29th Aug, 2025.
===========================================================================================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    int pid = fork();
    if (pid == 0) {
        printf("Child PID = %d, Parent PID = %d\n", getpid(), getppid());
        sleep(30);
        printf("Child process (after sleep): PID = %d, Parent PID = %d\n", getpid(), getppid());
    } 
    else {
        printf("Parent process: PID = %d\n", getpid());
        printf("Parent process terminated...\n");
        exit(1);
    }

    return 0;
}
/*
=====OUTPUT================================================================================================================
nani@Nani:~/handson1$ ./a.out
Parent process: PID = 3118
Parent process terminated...
Child PID = 3119, Parent PID = 3118
===========================================================================================================================
*/
