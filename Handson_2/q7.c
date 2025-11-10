/*
===========================================================================================================================
Name : q7.c
Author : Asutosh Panda
Description : Write a simple program to create three threads.
Date: 27th Sept, 2025
===========================================================================================================================
*/

#include <stdio.h>
#include <pthread.h>

void* thread(void* arg) {
    pthread_t thread_id = pthread_self();
    printf("Thread ID: %lu\n", thread_id);
    return NULL;
}

int main() {
    pthread_t threads[3];

    for(int i = 0; i < 3; i++) 
        pthread_create(&threads[i], NULL, thread, NULL);
    
    for(int i = 0; i < 3; i++) 
        pthread_join(threads[i], NULL);

    return 0;
}



/*
=====OUTPUT================================================================================================================
Thread ID: 126681488553536
Thread ID: 126681480160832
Thread ID: 126681471768128

===========================================================================================================================
*/

