#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <stdlib.h>
#include <fcntl.h>
#include "../include/manager.h"
#include "../include/data_structures.h"

extern double get_customer_balance(const char* username);
extern int update_password(const char* username, const char* new_password);
extern int verify_customer_exists(const char* username);
extern int verify_user_exists(const char* username);
extern int freeze_account(const char* username);
extern int unfreeze_account(const char* username);
extern void log_transaction(const char* username, const char* type, double amount, const char* details);

ssize_t read_line_mgr(int fd, char *buffer, size_t size) {
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

void manager_view_all_customers(int sock, const char* mgr_username);
void manager_search_customer(int sock, const char* mgr_username);
void manager_modify_customer(int sock, const char* mgr_username);
void manager_view_feedback(int sock, const char* mgr_username);
void manager_change_password(int sock, const char* mgr_username);
void manager_generate_reports(int sock, const char* mgr_username);
void manager_freeze_unfreeze_account(int sock, const char* mgr_username);
void manager_assign_loan_to_employee(int sock, const char* mgr_username);

void handle_manager_operations(int client_socket, const char* username) {
    char buffer[100];
    int choice;
    
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int bytes = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes <= 0) return;
        
        choice = atoi(buffer);
        
        switch(choice) {
            case 1: manager_view_all_customers(client_socket, username); break;
            case 2: manager_search_customer(client_socket, username); break;
            case 3: manager_modify_customer(client_socket, username); break;
            case 4: manager_view_feedback(client_socket, username); break;
            case 5: manager_change_password(client_socket, username); break;
            case 6: manager_generate_reports(client_socket, username); break;
            case 7: manager_freeze_unfreeze_account(client_socket, username); break;
            case 8: manager_assign_loan_to_employee(client_socket, username); break;
            case 9: return;
            default: send(client_socket, "Invalid choice!\nMENU_READY", 27, 0);
        }
    }
}

void manager_view_all_customers(int sock, const char* mgr_username) {
    int fd = open("data/customers.txt", O_RDONLY);
    if (fd == -1) {
        send(sock, "No customers found\nMENU_READY", 30, 0);
        return;
    }
    
    char line[200];
    char response[4096] = "=== All Customers ===\n";
    
    while (read_line_mgr(fd, line, sizeof(line)) > 0) {
        if (strstr(line, "customer")) {
            strcat(response, line);
        }
    }
    
    close(fd);
    strcat(response, "\nMENU_READY");
    send(sock, response, strlen(response), 0);
}

void manager_search_customer(int sock, const char* mgr_username) {
    char buffer[100];
    char search_username[50];
    
    send(sock, "Enter customer username: ", 25, 0);
    memset(buffer, 0, sizeof(buffer));
    recv(sock, buffer, sizeof(buffer), 0);
    sscanf(buffer, "%49s", search_username);
    
    if (!verify_customer_exists(search_username)) {
        send(sock, "Customer not found!\nMENU_READY", 31, 0);
        return;
    }
    
    double balance = get_customer_balance(search_username);
    char response[300];
    snprintf(response, sizeof(response),
            "=== Customer Details ===\nUsername: %s\nBalance: Rs%.2f\nMENU_READY",
            search_username, balance);
    send(sock, response, strlen(response), 0);
}

void manager_modify_customer(int sock, const char* mgr_username) {
    char buffer[100];
    char customer_username[50];
    
    send(sock, "Enter customer username: ", 25, 0);
    memset(buffer, 0, sizeof(buffer));
    recv(sock, buffer, sizeof(buffer), 0);
    sscanf(buffer, "%49s", customer_username);
    
    if (!verify_customer_exists(customer_username)) {
        send(sock, "Customer not found!\nMENU_READY", 31, 0);
        return;
    }
    
    send(sock, "Enter new password: ", 20, 0);
    memset(buffer, 0, sizeof(buffer));
    recv(sock, buffer, sizeof(buffer), 0);
    
    char new_password[50];
    sscanf(buffer, "%49s", new_password);
    
    if (update_password(customer_username, new_password) == 0) {
        send(sock, "Customer modified successfully!\nMENU_READY", 46, 0);
        
        char details[150];
        snprintf(details, sizeof(details), "Modified customer %s", customer_username);
        log_transaction(mgr_username, "MODIFY_CUSTOMER", 0, details);
    } else {
        send(sock, "Failed to modify customer!\nMENU_READY", 38, 0);
    }
}

void manager_view_feedback(int sock, const char* mgr_username) {
    int fd = open("data/feedback.txt", O_RDONLY);
    if (fd == -1) {
        send(sock, "No feedback available\nMENU_READY", 33, 0);
        return;
    }
    
    char line[500];
    char response[4096] = "=== Customer Feedback ===\n";
    
    while (read_line_mgr(fd, line, sizeof(line)) > 0) {
        strcat(response, line);
    }
    
    close(fd);
    strcat(response, "\nMENU_READY");
    send(sock, response, strlen(response), 0);
}

void manager_change_password(int sock, const char* mgr_username) {
    char buffer[100];
    char new_password[50];
    
    send(sock, "Enter new password: ", 20, 0);
    memset(buffer, 0, sizeof(buffer));
    recv(sock, buffer, sizeof(buffer), 0);
    sscanf(buffer, "%49s", new_password);
    
    if (strlen(new_password) < 4) {
        send(sock, "Password too short (min 4 characters)!\nMENU_READY", 50, 0);
        return;
    }
    
    if (update_password(mgr_username, new_password) == 0) {
        send(sock, "Password changed successfully!\nMENU_READY", 45, 0);
        log_transaction(mgr_username, "PASSWORD_CHANGE", 0, "Password updated");
    } else {
        send(sock, "Failed to change password!\nMENU_READY", 38, 0);
    }
}

