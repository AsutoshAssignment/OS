#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_PORT 8080
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

void print_main_menu() {
    printf("\n==============================\n");
    printf("  Banking Management System\n");
    printf("==============================\n");
    printf("1. Login\n");
    printf("2. Exit\n");
    printf("Enter choice: ");
}

void customer_menu(int sock) {
    char buffer[BUFFER_SIZE], response[BUFFER_SIZE];
    int choice;

    while (1) {
        printf("\n--- Customer Menu ---\n");
        printf("1. View Account\n");
        printf("2. Deposit\n");
        printf("3. Withdraw\n");
        printf("4. Transfer\n");
        printf("5. Apply for Loan\n");
        printf("6. Change Password\n");
        printf("7. Feedback\n");
        printf("8. View Transactions\n");
        printf("9. Logout\n");
        printf("10. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 1)
            snprintf(buffer, sizeof(buffer), "VIEW_ACC|");

        else if (choice == 2) {
            float amt;
            printf("Enter amount to deposit: ");
            scanf("%f", &amt);
            snprintf(buffer, sizeof(buffer), "DEPOSIT|%f", amt);
        }

        else if (choice == 3) {
            float amt;
            printf("Enter amount to withdraw: ");
            scanf("%f", &amt);
            snprintf(buffer, sizeof(buffer), "WITHDRAW|%f", amt);
        }

        else if (choice == 4) {
            int target_id;
            float amt;
            printf("Enter receiver user ID: ");
            scanf("%d", &target_id);
            printf("Enter amount to transfer: ");
            scanf("%f", &amt);
            snprintf(buffer, sizeof(buffer), "TRANSFER %d %f", target_id, amt);
        }

        else if (choice == 5) {
            float amt;
            printf("Enter loan amount to apply: ");
            scanf("%f", &amt);
            snprintf(buffer, sizeof(buffer), "LOAN|%f", amt);
        }

        else if (choice == 6) {
            char oldpass[50], newpass[50];
            printf("Enter old password: ");
            scanf("%s", oldpass);
            printf("Enter new password: ");
            scanf("%s", newpass);
            snprintf(buffer, sizeof(buffer), "CHPASS %s %s", oldpass, newpass);
        }

        else if (choice == 7) {
            char feedback[256];
            printf("Enter your feedback: ");
            getchar();
            fgets(feedback, sizeof(feedback), stdin);
            feedback[strcspn(feedback, "\n")] = '\0';
            snprintf(buffer, sizeof(buffer), "FEEDBACK|%s", feedback);
        }

        else if (choice == 8)
            snprintf(buffer, sizeof(buffer), "HISTORY");

        else if (choice == 9) {
            snprintf(buffer, sizeof(buffer), "LOGOUT");
            write(sock, buffer, strlen(buffer));
            break;
        }

        else if (choice == 10) {
            snprintf(buffer, sizeof(buffer), "EXIT");
            write(sock, buffer, strlen(buffer));
            close(sock);
            exit(0);
        }

        else {
            printf("Invalid choice.\n");
            continue;
        }

        write(sock, buffer, strlen(buffer));
        memset(response, 0, sizeof(response));
        read(sock, response, sizeof(response) - 1);
        printf("Server: %s\n", response);
    }
}

void employee_menu(int sock) {
    char buffer[1024], username[50], password[50], field[30], new_value[100];
    int cust_id;

    while (1) {
        printf("\n===== Employee Menu =====\n");
        printf("1. Add New Customer\n");
        printf("2. Modify Customer Details (username/password)\n");
        printf("3. View Customer Transactions\n");
        printf("4. View All Loan Applications\n");
        printf("5. Logout\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
        fflush(stdout);

        int choice;
        scanf("%d", &choice);
        getchar(); // consume newline

        switch (choice) {
            // ADD CUSTOMER
            case 1:
                printf("Enter new customer username: ");
                scanf("%s", username);
                printf("Enter new customer password: ");
                scanf("%s", password);
                snprintf(buffer, sizeof(buffer), "ADD_CUST|%s|%s", username, password);
                write(sock, buffer, strlen(buffer));

                memset(buffer, 0, sizeof(buffer));
                read(sock, buffer, sizeof(buffer));
                printf("%s\n", buffer);
                break;

            // MODIFY CUSTOMER
            case 2:
                    int id;
                    char newuser[50], newpass[50];
                    char sendbuf[200];

                    printf("\n--- Modify Customer Details ---\n");
                    printf("Enter Customer ID: ");
                    scanf("%d", &id);

                    printf("Enter new username (or '-' to keep unchanged): ");
                    scanf("%s", newuser);

                    printf("Enter new password (or '-' to keep unchanged): ");
                    scanf("%s", newpass);

                    // Format: MOD_CUST|<id>|<newusername>|<newpassword>
                    snprintf(sendbuf, sizeof(sendbuf), "MOD_CUST|%d|%s|%s", id, newuser, newpass);
                    write(sock, sendbuf, strlen(sendbuf));

                    // Wait for server response
                    char response[256];
                    int n = read(sock, response, sizeof(response) - 1);
                    if (n > 0) {
                        response[n] = '\0';
                        printf("%s\n", response);
                    }
                    break;

            // VIEW TRANSACTIONS
            case 3:
                printf("Enter customer ID to view transactions: ");
                scanf("%d", &cust_id);
                snprintf(buffer, sizeof(buffer), "VIEW_TXNS|%d", cust_id);
                write(sock, buffer, strlen(buffer));

                printf("\n--- Transaction History ---\n");
                while (1) {
                    memset(buffer, 0, sizeof(buffer));
                    int n = read(sock, buffer, sizeof(buffer) - 1);
                    if (n <= 0) break;
                    buffer[n] = '\0';
                    if (strncmp(buffer, "END", 3) == 0) break;
                    printf("%s", buffer);
                }
                printf("\n--- End of Transactions ---\n");
                break;

            // VIEW LOAN APPLICATIONS
            case 4:
                snprintf(buffer, sizeof(buffer), "VIEW_LOANS");
                write(sock, buffer, strlen(buffer));

                printf("\n--- Loan Applications ---\n");
                while (1) {
                    memset(buffer, 0, sizeof(buffer));
                    int n = read(sock, buffer, sizeof(buffer) - 1);
                    if (n <= 0) break;
                    buffer[n] = '\0';
                    if (strncmp(buffer, "END", 3) == 0) break;
                    printf("%s", buffer);
                }
                printf("\n--- End of Loan List ---\n");
                break;

            // LOGOUT
            case 5:
                snprintf(buffer, sizeof(buffer), "LOGOUT");
                write(sock, buffer, strlen(buffer));
                memset(buffer, 0, sizeof(buffer));
                read(sock, buffer, sizeof(buffer));
                printf("%s\n", buffer);
                return;

            // EXIT
            case 6:
                snprintf(buffer, sizeof(buffer), "EXIT");
                write(sock, buffer, strlen(buffer));
                printf("Exiting...\n");
                close(sock);
                exit(0);

            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}

void manager_menu(int sock) {
    char buffer[512];
    int choice;

    while (1) {
        printf("\n========== Manager Menu ==========\n");
        printf("1. View All Users\n");
        printf("2. Activate / Deactivate Customer\n");
        printf("3. Approve / Reject Employee\n");
        printf("4. View Customer Feedback\n");
        printf("5. Change Password\n");
        printf("6. Logout\n");
        printf("7. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar(); // clear newline

        switch (choice) {
            case 1:
                write(sock, "VIEW_USERS", strlen("VIEW_USERS"));
                break;

            case 2: {
                int id, status;
                printf("Enter customer ID: ");
                scanf("%d", &id);
                printf("Enter 1 to Activate or 0 to Deactivate: ");
                scanf("%d", &status);
                getchar();
                snprintf(buffer, sizeof(buffer), "UPDATE_CUST|%d|%d", id, status);
                write(sock, buffer, strlen(buffer));
                break;
            }

            case 3: {
                int id, decision;
                printf("Enter employee ID: ");
                scanf("%d", &id);
                printf("Enter 1 to Approve or 0 to Reject: ");
                scanf("%d", &decision);
                getchar();
                snprintf(buffer, sizeof(buffer), "APPROVE_EMP|%d|%d", id, decision);
                write(sock, buffer, strlen(buffer));
                break;
            }

            case 4:
                write(sock, "VIEW_FEEDBACK", strlen("VIEW_FEEDBACK"));
                break;

            case 5: {
                char oldpass[50], newpass[50];
                printf("Enter old password: ");
                scanf("%s", oldpass);
                printf("Enter new password: ");
                scanf("%s", newpass);
                snprintf(buffer, sizeof(buffer), "CHANGE_PASS|%s|%s", oldpass, newpass);
                write(sock, buffer, strlen(buffer));
                break;
            }

            case 6:
                write(sock, "LOGOUT", strlen("LOGOUT"));
                return;

            case 7:
                write(sock, "EXIT", strlen("EXIT"));
                close(sock);
                exit(0);

            default:
                printf("Invalid choice.\n");
                continue;
        }

        // --- Read server response ---
        int n = read(sock, buffer, sizeof(buffer) - 1);
        if (n > 0) {
            buffer[n] = '\0';
            printf("\n[Server]: %s\n", buffer);
        } else {
            printf("Server disconnected.\n");
            break;
        }
    }
}

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE], response[BUFFER_SIZE];
    int choice;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server successfully!\n");

    while (1) {
        print_main_menu();
        scanf("%d", &choice);
        getchar();

        if (choice == 1) {
            char username[50], password[50];
            printf("Enter username: ");
            scanf("%s", username);
            printf("Enter password: ");
            scanf("%s", password);

            snprintf(buffer, sizeof(buffer), "LOGIN|%s|%s", username, password);
            write(sock, buffer, strlen(buffer));

            memset(response, 0, sizeof(response));
            read(sock, response, sizeof(response));

            printf("Server: %s\n", response);

          if (strncmp(response, "customer", 8) == 0) {
    printf("Login successful! Welcome, Customer.\n");
    customer_menu(sock);
}
else if (strncmp(response, "employee", 8) == 0) {
    printf("Login successful! Welcome, Employee.\n");
    employee_menu(sock);
}
else if (strncmp(response, "manager", 7) == 0) {
    printf("Login successful! Welcome, Manager.\n");
    manager_menu(sock);  // You can define this later
}
else {
    printf("Login failed or unknown role: %s\n", response);
}

        }

        else if (choice == 2) {
            printf("Exiting...\n");
            close(sock);
            break;
        }

        else {
            printf("Invalid choice!\n");
        }
    }

    return 0;
}
