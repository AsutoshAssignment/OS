/*
===========================================================================================================================
Name : 28_realtime.c
Author : Asutosh Panda
Description : Write a program to get maximum and minimum real time priority.
Date: 3rd Sept, 2025.
===========================================================================================================================
*/

#include <stdio.h>
#include <sched.h>

int main() {
    int max_fifo = sched_get_priority_max(SCHED_FIFO);
    int min_fifo = sched_get_priority_min(SCHED_FIFO);
    int max_rr = sched_get_priority_max(SCHED_RR);
    int min_rr = sched_get_priority_min(SCHED_RR);
    printf("FIFO -> Min: %d, Max: %d\n", min_fifo, max_fifo);
    printf("RR   -> Min: %d, Max: %d\n", min_rr, max_rr);
    return 0;
}

/*
=====OUTPUT================================================================================================================
nani@Nani:~/handson1$ ./a.out
FIFO -> Min: 1, Max: 99
RR   -> Min: 1, Max: 99
===========================================================================================================================
*/

