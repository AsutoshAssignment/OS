/*
===========================================================================================================================
Name : q34a_client.c
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

int main(int argc, char const *argv[]) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    send(sock, hello, strlen(hello), 0);
    printf("Hello message sent\n");
    read(sock, buffer, 1024);
    printf("Server response: %s\n", buffer);
    close(sock);
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
