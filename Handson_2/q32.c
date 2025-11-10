/*
===========================================================================================================================
Name : q32.c
Author : Asutosh Panda
Description : Write a program to implement semaphore to protect any critical section.
            a. rewrite the ticket number creation program using semaphore
            b. protect shared memory from concurrent write access
            c. protect multiple pseudo resources ( may be two) using counting semaphore
            d. remove the created semaphore
Date: 1st Oct, 2025
===========================================================================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>   
#include <unistd.h>  
#include <sys/stat.h>

int tno = 0;
int rsrsc1 = 0, rsrc2 = 0;

sem_t *sem_binary, *sem_counting;

void create_ticket() {
    sem_wait(sem_binary);
    tno++;
    printf("Ticket Number: %d\n", tno);
    sem_post(sem_binary);
}

void access_shared_memory() {
    sem_wait(sem_binary); 
    tno += 10;
    printf("Updated Memo: %d\n", tno);
    sem_post(sem_binary);
}

void useRsrc() {
    sem_wait(sem_counting);
    if (rsrsc1 == 0) {
        rsrsc1 = 1;
        printf("Resource 1 in use.\n");
    } else if (rsrc2 == 0) {
        rsrc2 = 1;
        printf("Resource 2 in use.\n");
    }
    sleep(1);
    if (rsrsc1 == 1) {
        rsrsc1 = 0;
        printf("Resource 1 released.\n");
    } else if (rsrc2 == 1) {
        rsrc2 = 0;
        printf("Resource 2 released.\n");
    }
    sem_post(sem_counting);
}

int main() {
    sem_binary = sem_open("/binary", O_CREAT, 0644, 1); 
    sem_counting = sem_open("/counting", O_CREAT, 0644, 3); 
    if (sem_binary == SEM_FAILED || sem_counting == SEM_FAILED) {
        printf("failed");
        exit(1);
    }
    create_ticket();
    access_shared_memory();
    useRsrc();
    useRsrc();
    sem_close(sem_binary);
    sem_close(sem_counting);
    sem_unlink("/binary");
    sem_unlink("/counting");
    return 0;
}
/*
=====OUTPUT================================================================================================================
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ cc 32.c
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ ./a.out
Ticket Number: 1
Updated Memo: 11
Resource 1 in use.
Resource 1 released.
Resource 1 in use.
Resource 1 released.
===========================================================================================================================
*/
