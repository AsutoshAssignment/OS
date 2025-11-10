#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/file.h>
#include "../include/employee.h"
#include "../include/data_structures.h"

extern double get_customer_balance(const char* username);
extern int update_password(const char* username, const char* new_password);
extern int verify_customer_exists(const char* username);
extern int add_new_customer(const char* username, const char* password, double initial_balance);
extern void log_transaction(const char* username, const char* type, double amount, const char* details);
extern int approve_loan(int loan_id);
extern int reject_loan(int loan_id);

ssize_t read_line_emp(int fd, char *buffer, size_t size) {
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

void employee_add_customer(int sock, const char* emp_username);
void employee_modify_customer(int sock, const char* emp_username);
void employee_process_loan(int sock, const char* emp_username);
void employee_view_assigned_loans(int sock, const char* emp_username);
void employee_view_customer_transactions(int sock, const char* emp_username);
void employee_change_password(int sock, const char* emp_username);

void handle_employee_operations(int client_socket, const char* username) {
    char buffer[100];
    int choice;
    
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int bytes = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes <= 0) return;
        
        choice = atoi(buffer);
        
        switch(choice) {
            case 1: employee_add_customer(client_socket, username); break;
            case 2: employee_modify_customer(client_socket, username); break;
            case 3: employee_process_loan(client_socket, username); break;
            case 4: employee_view_assigned_loans(client_socket, username); break;
            case 5: employee_view_customer_transactions(client_socket, username); break;
            case 6: employee_change_password(client_socket, username); break;
            case 7: return;
            default: send(client_socket, "Invalid choice!\nMENU_READY", 27, 0);
        }
    }
}

void employee_add_customer(int sock, const char* emp_username) {
    char buffer[100];
    char new_username[50], new_password[50];
    double initial_balance;
    
    send(sock, "Enter new customer username: ", 29, 0);
    memset(buffer, 0, sizeof(buffer));
    int bytes = recv(sock, buffer, sizeof(buffer), 0);
    if (bytes > 0) buffer[bytes] = '\0';
    sscanf(buffer, "%49s", new_username);
    
    if (verify_customer_exists(new_username)) {
        send(sock, "Username already exists!\nMENU_READY", 36, 0);
        return;
    }
    
    send(sock, "Enter password: ", 16, 0);
    memset(buffer, 0, sizeof(buffer));
    bytes = recv(sock, buffer, sizeof(buffer), 0);
    if (bytes > 0) buffer[bytes] = '\0';
    sscanf(buffer, "%49s", new_password);
    
    send(sock, "Enter initial balance: ", 23, 0);
    memset(buffer, 0, sizeof(buffer));
    bytes = recv(sock, buffer, sizeof(buffer), 0);
    if (bytes > 0) buffer[bytes] = '\0';
    initial_balance = atof(buffer);
    
    if (initial_balance < 0) {
        send(sock, "Invalid initial balance!\nMENU_READY", 36, 0);
        return;
    }
    
    int account_num = add_new_customer(new_username, new_password, initial_balance);
    if (account_num > 0) {
        char response[200];
        snprintf(response, sizeof(response),
                "Customer added successfully!\nUsername: %s\nAccount Number: %d\n"
                "Initial Balance: Rs%.2f\nMENU_READY",
                new_username, account_num, initial_balance);
        send(sock, response, strlen(response), 0);
        
        char details[150];
        snprintf(details, sizeof(details), "New customer %s added by %s", new_username, emp_username);
        log_transaction(emp_username, "ADD_CUSTOMER", 0, details);
    } else {
        send(sock, "Failed to add customer!\nMENU_READY", 35, 0);
    }
}

void employee_modify_customer(int sock, const char* emp_username) {
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
        send(sock, "Customer details modified successfully!\nMENU_READY", 53, 0);
        
        char details[150];
        snprintf(details, sizeof(details), "Modified customer %s", customer_username);
        log_transaction(emp_username, "MODIFY_CUSTOMER", 0, details);
    } else {
        send(sock, "Failed to modify customer!\nMENU_READY", 38, 0);
    }
}

