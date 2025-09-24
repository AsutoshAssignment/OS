/*
===========================================================================================================================
Name : 29_modify_policy.c
Author : Asutosh Panda
Description : Write a program to get scheduling policy and modify the scheduling policy 

Date: 4th Sept, 2025.
===========================================================================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>

int main() {

    struct sched_param param;
    int policy = sched_getscheduler(0);
    if (policy == -1) {
        printf("Error to get schedule");
        return 1;
    }
    printf("Current policy : %d\n", policy);

    param.sched_priority = 10;
    if (sched_setscheduler(0, SCHED_FIFO, &param) == -1) {
        printf("Error to set schedule");
        return 1;
    }

    printf("New priority: %d\n", param.sched_priority);
    return 0;
}
/*
=====OUTPUT================================================================================================================
nani@Nani:~/handson1$ sudo ./a.out
[sudo] password for nani:            
Current policy : 0
New priority: 10
===========================================================================================================================
*/

