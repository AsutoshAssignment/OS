/*
===========================================================================================================================
Name : q1_a.c
Author : Asutosh Panda
Description : Write a program to set (any one) system resource limit. Use setrlimit system call.
Date: 27th Sept, 2025
===========================================================================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>

int main() {
    struct rlimit rl;

    rl.rlim_cur = 1; 
    rl.rlim_max = 2; 

    setrlimit(RLIMIT_CPU, &rl);

    printf("CPU time limit set to 1 second.\n");

    while (1);

    return EXIT_SUCCESS;
};
/*
=====OUTPUT================================================================================================================
CPU time limit set to 1 second.
CPU time limit exceeded

===========================================================================================================================
*/

