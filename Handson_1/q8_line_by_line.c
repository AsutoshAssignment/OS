/*
===========================================================================================================================
Name : 8_line_by_line.c
Author : Asutosh Panda
Description : Write a program to open a file in read only mode, read line by line and display each line as it is read.
Close the file when end of file is reached.
Date: 20th Aug, 2025.
===========================================================================================================================
*/
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

int main(){
    int bytes;
    char buffer[1024];
    int fd = open("file1.txt", O_RDONLY);
    if(fd==-1){
        printf("Error opening file\n");
    }
    while ((bytes=read(fd, buffer, sizeof(buffer))) >0){
        write(1, buffer, bytes);    
    }
    close(fd);
    return 0;
}
/*
=====OUTPUT================================================================================================================
nani@Nani:~/handson1$ cat file1.txt
Hello this is file 1 line 1
Hello this is file 1 line 2
nani@Nani:~/handson1$ ./a.out
Hello this is file 1 line 1
Hello this is file 1 line 2
===========================================================================================================================
*/
