/*
===========================================================================================================================
Name : 24.c
Author : Asutosh Panda
Description : Write a program to create a message queue and print the key and message queue id.
Date: 30th Sep, 2025
===========================================================================================================================
*/
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main() {
    key_t key = ftok("24.txt", 65);
    int msgid = msgget(key, 0666 | IPC_CREAT);
    
    printf("Key: %d\n", key);
    printf("Message Queue ID: %d\n", msgid);
    
    return 0;
}
/*
=====OUTPUT================================================================================================================
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ ./a.out
Key: 1090665374
Message Queue ID: 2

===========================================================================================================================
*/
