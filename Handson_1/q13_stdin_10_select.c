/*
===========================================================================================================================
Name : 13_stdin_10_select.c
Author : Asutosh Panda
Description : Write a program to wait for a STDIN for 10 seconds using select. Write a proper print statement to
verify whether the data is available within 10 seconds or not (check in $man 2 select).
Date: 25th Aug, 2025.
===========================================================================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>

int main() {
    fd_set input_fds;                 
    struct timeval wait;               
    char text[100];          

    FD_ZERO(&input_fds);
    FD_SET(STDIN_FILENO, &input_fds);

    printf("you have 10 seconds:\n");

    wait.tv_sec = 10;   
    wait.tv_usec = 0;   

    int status = select(STDIN_FILENO + 1, &input_fds, NULL, NULL, &wait);

    if (status == -1) {
        printf("Error.\n");
    } 
    else if (status > 0) {
        int chars = read(STDIN_FILENO, text, sizeof(text));
        if (chars > 0) {
            printf("Input: %s\n", text);
        }
    } 
    else {
        printf("Timeout\n");
    }

    return 0;
}
/*
=====OUTPUT================================================================================================================
nani@Nani:~/handson1$ ./a.out
you have 10 seconds:
Timeout
nani@Nani:~/handson1$ ./a.out
you have 10 seconds:
Fast input
Input: Fast input
===========================================================================================================================
*/
