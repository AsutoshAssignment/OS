/*
===========================================================================================================================
Name : q1_b.c
Author : Asutosh Panda
Description : Write a separate program (for each time domain) to set a interval timer in 10sec and 10micro second.
        a. ITIMER_REAL
        b. ITIMER_VIRTUAL
        c. ITIMER_PROF
Date: 22nd Sept, 2024
===========================================================================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>

void handle_t(int signum) {
    printf("Timer up! (ITIMER_VIRTUAL)\n");
}

int main() {
    struct itimerval timer;
    signal(SIGVTALRM, handle_t);
    timer.it_value.tv_sec = 10;
    timer.it_value.tv_usec = 0;

    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 10;
    setitimer(ITIMER_VIRTUAL, &timer, NULL);
    while (1);
    return 0;
    }
/*
=====OUTPUT================================================================================================================
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ ./a.out
Timer up! (ITIMER_VIRTUAL)
Timer up! (ITIMER_VIRTUAL)
Timer up! (ITIMER_VIRTUAL)
Timer up! (ITIMER_VIRTUAL)
Timer up! (ITIMER_VIRTUAL)
Timer up! (ITIMER_VIRTUAL)
===========================================================================================================================
*/
