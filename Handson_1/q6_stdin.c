/*
===========================================================================================================================
Name : 6.c
Author : Asutosh Panda
Description : Write a program to take input from STDIN and display on STDOUT. Use only read/write system calls
Date: 20th Aug, 2025.
===========================================================================================================================
*/

#include <unistd.h>
int main() {
    char buffer[1024];
    int n;
    if((n=read(0, buffer, sizeof(buffer))) > 0) {
        write(1, buffer, n);
    }

    return 0;
}

/*
=====OUTPUT==================================================================================================
nani@Nani:~/handson1$ ./a.out
This line is stdin. The next next line will be stdout.
This line is stdin. The next next line will be stdout.
=====OUTPUT==================================================================================================
*/
