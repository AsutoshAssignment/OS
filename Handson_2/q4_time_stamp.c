/*
===========================================================================================================================
Name : q4_time_stamp.c
Author : Asutosh Panda
Description:Write a program to measure how much time is taken to execute 100 getppid ( ) system call. Use time stamp counter.
Date: 22nd September, 2025.
===========================================================================================================================
*/
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <x86intrin.h>

static inline uint64_t get_tsc() {
    	return __rdtscp();
}



int main() {
    uint64_t start, end;
    int i;
    start = get_tsc();
    for (i = 0; i < 100; i++) {
        getppid();
    }
    end = get_tsc();
    printf("Cycles taken for 100 getppid() calls: %llu\n", end - start);
    printf("Average cycles per call: %llu\n", (end - start) / 100);
    return 0;

}
/*
=====OUTPUT================================================================================================================
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ ./a.out
Cycles taken for 100 getppid() calls: 120057
Average cycles per call: 1200
===========================================================================================================================
*/
