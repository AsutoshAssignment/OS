/*
===========================================================================================================================
Name : q34a.c
Author : Asutosh Panda
Description : Write a program to create a concurrent server.
            a. use fork
            b. use pthread_create
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
    char buffer[1024] = {0};
    const char *message = "Hi, this is a message from server";
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3) ;
    while (1) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if (fork() == 0) {
            read(new_socket, buffer, 1024);
            printf("Client: %s\n", buffer);
            send(new_socket, message, strlen(message), 0);
            close(new_socket);
            exit(0);
        }
    }
    return 0;
}
/*
=====OUTPUT================================================================================================================
---------------Terminal 1 (Server)----------------
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ cc 34a.c -o server
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ ./server
Client: Hello from client
^C

---------------Terminal 2 (Client)---------------
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ cc client.c -o client
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ ./client
Hello message sent
Server response: Hi, this is a message from server
===========================================================================================================================
*/