void employee_process_loan(int sock, const char* emp_username) {
    char buffer[100];
    
    send(sock, "Enter loan ID to process: ", 26, 0);
    memset(buffer, 0, sizeof(buffer));
    recv(sock, buffer, sizeof(buffer), 0);
    int loan_id = atoi(buffer);
    
    int fd = open("data/loans.txt", O_RDONLY);
    if (fd == -1) {
        send(sock, "Failed to access loans database!\nMENU_READY", 43, 0);
        return;
    }
    
    char line[300];
    int loan_found = 0;
    int assigned_to_employee = 0;
    char loan_status[20] = {0};
    
    while (read_line_emp(fd, line, sizeof(line)) > 0) {
        int current_id;
        char customer[50], status[20], timestamp[100], assigned_to[50];
        double amount, emi;
        int tenure;
        
        int parsed = sscanf(line, "%d|%49[^|]|%lf|%d|%lf|%19[^|]|%99[^|]|%49[^\n]",
                           &current_id, customer, &amount, &tenure, &emi, 
                           status, timestamp, assigned_to);
        
        if (parsed >= 8 && current_id == loan_id) {
            loan_found = 1;
            strcpy(loan_status, status);
            
            if (strcmp(assigned_to, emp_username) == 0) {
                assigned_to_employee = 1;
            }
            break;
        }
    }
    
    close(fd);
    
    if (!loan_found) {
        send(sock, "Loan ID not found!\nMENU_READY", 30, 0);
        return;
    }
    
    if (!assigned_to_employee) {
        send(sock, "Access Denied! This loan is not assigned to you.\nMENU_READY", 59, 0);
        return;
    }
    
    if (strcmp(loan_status, "PENDING") != 0) {
        char response[150];
        snprintf(response, sizeof(response), 
                "Loan already processed! Current status: %s\nMENU_READY", loan_status);
        send(sock, response, strlen(response), 0);
        return;
    }
    
    send(sock, "Approve or Reject? (A/R): ", 26, 0);
    memset(buffer, 0, sizeof(buffer));
    recv(sock, buffer, sizeof(buffer), 0);
    
    if (buffer[0] == 'A' || buffer[0] == 'a') {
        if (approve_loan(loan_id) == 0) {
            send(sock, "Loan approved successfully!\nMENU_READY", 42, 0);
            char details[50];
            snprintf(details, sizeof(details), "Loan ID: %d", loan_id);
            log_transaction(emp_username, "LOAN_APPROVED", 0, details);
        } else {
            send(sock, "Failed to approve loan!\nMENU_READY", 35, 0);
        }
    } else if (buffer[0] == 'R' || buffer[0] == 'r') {
        if (reject_loan(loan_id) == 0) {
            send(sock, "Loan rejected successfully!\nMENU_READY", 42, 0);
            char details[50];
            snprintf(details, sizeof(details), "Loan ID: %d", loan_id);
            log_transaction(emp_username, "LOAN_REJECTED", 0, details);
        } else {
            send(sock, "Failed to reject loan!\nMENU_READY", 34, 0);
        }
    } else {
        send(sock, "Invalid choice!\nMENU_READY", 27, 0);
    }
}

void employee_view_assigned_loans(int sock, const char* emp_username) {
    int fd = open("data/loans.txt", O_RDONLY);
    if (fd == -1) {
        send(sock, "No loans found\nMENU_READY", 26, 0);
        return;
    }
    
    char line[300];
    char response[4096] = "=== Loans Assigned to You ===\n";
    int count = 0;
    
    while (read_line_emp(fd, line, sizeof(line)) > 0) {
        if (strstr(line, emp_username)) {
            strcat(response, line);
            count++;
        }
    }
    
    close(fd);
    if (count == 0) strcpy(response, "No loans assigned to you\n");
    strcat(response, "\nMENU_READY");
    send(sock, response, strlen(response), 0);
}

void employee_view_customer_transactions(int sock, const char* emp_username) {
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
    
    int fd = open("data/transactions.txt", O_RDONLY);
    if (fd == -1) {
        send(sock, "No transactions found\nMENU_READY", 33, 0);
        return;
    }
    
    char line[500];
    char response[4096];
    snprintf(response, sizeof(response), "=== Transactions for %s ===\n", customer_username);
    int count = 0;
    
    while (read_line_emp(fd, line, sizeof(line)) > 0) {
        if (strstr(line, customer_username)) {
            strcat(response, line);
            count++;
        }
    }
    
    close(fd);
    if (count == 0) strcpy(response, "No transactions found\n");
    strcat(response, "\nMENU_READY");
    send(sock, response, strlen(response), 0);
}

void employee_change_password(int sock, const char* emp_username) {
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
    
    if (update_password(emp_username, new_password) == 0) {
        send(sock, "Password changed successfully!\nMENU_READY", 45, 0);
        log_transaction(emp_username, "PASSWORD_CHANGE", 0, "Password updated");
    } else {
        send(sock, "Failed to change password!\nMENU_READY", 38, 0);
    }
}
