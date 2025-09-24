/*
===========================================================================================================================
Name : 5_stdin.c
Author : Asutosh Panda
Description : Write a program to create five new files with infinite loop. Execute the program in the background and check the file descriptor table at /proc/pid/fd
Date: 20th August, 2025.
===========================================================================================================================
*/
#include <unistd.h> 

int main() {
    char buffer[100];
    int n = read(0, buffer, sizeof(buffer));
    write(1, buffer, n);
    return 0;
}
/*
=====OUTPUT================================================================================================================
nani@Nani:~/handson1$ ./a.out
hello from stdin
hello from stdin
===========================================================================================================================
*/


