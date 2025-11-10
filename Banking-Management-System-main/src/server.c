#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>
#include "../include/server.h"

#define PORT 8080
#define MAX_CLIENTS 10

sem_t *balance_sem;

extern void handle_session(int client_socket);

void* handle_client(void* socket_desc) {
    int client_socket = *(int*)socket_desc;
    free(socket_desc);
    
    printf("Client thread started (socket: %d)\n", client_socket);
    
    handle_session(client_socket);
    
    close(client_socket);
    printf("Client disconnected (socket: %d)\n", client_socket);
    
    return NULL;
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    signal(SIGPIPE, SIG_IGN);

    balance_sem = sem_open("/bank_balance_sem", O_CREAT, 0644, 1);
    if (balance_sem == SEM_FAILED) {
        perror("Semaphore initialization failed");
        exit(EXIT_FAILURE);
    }

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("Setsockopt failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("╔════════════════════════════════════════╗\n");
    printf("║  BANKING SERVER STARTED                ║\n");
    printf("║  Port: %d                            ║\n", PORT);
    printf("║  Waiting for connections...            ║\n");
    printf("╚════════════════════════════════════════╝\n\n");

    while (1) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        
        if (new_socket < 0) {
            perror("Accept failed");
            continue;
        }

        printf("New client connected: %s:%d (socket: %d)\n", 
        inet_ntoa(address.sin_addr), ntohs(address.sin_port), new_socket);

        pthread_t thread_id;
        int *new_sock = malloc(sizeof(int));
        if (new_sock == NULL) {
            perror("Malloc failed");
            close(new_socket);
            continue;
        }
        
        *new_sock = new_socket;

        if (pthread_create(&thread_id, NULL, handle_client, (void*)new_sock) < 0) {
            perror("Thread creation failed");
            free(new_sock);
            close(new_socket);
            continue;
        }

        pthread_detach(thread_id);
    }

    sem_close(balance_sem);
    sem_unlink("/bank_balance_sem");
    close(server_fd);
    
    return 0;
}
