/*
===========================================================================================================================
Name : q1_a.c
Author : Asutosh Panda
Description : Write a separate program (for each time domain) to set a interval timer in 10sec and 10micro second.
        a. ITIMER_REAL
        b. ITIMER_VIRTUAL
        c. ITIMER_PROF
Date: 27th Sept, 2025
===========================================================================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>

void handle_t(int signum) {
    printf("Timer up! (ITIMER_REAL)\n");
}

int main() {
    struct itimerval timer;
    signal(SIGALRM, handle_t);
    timer.it_value.tv_sec = 10;
    timer.it_value.tv_usec = 0;

    timer.it_interval.tv_sec = 10;
    timer.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &timer, NULL);
    while (1);
    return 0;
}
/*
=====OUTPUT================================================================================================================
Timer up! (ITIMER_REAL)
Timer up! (ITIMER_REAL)
Timer up! (ITIMER_REAL)
Timer up! (ITIMER_REAL)

===========================================================================================================================
*/
