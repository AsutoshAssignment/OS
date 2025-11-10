#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>     
#include <netinet/tcp.h>     
#include <stdlib.h>
#include <fcntl.h>
#include <sys/file.h>
#include "../include/admin.h"
#include "../include/data_structures.h"

extern int add_new_customer(const char* username, const char* password, double initial_balance);
extern int update_password(const char* username, const char* new_password);
extern int verify_customer_exists(const char* username);
extern int verify_user_exists(const char* username);
extern int change_user_role(const char* username, const char* new_role);
extern void log_transaction(const char* username, const char* type, double amount, const char* details);

ssize_t read_line_admin(int fd, char *buffer, size_t size) {
    size_t i = 0;
    char c;
    ssize_t bytes_read;
    
    while (i < size - 1) {
        bytes_read = read(fd, &c, 1);
        if (bytes_read <= 0) break;
        if (c == '\n') {
            buffer[i++] = c;
            break;
        }
        buffer[i++] = c;
    }
    buffer[i] = '\0';
    return i;
}

void admin_add_employee(int sock, const char* admin_username);
void admin_modify_employee(int sock, const char* admin_username);
void admin_manage_roles(int sock, const char* admin_username);
void admin_change_password(int sock, const char* admin_username);
void admin_view_all_users(int sock, const char* admin_username);

void handle_admin_operations(int client_socket, const char* username) {
    char buffer[100];
    int choice;
    
    int flag = 1;
    setsockopt(client_socket, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(int));
    
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int bytes = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes <= 0) return;
        
        choice = atoi(buffer);
        
        switch(choice) {
            case 1: admin_add_employee(client_socket, username); break;
            case 2: admin_modify_employee(client_socket, username); break;
            case 3: admin_manage_roles(client_socket, username); break;
            case 4: admin_change_password(client_socket, username); break;
            case 5: admin_view_all_users(client_socket, username); break;
            case 6: return;
            default:
                send(client_socket, "Invalid choice!\n", 16, 0);
                send(client_socket, "MENU_READY", 10, 0);
        }
    }
}

void admin_add_employee(int sock, const char* admin_username) {
    char buffer[100];
    char new_username[50], new_password[50], role[20];
    
    const char* prompt1 = "Enter new username: ";
    send(sock, prompt1, strlen(prompt1), MSG_NOSIGNAL);
    
    memset(buffer, 0, sizeof(buffer));
    int bytes = recv(sock, buffer, sizeof(buffer), 0);
    if (bytes <= 0) {
        send(sock, "MENU_READY", 10, 0);
        return;
    }
    buffer[bytes] = '\0';
    sscanf(buffer, "%49s", new_username);
    
    if (verify_user_exists(new_username)) {
        send(sock, "Username already exists!\n", 25, 0);
        send(sock, "MENU_READY", 10, 0);
        return;
    }
    
    const char* prompt2 = "Enter password: ";
    send(sock, prompt2, strlen(prompt2), MSG_NOSIGNAL);
    
    memset(buffer, 0, sizeof(buffer));
    bytes = recv(sock, buffer, sizeof(buffer), 0);
    if (bytes <= 0) {
        send(sock, "MENU_READY", 10, 0);
        return;
    }
    buffer[bytes] = '\0';
    sscanf(buffer, "%49s", new_password);

    const char* prompt3 = "Enter role (employee/manager): ";
    send(sock, prompt3, strlen(prompt3), MSG_NOSIGNAL);
    
    memset(buffer, 0, sizeof(buffer));
    bytes = recv(sock, buffer, sizeof(buffer), 0);
    if (bytes <= 0) {
        send(sock, "MENU_READY", 10, 0);
        return;
    }
    buffer[bytes] = '\0';
    sscanf(buffer, "%19s", role);
    
    if (strcmp(role, "employee") != 0 && strcmp(role, "manager") != 0) {
        send(sock, "Invalid role! Use 'employee' or 'manager'\n", 42, 0);
        send(sock, "MENU_READY", 10, 0);
        return;
    }
    
    int account_num = add_new_customer(new_username, new_password, 0.0);
    if (account_num > 0) {
        if (change_user_role(new_username, role) == 0) {
            char response[200];
            snprintf(response, sizeof(response),
                    "%s added successfully!\nUsername: %s\nAccount Number: %d\n",
                    role, new_username, account_num);
            send(sock, response, strlen(response), 0);
            
            char details[150];
            snprintf(details, sizeof(details), "New %s %s added", role, new_username);
            log_transaction(admin_username, "ADD_EMPLOYEE", 0, details);
        } else {
            send(sock, "Failed to set role!\n", 20, 0);
        }
    } else {
        send(sock, "Failed to add employee!\n", 24, 0);
    }
    
    send(sock, "MENU_READY", 10, 0);
}

