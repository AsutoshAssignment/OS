// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include "../data/user.h"
#include "../data/account.h"
#include "../data/transaction.h"
#include "../data/loan.h"
#include "../data/feedback.h"


#define PORT 8080
#define BUFFER_SIZE 1024

// forward declarations
void handle_customer(int client_sock, struct User *u);
void handle_employee(int client_sock, struct User *u);
void handle_manager(int client_sock, struct User *u);

void handle_client(int client_sock) {
    char buffer[BUFFER_SIZE];
    struct User u;
    int logged_in = 0;

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int n = read(client_sock, buffer, sizeof(buffer) - 1);
        if (n <= 0) {
            printf("Client disconnected.\n");
            break;
        }

        buffer[n] = '\0';
        printf("Received: %s\n", buffer);
        fflush(stdout);

        // LOGIN Handling
        if (strncmp(buffer, "LOGIN|", 6) == 0) {
            sscanf(buffer + 6, "%49[^|]|%49s", u.username, u.password);
            int result = authenticate_user(&u);

            char response[128];
            if (result == 1) {
                update_login_status(u.id, 1);
                logged_in = 1;

                // Return role to client
                snprintf(response, sizeof(response), "%s", u.role);
                write(client_sock, response, strlen(response));

                // Dispatch role handler
                if (strcmp(u.role, "customer") == 0) {
                    handle_customer(client_sock, &u);
                } 
                else if (strcmp(u.role, "employee") == 0) {
                    handle_employee(client_sock, &u);
                } 

                else if (strcmp(u.role, "manager") == 0) {
                    handle_manager(client_sock, &u);
                } 
                else {
                    write(client_sock, "UNKNOWN_ROLE", 12);
                }
            } 
            else if (result == -2) {
                snprintf(response, sizeof(response), "ALREADY_LOGGED");
                write(client_sock, response, strlen(response));
            } 
            else {
                snprintf(response, sizeof(response), "LOGIN_FAILED");
                write(client_sock, response, strlen(response));
            }
        }

        else if (strcmp(buffer, "EXIT") == 0) {
            if (logged_in)
                update_login_status(u.id, 0);
            printf("Client requested exit.\n");
            break;
        }

        else {
            char response[128] = "ERROR|Unknown command";
            write(client_sock, response, strlen(response));
        }
    }

    close(client_sock);
}

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        exit(1);
    }

    if (listen(server_sock, 5) < 0) {
        perror("listen");
        exit(1);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_len);
        if (client_sock < 0) {
            perror("accept");
            continue;
        }
        printf("Client connected.\n");

        if (fork() == 0) {
            close(server_sock);
            handle_client(client_sock);
            exit(0);
        }
        close(client_sock);
    }

    close(server_sock);
    return 0;
}
