/*
==========================================================================
==================================================
Name : q14.c
Author : Asutosh Panda
Description : Write a simple program to create a pipe, write to the pipe, read from pipe and display on the monitor.
Date: 28th Sept, 2025
==========================================================================
==================================================
*/
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    int pipefd[2];
    char writeMsg[] = "Pipe is Created";
    char readMsg[100];

    pipe(pipefd);
    write(pipefd[1], writeMsg, strlen(writeMsg) + 1);
    read(pipefd[0], readMsg, sizeof(readMsg));
    printf("Read from pipe: %s\n", readMsg);

    return 0;
}

/*
=====OUTPUT===============================================================
==================================================
Read from pipe: Pipe is Created

==========================================================================
==================================================
*/

