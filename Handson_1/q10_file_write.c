/*
===========================================================================================================================
Name : 10_file_write.c
Author : Asutosh Panda
Description : Write a program to open a file with read write mode, write 10 bytes, move the file pointer by 10
bytes (use lseek) and write again 10 bytes.
            a. check the return value of lseek
            b. open the file with od and check the empty spaces in between the data.
Date: 21st Aug, 2025.
===========================================================================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(){
    const char *filename = "file1.txt";

    int fd = open(filename, O_RDWR);
    if(fd==-1){
        printf("Error while opening\n");
	return 1;
    }
    const char *data1 = "10 Byte feed";
    if (write(fd, data1, 10) != 10) {
        printf("Unable to write first 10 bytes\n");
        close(fd);
        return 1;
    }

    int new_offset = lseek(fd, 10, SEEK_CUR);
    printf("new offset: %d\n", new_offset);

    const char *data2 = "2nd data inp";
    if (write(fd, data2, 10) != 10) {
        printf("Unable to write next 10 bytes\n");
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}
/*
=====OUTPUT================================================================================================================
nani@Nani:~/handson1$ ./a.out
new offset: 20
nani@Nani:~/handson1$ od -c file1.txt
0000000   1   0       B   y   t   e       f   e  \0  \0  \0  \0  \0  \0
0000020  \0  \0  \0  \0   2   n   d       d   a   t   a       i
0000036

===========================================================================================================================
*/
