/*
===========================================================================================================================
Name : q33a.c
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
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char *message = "Hi, this is a message from the server";
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);
    printf("Server is listening on port %d\n", PORT);
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
    send(new_socket, message, strlen(message), 0);
    printf("Message sent to client\n");
    close(new_socket);
    close(server_fd);
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
================================================================
===========================================================================================================================
*/
