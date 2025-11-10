/*
===========================================================================================================================
Name : q6.c
Author : Asutosh Panda
Description : Write a simple program to create three threads.
Date: 27th Sept, 2025
===========================================================================================================================
*/

#include <stdio.h>
#include <pthread.h>

void* thread_function(void* arg) {
    int th_count = *(int*)arg;
    printf("Thread %d \n", th_count);
    return NULL;
}

int main() {
    pthread_t threads[3];
    int th_counts[3] = {1, 2, 3};

    for(int i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, thread_function, &th_counts[i]);
    }

    for(int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

/*
=====OUTPUT================================================================================================================
Thread 1 
Thread 2 
Thread 3 
===========================================================================================================================
*/

