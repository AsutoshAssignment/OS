/*
===========================================================================================================================
Name : q29.c
Author : Asutosh Panda
Description : Write a program to remove the message queue.
Date: 30th Sep, 2025
===========================================================================================================================
*/
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main() {
    key_t key = ftok("29.txt", 65);
    int msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        printf("Failed to create or access the message queue.\n");
        return 1;
    }
    else{
        printf("Message queue created with ID: %d\n", msgid);
    }   
    msgctl(msgid, IPC_RMID, NULL);
    printf("Message queue removed.\n");
    return 0;
}
/*
=====OUTPUT================================================================================================================
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ touch 29.txt
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ cc 29.c
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ ./a.out
Message queue created with ID: 6
Message queue removed.
===========================================================================================================================
*/
