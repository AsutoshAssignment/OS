/*
===========================================================================================================================
Name : 23_zombie.c
Author : Asutosh Panda
Description : Write a program to create a Zombie state of the running program.
Date: 29th Aug, 2025.
===========================================================================================================================
*/
#include <stdio.h>
#include <unistd.h> 
#include <sys/types.h>

int main() {
    int pid = fork();
    if (pid == 0) {
        printf("Child process = %d\n", getpid());
        return 1;
    } 
    else {
        printf("Parent process = %d\n", getpid());
        printf("Sleep 30s\n");
        sleep(30);
    }
    return 0;
}
/*
=====OUTPUT================================================================================================================
nani@Nani:~/handson1$ ./a.out
Parent process = 8459
Sleep 30s
Child process = 8460
===========================================================================================================================
*/