void manager_generate_reports(int sock, const char* mgr_username) {
    int fd = open("data/transactions.txt", O_RDONLY);
    if (fd == -1) {
        send(sock, "No transactions available\nMENU_READY", 37, 0);
        return;
    }
    
    char line[500];
    char response[4096] = "=== Transaction Report ===\n";
    int count = 0;
    
    while (read_line_mgr(fd, line, sizeof(line)) > 0 && count < 20) {
        strcat(response, line);
        count++;
    }
    
    close(fd);
    if (count == 0) strcpy(response, "No transactions found\n");
    strcat(response, "\nMENU_READY");
    send(sock, response, strlen(response), 0);
}

void manager_freeze_unfreeze_account(int sock, const char* mgr_username) {
    char buffer[100];
    char customer_username[50];
    
    send(sock, "Enter customer username: ", 25, 0);
    memset(buffer, 0, sizeof(buffer));
    recv(sock, buffer, sizeof(buffer), 0);
    sscanf(buffer, "%49s", customer_username);
    
    if (!verify_customer_exists(customer_username)) {
        send(sock, "Customer not found!\nMENU_READY", 31, 0);
        return;
    }
    
    send(sock, "Freeze or Unfreeze? (F/U): ", 27, 0);
    memset(buffer, 0, sizeof(buffer));
    recv(sock, buffer, sizeof(buffer), 0);
    
    if (buffer[0] == 'F' || buffer[0] == 'f') {
        if (freeze_account(customer_username) == 0) {
            send(sock, "Account frozen successfully!\nMENU_READY", 43, 0);
            char details[150];
            snprintf(details, sizeof(details), "Frozen account %s", customer_username);
            log_transaction(mgr_username, "FREEZE_ACCOUNT", 0, details);
        } else {
            send(sock, "Failed to freeze account!\nMENU_READY", 37, 0);
        }
    } else if (buffer[0] == 'U' || buffer[0] == 'u') {
        if (unfreeze_account(customer_username) == 0) {
            send(sock, "Account unfrozen successfully!\nMENU_READY", 45, 0);
            char details[150];
            snprintf(details, sizeof(details), "Unfrozen account %s", customer_username);
            log_transaction(mgr_username, "UNFREEZE_ACCOUNT", 0, details);
        } else {
            send(sock, "Failed to unfreeze account!\nMENU_READY", 39, 0);
        }
    } else {
        send(sock, "Invalid choice!\nMENU_READY", 27, 0);
    }
}

void manager_assign_loan_to_employee(int sock, const char* mgr_username) {
    char buffer[100];
    
    send(sock, "Enter loan ID to assign: ", 25, 0);
    memset(buffer, 0, sizeof(buffer));
    recv(sock, buffer, sizeof(buffer), 0);
    int loan_id = atoi(buffer);
    
    send(sock, "Enter employee username: ", 25, 0);
    memset(buffer, 0, sizeof(buffer));
    recv(sock, buffer, sizeof(buffer), 0);
    char employee[50];
    sscanf(buffer, "%49s", employee);
    
    if (!verify_user_exists(employee)) {
        send(sock, "Employee not found!\nMENU_READY", 31, 0);
        return;
    }
    
    int fd = open("data/loans.txt", O_RDONLY);
    if (fd == -1) {
        send(sock, "No loans found\nMENU_READY", 26, 0);
        return;
    }
    
    int temp_fd = open("data/loans.tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (temp_fd == -1) {
        close(fd);
        send(sock, "Error creating temp file\nMENU_READY", 36, 0);
        return;
    }
    
    char line[300];
    int found = 0;
    
    while (read_line_mgr(fd, line, sizeof(line)) > 0) {
        int current_id;
        char customer[50], status[20], timestamp[100], assigned_to[50];
        double amount, emi;
        int tenure;
        
        int parsed = sscanf(line, "%d|%49[^|]|%lf|%d|%lf|%19[^|]|%99[^|]|%49[^\n]",
                           &current_id, customer, &amount, &tenure, &emi, status, timestamp, assigned_to);
        
        if (parsed >= 6 && current_id == loan_id) {
            char output[300];
            int len = snprintf(output, sizeof(output), "%d|%s|%.2f|%d|%.2f|%s|%s|%s\n",
                             current_id, customer, amount, tenure, emi, status, timestamp, employee);
            write(temp_fd, output, len);
            found = 1;
        } else {
            write(temp_fd, line, strlen(line));
        }
    }
    
    close(fd);
    close(temp_fd);
    
    if (found) {
        rename("data/loans.tmp", "data/loans.txt");
        char response[100];
        snprintf(response, sizeof(response),
                "Loan %d assigned to %s successfully!\nMENU_READY", loan_id, employee);
        send(sock, response, strlen(response), 0);
        
        char details[150];
        snprintf(details, sizeof(details),
                "Loan %d assigned to employee %s", loan_id, employee);
        log_transaction(mgr_username, "LOAN_ASSIGNED", 0, details);
    } else {
        unlink("data/loans.tmp");
        send(sock, "Loan ID not found!\nMENU_READY", 30, 0);
    }
}
