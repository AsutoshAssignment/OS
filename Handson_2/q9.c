/*
===========================================================================================================================
Name : q9.c
Author : Asutosh Panda
Description : Write a program to ignore a SIGINT signal then reset the default action of the SIGINT signal - Use signal system call.
Date: 27th Sept, 2025
===========================================================================================================================
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void ignore_sigint(int sig) {}

int main() {

    signal(SIGINT, ignore_sigint);

    printf("SIGINT is now being ignored. Press Ctrl+C to test.\n");
    sleep(10);

    signal(SIGINT, SIG_DFL);

    printf("\nSIGINT action has been reset to default. Press Ctrl+C to test.\n");
    sleep(10);

    return 0;
}
/*
=====OUTPUT================================================================================================================
SIGINT is now being ignored. Press Ctrl+C to test.
^C
SIGINT action has been reset to default. Press Ctrl+C to test.
^C
===========================================================================================================================
*/

