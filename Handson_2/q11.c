/*
===========================================================================================================================
Name : q11.c
Author : Asutosh Panda
Description : Write a program to ignore a SIGINT signal then reset the default action of the SIGINT signal -use sigaction system call.
Date: 27th Sept, 2025
===========================================================================================================================
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main() {
    struct sigaction sa;

    sa.sa_handler = SIG_IGN;
    sigaction(SIGINT, &sa, NULL);
    printf("SIGINT ignored. Sleep for 5 sec.....\n");
    sleep(5);

    sa.sa_handler = SIG_DFL;
    sigaction(SIGINT, &sa, NULL);
    printf("\nSIGINT reset. Sleep again for 5 sec.....\n");
    sleep(5);

    return 0;
}
/*
=====OUTPUT================================================================================================================
SIGINT ignored. Sleep for 5 sec.....
^C
SIGINT reset. Sleep again for 5 sec.....

===========================================================================================================================
*/

