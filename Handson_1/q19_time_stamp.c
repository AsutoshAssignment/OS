/*
===========================================================================================================================
Name : 19_time_stamp.c
Author : Asutosh Panda
Description : Write a program to find out time taken to execute getpid system call. Use time stamp counter.
Date: 27th Aug, 2025.
===========================================================================================================================
*/
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <x86intrin.h>

static inline uint64_t get_tsc() {
    return __rdtsc();
}

int main() {
    uint64_t start_tsc = get_tsc();
    pid_t pid = getpid();
    uint64_t end_tsc = get_tsc();
    uint64_t time_taken = end_tsc - start_tsc;

    printf("Process ID: %d\n", pid);
    printf("Time taken for getpid() to execute: %llu\n", time_taken);

    return 0;
}
/*
=====OUTPUT================================================================================================================
Process ID: 5842
Time taken for getpid() to execute: 15456
===========================================================================================================================
*/
