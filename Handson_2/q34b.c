/*
===========================================================================================================================
Name : q34b.c
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
#include <pthread.h>

#define PORT 8080

void *handle_client(void *socket_desc) {
    int new_socket = *(int*)socket_desc;
    char buffer[1024] = {0};
    const char *message = "Hi, this is a message from server";
    read(new_socket, buffer, 1024);
    printf("Client: %s\n", buffer);
    send(new_socket, message, strlen(message), 0);
    close(new_socket);
    free(socket_desc);
    pthread_exit(NULL);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);
    while (1) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        pthread_t thread_id;
        int *new_sock = malloc(sizeof(int));
        *new_sock = new_socket;
        pthread_create(&thread_id, NULL, handle_client, (void*)new_sock);
    }
    return 0;
}
/*
=====OUTPUT================================================================================================================
---------------Terminal 1 (Server)----------------
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ cc 34b.c -o server
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
