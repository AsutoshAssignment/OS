/*
===========================================================================================================================
Name : q31b.c
Author : Asutosh Panda
Description : Write a program to create a semaphore and initialize value to the semaphore.
            a. create a binary semaphore
            b. create a counting semaphore
Date: 1st Oct, 2025
===========================================================================================================================
*/

#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
int main() {
    int *counting_semaphore;
    counting_semaphore = sem_open("/counting", O_CREAT, 0644, 5);
    if (counting_semaphore == SEM_FAILED) {
        printf("failed\n");
        return 1;
    }
    printf("Counting semaphore created and initialized to 5.\n");
    sem_close(counting_semaphore);
    sem_unlink("/counting");
    return 0;
}
/*
=====OUTPUT================================================================================================================
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ cc 31b.c
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ ./a.out
Counting semaphore created and initialized to 5.
===========================================================================================================================
*/
