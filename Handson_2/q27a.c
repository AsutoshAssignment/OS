/*
===========================================================================================================================
Name : q27a.c
Author : Asutosh Panda
Description : Write a program to receive messages from the message queue.
            a. with 0 as a flag
            b. with IPC_NOWAIT as a flag
Date: 30th Sep, 2025
===========================================================================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSGSZ 128

struct msgbuf {
    long mtype;
    char mtext[MSGSZ];
};

int main() {
    key_t key = 1234;
    int msqid;
    struct msgbuf msg;
    msqid = msgget(key, 0666 | IPC_CREAT);
    if (msqid < 0) {
        printf("Error in creating message queue\n");
        exit(1);
    }
    printf("Waiting for the message\n");
    if (msgrcv(msqid, &msg, MSGSZ, 0, 0) < 0) {
        printf("Error receiving message\n");
        exit(1);
    }
    printf("Received message: %s\n", msg.mtext);
    return 0;
}
/*
=====OUTPUT================================================================================================================
---------------Terminal 1(sender)---------------
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ cc 27sender.c -o sender
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ ./sender
Message sent: Hello, this is a message from sender!

---------------Terminal 2(reciever)----------------
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ cc 27a.c
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$./a.out
Waiting for the message
Received message: Hello, this is a message from sender!
===========================================================================================================================
*/
