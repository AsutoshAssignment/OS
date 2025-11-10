/*
===========================================================================================================================
Name : q30.c
Author : Asutosh Panda
Description : Write a program to create a shared memory.
            a. write some data to the shared memory
            b. attach with O_RDONLY and check whether you are able to overwrite.
            c. detach the shared memory
            d. remove the shared memory
Date: 1st Oct, 2025
===========================================================================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <fcntl.h>

int main() {
    int key = ftok("q30_textfile.txt", 65);
     int shared_id = shmget(key, 1024, 0666 | IPC_CREAT);
    char *text = (char*) shmat(shared_id, (void*)0, 0);
    
    strcpy(text, "Writing to the shared memory");
    printf("Write succesful: %s\n", text);
    
    shmdt(text);
    text = (char*) shmat(shared_id, (void*)0, SHM_RDONLY);
    printf("Read-only mode: %s\n", text);
    strcpy(text, "Modified text");
    
    printf("Attempt to overwrite: %s\n", text);
    shmdt(text);
    shmctl(shared_id, IPC_RMID, NULL);
    return 0;
}
/*
=====OUTPUT================================================================================================================
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ touch textfile.txt
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ cc 30.c
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$./a.out
Data written: Writing to the shared memory
Read-only mode: Writing text to the shared memory
Segmentation fault (core dumped)
===========================================================================================================================
*/
