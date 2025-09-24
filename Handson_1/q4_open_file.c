/*
===========================================================================================================================
Name : 4_open_file.c
Author : Asutosh Panda
Description : Write a program to open an existing file with read write mode.
Date: 20th August, 2025.
===========================================================================================================================
*/
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>


int main() {
    int fd = open("newfile.txt",O_RDWR | O_EXCL);

    if(fd == -1) {                 
        perror("ERROR");   
        return 1;
    }

    printf("File opened successfully\n");

    if(close(fd) == -1){ 
	    perror("ERROR"); 
	    return 1; 
    }

    return 0;
}
/*
=====OUTPUT================================================================================================================
nani@Nani:~/handson1$ ./a.out
File opened successfull
===========================================================================================================================
*/

