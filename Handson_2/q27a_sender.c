/*
===========================================================================================================================
Name : q27_sender.c
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
#include <string.h>

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
        exit(EXIT_FAILURE);
    }
    msg.mtype = 1;
    strcpy(msg.mtext, "Hello, this is a message from sender!");
    if (msgsnd(msqid, &msg, sizeof(msg.mtext), 0) < 0) {
        printf("Error sending message\n");
        exit(EXIT_FAILURE);
    }
    printf("Message sent: %s\n", msg.mtext);
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
