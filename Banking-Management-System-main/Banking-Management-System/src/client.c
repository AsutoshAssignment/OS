#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/time.h>
#include <errno.h>

#define PORT 8080
#define SERVER_IP "127.0.0.1"
#define RECV_TIMEOUT_MS 100 
#define MAX_TIMEOUTS 50  

void display_customer_menu();
void display_employee_menu();
void display_manager_menu();
void display_admin_menu();

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    char input[500];

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation error\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        printf("Invalid address\n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection Failed\n");
        return -1;
    }

    printf("Banking Management System - Client\n");
    printf("=====================================\n\n");

    int logged_in = 0;
    char role[20] = {0};

    while (!logged_in) {
        memset(buffer, 0, sizeof(buffer));
        recv(sock, buffer, sizeof(buffer), 0);
        printf("%s", buffer);
        fflush(stdout);

        memset(input, 0, sizeof(input));
        fgets(input, sizeof(input), stdin);
        send(sock, input, strlen(input), 0);

        memset(buffer, 0, sizeof(buffer));
        recv(sock, buffer, sizeof(buffer), 0);
        printf("%s", buffer);
        fflush(stdout);

        memset(input, 0, sizeof(input));
        fgets(input, sizeof(input), stdin);
        send(sock, input, strlen(input), 0);

        memset(buffer, 0, sizeof(buffer));
        recv(sock, buffer, sizeof(buffer), 0);

        if (strncmp(buffer, "LOGIN_FAILED", 12) == 0) {
            printf("Invalid credentials! Please try again.\n\n");
            continue;
        } else if (strncmp(buffer, "ALREADY_LOGGED_IN", 17) == 0) {
            printf("User already logged in! Try another username.\n\n");
            continue;
        } else if (strncmp(buffer, "LOGIN_SUCCESS", 13) == 0) {
            char *role_ptr = strchr(buffer, '|');
            if (role_ptr) {
                role_ptr++;
                char *newline = strchr(role_ptr, '\n');
                if (newline) *newline = '\0';
                strcpy(role, role_ptr);
            }
            printf("Login successful! Role: %s\n\n", role);
            logged_in = 1;
        }
    }

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = RECV_TIMEOUT_MS * 1000;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    while (1) {
        if (strcmp(role, "customer") == 0) {
            display_customer_menu();
        } else if (strcmp(role, "employee") == 0) {
            display_employee_menu();
        } else if (strcmp(role, "manager") == 0) {
            display_manager_menu();
        } else if (strcmp(role, "admin") == 0) {
            display_admin_menu();
        }

        int choice;
        printf("\nEnter choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while(getchar() != '\n');
            continue;
        }
        getchar();

        char choice_str[10];
        sprintf(choice_str, "%d", choice);
        send(sock, choice_str, strlen(choice_str), 0);

        if ((strcmp(role, "customer") == 0 && choice == 9) ||
            (strcmp(role, "employee") == 0 && choice == 7) ||
            (strcmp(role, "manager") == 0 && choice == 9) ||
            (strcmp(role, "admin") == 0 && choice == 6)) {
            printf("Logging out...\n");
            break;
        }

        char response[8192] = {0};
        int response_len = 0;
        int timeout_count = 0;

        while (1) {
            memset(buffer, 0, sizeof(buffer));
            int bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);

            if (bytes > 0) {
                buffer[bytes] = '\0';
                
                if (response_len + bytes < sizeof(response) - 1) {
                    strcat(response, buffer);
                    response_len += bytes;
                }

                timeout_count = 0;

                if (strstr(response, "MENU_READY")) {
                    char *marker = strstr(response, "MENU_READY");
                    *marker = '\0';
                    
                    if (strlen(response) > 0) {
                        printf("%s", response);
                        if (response[strlen(response) - 1] != '\n') {
                            printf("\n");
                        }
                    }
                    break;
                }

                if (response_len >= 2 &&
                    response[response_len-2] == ':' &&
                    response[response_len-1] == ' ') {
                    
                    printf("%s", response);
                    fflush(stdout);

                    memset(input, 0, sizeof(input));
                    if (fgets(input, sizeof(input), stdin) != NULL) {
                        input[strcspn(input, "\n")] = 0;
                        send(sock, input, strlen(input), 0);
                    }

                    memset(response, 0, sizeof(response));
                    response_len = 0;
                    timeout_count = 0;  
                }

            } else if (bytes == 0) {
                printf("\nServer disconnected\n");
                close(sock);
                return 0;

            } else {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    timeout_count++;
                    
                    if (response_len > 0 && timeout_count >= MAX_TIMEOUTS) {
                        printf("Response timeout - assuming complete\n");
                        printf("%s", response);
                        break;
                    }
                    
                    if (response_len == 0 && timeout_count >= MAX_TIMEOUTS) {
                        printf("No response from server\n");
                        break;
                    }
                } else {
                    perror("recv error");
                    close(sock);
                    return -1;
                }
            }
        }
    }

    close(sock);
    return 0;
}

void display_customer_menu() {
    printf("\n\n");
    printf("        CUSTOMER MENU              ║\n");
    printf("\n");
    printf("1. View Balance                    \n");
    printf("2. Deposit Money                   \n");
    printf("3. Withdraw Money                  \n");
    printf("4. Transfer Funds                  \n");
    printf("5. Apply for Loan                  \n");
    printf("6. Change Password                 \n");
    printf("7. Add Feedback                    \n");
    printf("8. View Transaction History        \n");
    printf("9. Logout                          \n");
    printf("\n");
}

void display_employee_menu() {
    printf("\n\n");
    printf("         EMPLOYEE MENU              \n");
    printf("\n");
    printf(" 1. Add New Customer                \n");
    printf(" 2. Modify Customer Details         \n");
    printf(" 3. Process Loan Application        \n");
    printf(" 4. View Assigned Loans             \n");
    printf(" 5. View Customer Transactions      \n");
    printf(" 6. Change Password                 \n");
    printf(" 7. Logout                          \n");
    printf("\n");
}

void display_manager_menu() {
    printf("\n\n");
    printf("         MANAGER MENU               \n");
    printf("╠════════════════════════════════════╣\n");
    printf(" 1. View All Customers              \n");
    printf(" 2. Search Customer                 \n");
    printf(" 3. Modify Customer                 \n");
    printf(" 4. View Feedback                   \n");
    printf(" 5. Change Password                 \n");
    printf(" 6. Generate Reports                \n");
    printf(" 7. Freeze/Unfreeze Account         \n");
    printf(" 8. Assign Loan to Employee         \n");
    printf(" 9. Logout                          \n");
    printf("\n");
}


void display_admin_menu() {
    printf("\n\n");
    printf("          ADMIN MENU                \n");
    printf("\n");
    printf(" 1. Add New Employee                \n");
    printf(" 2. Modify Employee Details         \n");
    printf(" 3. Manage User Roles               \n");
    printf(" 4. Change Password                 \n");
    printf(" 5. View All Users                  \n");
    printf(" 6. Logout                          \n");
    printf("\n");
}
