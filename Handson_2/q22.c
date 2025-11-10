/*
===========================================================================================================================
Name : q22.c
Author : Asutosh Panda
Description : Write a program to wait for data to be written into FIFO within 10 seconds, use select system call with FIFO.
Date: 28th Sep, 2025
===========================================================================================================================
*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>

int main() {
    char buffer[100];
    int fd = open("myfifo", O_RDONLY | O_NONBLOCK);

    fd_set fds; 
    FD_ZERO(&fds); 
    FD_SET(fd, &fds);
    struct timeval tv = {10, 0};

    if (select(fd + 1, &fds, NULL, NULL, &tv) > 0) {
        read(fd, buffer, sizeof(buffer));
        printf("Received: %s\n", buffer);
    } else 
        printf("Timeout or error\n");

    close(fd);
    return 0;
}  

/*
=====OUTPUT================================================================================================================
---------------Terminal 1---------------
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ echo "This line was written in cmd" > myfifo
---------------Terminal 2----------------
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ cc 22.c
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ mkfifo myfifo
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ ./a.out
Received: This line was written in cmd
===========================================================================================================================
*/
