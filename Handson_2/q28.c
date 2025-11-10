/*
===========================================================================================================================
Name : q28.c
Author : Asutosh Panda
Description : Write a program to change the exiting message queue permission. (use msqid_ds structure)
Date: 26th Sep, 2025
===========================================================================================================================
*/
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main() {
    key_t key = ftok("q28.txt", 65);
    int msgid = msgget(key, 0666 | IPC_CREAT);
    
    struct msqid_ds info;
    msgctl(msgid, IPC_STAT, &info);
    info.msg_perm.mode = 0644;
    msgctl(msgid, IPC_SET, &info);
    printf("Permissions changed to 0644\n");
    return 0;
}
/*
=====OUTPUT================================================================================================================
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ touch 28.txt
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ cc 28.c
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ ./a.out
Permissions changed to 0644
===========================================================================================================================
*/
