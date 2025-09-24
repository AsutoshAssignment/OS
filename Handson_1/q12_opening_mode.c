/*
===========================================================================================================================
Name : 12_opening_mode.c
Author : Asutosh Panda
Description : Write a program to find out the opening mode of a file. Use fcntl.
Date: 25th Aug, 2025.
===========================================================================================================================
*/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
   
    int fd = open("textfile_12.txt", O_RDWR);
    if(fd==-1){
        printf("Error while opening\n");
    }
    int flag = fcntl(fd, F_GETFL);
    if (flag == -1) {
        printf("Failed\n");
	return 1;
    }
    printf("File mode is: \n");
    int access_mode = flag & O_ACCMODE;
    if (flag & O_RDONLY) printf("Opened in Read-only mode\n");
    else if (flag & O_WRONLY) printf("Opened in Write-only mode\n");
    else if (flag & O_RDWR)   printf("Opened in Read and Write mode\n");
    if (flag & O_APPEND)   printf("Opened in Append mode\n");
    if (flag & O_CREAT)   printf("Opened in Create mode\n");

    close(fd);
    return 0;
}
/*
=====OUTPUT================================================================================================================
nani@Nani:~/handson1$ ./a.out
File mode is: 
Opened in Read and Write mode
===========================================================================================================================
*/
