/*
===========================================================================================================================
Name : q33b.c
Author : Asutosh Panda
Description : Write a program to communicate between two machines using socket.
Date: 1st Oct, 2025
===========================================================================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    read(sock, buffer, sizeof(buffer));
    printf("Message from server: %s\n", buffer);
    close(sock);
    return 0;
}
/*
=====OUTPUT================================================================================================================
---------------Terminal 1 (Server)----------------
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ cc 33a.c
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ ./a.out
Server is listening on port 8080
Message sent to client

---------------Terminal 2 (Client)---------------
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ cc 33b.c
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ ./a.out
Message from server: Hi, this is a message from the server
===========================================================================================================================
*/
