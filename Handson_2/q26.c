/*
===========================================================================================================================
Name : q26.c
Author : Asutosh Panda
Description : Write a program to send messages to the message queue. Check $ipcs -q
Date: 30th Sep, 2025
===========================================================================================================================
*/
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

struct msg_buffer {
    long msg_type;
    char msg_text[100];
};

int main() {
    key_t key = ftok("progfile", 65);
    int msgid = msgget(key, 0666 | IPC_CREAT);
    struct msg_buffer message;
    message.msg_type = 1;
    strcpy(message.msg_text, "A new message to the queue");
    msgsnd(msgid, &message, sizeof(message.msg_text), 0);
    printf("Message sent: %s\n", message.msg_text);
}
/*
=====OUTPUT================================================================================================================
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ ./a.out
Message sent: A new message to the queue
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ ipcs -q

------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages    
0x410236c6 0          asutosh    666        0            0           
0x41023bdb 1          asutosh    666        0            0           
0x41023b9e 2          asutosh    666        0            0           
0x41023d30 3          asutosh    666        0            0           
0x41023c15 4          asutosh    666        0            0           
0x41023d48 5          asutosh    666        0            0           
0xffffffff 6          asutosh    666        0            0           
0x000004d2 7          asutosh    666        128          1           
0x3e02369a 8          asutosh    666        0            0           
0x3e024c6b 9          asutosh    666        0            0           
0x41023e7c 10         asutosh    666        100          1           
===========================================================================================================================
*/
