/*
==========================================================================
==================================================
Name : q16.c
Author : Asutosh Panda
Description : Write a program to send and receive data from parent to child vice versa. Use two way communication.
Date: 28th Sept, 2025
==========================================================================
==================================================
*/
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    int pipe1[2], pipe2[2];
    char parent_msg[] = "This is Parent!";
    char child_msg[] = "This is Child!";
    char buf[100];

    pipe(pipe1);
    pipe(pipe2);

    if (fork() == 0) {  
        close(pipe1[1]); 
        close(pipe2[0]);
        read(pipe1[0], buf, sizeof(buf));
        printf("Child received: %s\n", buf);
        write(pipe2[1], child_msg, sizeof(child_msg));
        close(pipe1[0]);
        close(pipe2[1]);
    } else {  
        close(pipe1[0]); 
        close(pipe2[1]); 
        write(pipe1[1], parent_msg, sizeof(parent_msg));
        read(pipe2[0], buf, sizeof(buf));
        printf("Parent received: %s\n", buf);
        close(pipe1[1]);
        close(pipe2[0]);
    }

    return 0;
}
/*
=====OUTPUT===============================================================
==================================================
Child received: This is Parent!
Parent received: This is Child!

==========================================================================
==================================================
*/

