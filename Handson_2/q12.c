/*
==========================================================================
==================================================
Name : q12.c
Author : Asutosh Panda
Description : Write a program to create an orphan process. Use kill system call to send SIGKILL signal to the parent process from the child process.
Date: 28th Sept, 2025
==========================================================================
==================================================
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }

    if (pid == 0) {
        printf("Child process (PID: %d), killing parent (PID: %d)\n", getpid(), getppid());
        kill(getppid(), SIGKILL);
        sleep(2);  
        printf("Child process (PID: %d) is now orphan, adopted by init (PID: %d)\n", getpid(), getppid());
    } else {  
        sleep(10);  
    }

    return 0;
}