void admin_modify_employee(int sock, const char* admin_username) {
    char buffer[100];
    char employee_username[50];
    
    const char* prompt1 = "Enter employee username: ";
    send(sock, prompt1, strlen(prompt1), MSG_NOSIGNAL);
    
    memset(buffer, 0, sizeof(buffer));
    int bytes = recv(sock, buffer, sizeof(buffer), 0);
    if (bytes <= 0) {
        send(sock, "MENU_READY", 10, 0);
        return;
    }
    buffer[bytes] = '\0';
    sscanf(buffer, "%49s", employee_username);
    
    if (!verify_user_exists(employee_username)) {
        send(sock, "Employee not found!\n", 20, 0);
        send(sock, "MENU_READY", 10, 0);
        return;
    }
    
    const char* prompt2 = "Enter new password: ";
    send(sock, prompt2, strlen(prompt2), MSG_NOSIGNAL);
    
    memset(buffer, 0, sizeof(buffer));
    bytes = recv(sock, buffer, sizeof(buffer), 0);
    if (bytes <= 0) {
        send(sock, "MENU_READY", 10, 0);
        return;
    }
    buffer[bytes] = '\0';
    
    char new_password[50];
    sscanf(buffer, "%49s", new_password);
    
    if (update_password(employee_username, new_password) == 0) {
        send(sock, "Employee details modified successfully!\n", 42, 0);
        
        char details[150];
        snprintf(details, sizeof(details), "Modified employee %s", employee_username);
        log_transaction(admin_username, "MODIFY_EMPLOYEE", 0, details);
    } else {
        send(sock, "Failed to modify employee!\n", 27, 0);
    }
    
    send(sock, "MENU_READY", 10, 0);
}

void admin_manage_roles(int sock, const char* admin_username) {
    char buffer[100];
    char target_username[50], new_role[20];

    const char* prompt1 = "Enter username: ";
    send(sock, prompt1, strlen(prompt1), MSG_NOSIGNAL);
    
    memset(buffer, 0, sizeof(buffer));
    int bytes = recv(sock, buffer, sizeof(buffer), 0);
    if (bytes <= 0) {
        send(sock, "MENU_READY", 10, 0);
        return;
    }
    buffer[bytes] = '\0';
    sscanf(buffer, "%49s", target_username);
    
    if (!verify_user_exists(target_username)) {
        send(sock, "User not found!\n", 16, 0);
        send(sock, "MENU_READY", 10, 0);
        return;
    }
    
    const char* prompt2 = "Enter new role (customer/employee/manager): ";
    send(sock, prompt2, strlen(prompt2), MSG_NOSIGNAL);
    
    memset(buffer, 0, sizeof(buffer));
    bytes = recv(sock, buffer, sizeof(buffer), 0);
    if (bytes <= 0) {
        send(sock, "MENU_READY", 10, 0);
        return;
    }
    buffer[bytes] = '\0';
    sscanf(buffer, "%19s", new_role);
    
    if (strcmp(new_role, "customer") != 0 &&
        strcmp(new_role, "employee") != 0 &&
        strcmp(new_role, "manager") != 0) {
        send(sock, "Invalid role!\n", 14, 0);
        send(sock, "MENU_READY", 10, 0);
        return;
    }
    
    if (change_user_role(target_username, new_role) == 0) {
        char response[100];
        snprintf(response, sizeof(response),
                "Role changed to %s successfully!\n", new_role);
        send(sock, response, strlen(response), 0);
        
        char details[150];
        snprintf(details, sizeof(details), "Changed role of %s to %s", target_username, new_role);
        log_transaction(admin_username, "CHANGE_ROLE", 0, details);
    } else {
        send(sock, "Failed to change role!\n", 23, 0);
    }
    
    send(sock, "MENU_READY", 10, 0);
}

void admin_change_password(int sock, const char* admin_username) {
    char buffer[100];
    char new_password[50];
    
    const char* prompt = "Enter new password: ";
    send(sock, prompt, strlen(prompt), MSG_NOSIGNAL);
    
    memset(buffer, 0, sizeof(buffer));
    int bytes = recv(sock, buffer, sizeof(buffer), 0);
    if (bytes <= 0) {
        send(sock, "MENU_READY", 10, 0);
        return;
    }
    buffer[bytes] = '\0';
    sscanf(buffer, "%49s", new_password);
    
    if (strlen(new_password) < 4) {
        send(sock, "Password too short (min 4 characters)!\n", 39, 0);
        send(sock, "MENU_READY", 10, 0);
        return;
    }
    
    if (update_password(admin_username, new_password) == 0) {
        send(sock, "Password changed successfully!\n", 33, 0);
        log_transaction(admin_username, "PASSWORD_CHANGE", 0, "Password updated");
    } else {
        send(sock, "Failed to change password!\n", 27, 0);
    }
    
    send(sock, "MENU_READY", 10, 0);
}

void admin_view_all_users(int sock, const char* admin_username) {
    int fd = open("data/customers.txt", O_RDONLY);
    if (fd == -1) {
        send(sock, "No users found\n", 15, 0);
        send(sock, "MENU_READY", 10, 0);
        return;
    }
    
    char line[200];
    char response[4096] = "=== All Users ===\n";
    
    while (read_line_admin(fd, line, sizeof(line)) > 0) {
        strcat(response, line);
    }
    
    close(fd);
    send(sock, response, strlen(response), 0);
    send(sock, "MENU_READY", 10, 0);
}
