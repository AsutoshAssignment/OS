/*
==========================================================================
==================================================
Name : q23.c
Author : Asutosh Panda
Description:Write a program to print the maximum number of files can be opened within a process and size of a pipe (circular buffer).
Date: 30th Sept, 2025
==========================================================================
==================================================
*/

#include <stdio.h>
#include <unistd.h>

int main() {

    printf("Maximum number of open files is %ld\n", sysconf(_SC_OPEN_MAX));
    printf("Pipe sizeis %ld bytes\n", fpathconf(STDIN_FILENO, _PC_PIPE_BUF));

    return 0;
}
/*
=====OUTPUT===============================================================
==================================================
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ ./a.out
Maximum number of open files is 1024
Pipe sizeis 4096 bytes
==========================================================================
==================================================
*/

