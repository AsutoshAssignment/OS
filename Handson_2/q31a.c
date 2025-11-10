/*
===========================================================================================================================
Name : q31a.c
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
    int *binary_semaphore;
    binary_semaphore = sem_open("/binary", O_CREAT, 0644, 1);
    if (binary_semaphore == SEM_FAILED) {
        printf("failed\n");
        return 1;
    }
    printf("Binary semaphore created and initialized to 1.\n");
    sem_close(binary_semaphore);
    sem_unlink("/binary");
    return 0;
}
/*
=====OUTPUT================================================================================================================
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ cc 31a.c
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ ./a.out
Binary semaphore created and initialized to 1.
===========================================================================================================================
*/
