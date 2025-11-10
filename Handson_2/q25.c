/*
===========================================================================================================================
Name : q25.c
Author : Asutosh Panda
Description : Write a program to print a message queue's (use msqid_ds and ipc_perm structures)
            a. access permission
            b. uid, gid
            c. time of last message sent and received
            d. time of last change in the message queue
            e. size of the queue
            f. number of messages in the queue
            g. maximum number of bytes allowed
            h. pid of the msgsnd and msgrcv
Date: 30th Sep, 2025
===========================================================================================================================
*/

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>

int main() {
    key_t key = ftok("q25.txt", 62);
    int msgid = msgget(key, 0666 | IPC_CREAT);
    
    struct msqid_ds info;
    msgctl(msgid, IPC_STAT, &info);
    printf("Access Permission: %o\n", info.msg_perm.mode);
    printf("UID: %d\n", info.msg_perm.uid);
    printf("GID: %d\n", info.msg_perm.gid);
    printf("Time of last message sent: %s", ctime(&info.msg_stime));
    printf("Time of last message received: %s", ctime(&info.msg_rtime));
    printf("Time of last change in the message: %s", ctime(&info.msg_ctime));
    printf("Size of queue (in bytes): %lu\n", info.__msg_cbytes);
    printf("Number of messages in the queue: %lu\n", info.msg_qnum);
    printf("Maximum nuuber of bytes allowed: %lu\n", info.msg_qbytes);
    printf("PID of last msgsnd: %d\n", info.msg_lspid);
    printf("PID of last msgrcv: %d\n", info.msg_lrpid);
    return 0;
}
/*
=====OUTPUT================================================================================================================
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ ./a.out
Access Permission: 666
UID: 1000
GID: 1000
Time of last message sent: Thu Jan  1 05:30:00 1970
Time of last message received: Thu Jan  1 05:30:00 1970
Time of last change in the message: Wed Oct  1 14:43:19 2025
Size of queue (in bytes): 0
Number of messages in the queue: 0
Maximum nuuber of bytes allowed: 16384
PID of last msgsnd: 0
PID of last msgrcv: 0

===========================================================================================================================
*/
