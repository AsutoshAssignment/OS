#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/file.h>
#include <time.h>
#include <math.h>
#include "../include/customer.h"
#include "../include/data_structures.h"

extern double get_customer_balance(const char* username);
extern int atomic_deposit(const char* username, double amount);
extern int atomic_withdraw(const char* username, double amount);
extern int update_password(const char* username, const char* new_password);
extern int verify_customer_exists(const char* username);
extern void log_transaction(const char* username, const char* type, double amount, const char* details);
extern int get_next_loan_id(void);
extern int is_account_frozen(const char* username);

void customer_view_balance(int sock, const char* username);
void customer_deposit(int sock, const char* username);
void customer_withdraw(int sock, const char* username);
void customer_transfer(int sock, const char* username);
void customer_apply_loan(int sock, const char* username);
void customer_change_password(int sock, const char* username);
void customer_add_feedback(int sock, const char* username);
void customer_view_transactions(int sock, const char* username);
void customer_logout(int sock, const char* username);

void handle_customer_operations(int client_socket, const char* username) {
    char buffer[100];
    int choice;
    
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int bytes = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes <= 0) return;
        
        choice = atoi(buffer);
        
        switch(choice) {
            case 1: customer_view_balance(client_socket, username); break;
            case 2: customer_deposit(client_socket, username); break;
            case 3: customer_withdraw(client_socket, username); break;
            case 4: customer_transfer(client_socket, username); break;
            case 5: customer_apply_loan(client_socket, username); break;
            case 6: customer_change_password(client_socket, username); break;
            case 7: customer_add_feedback(client_socket, username); break;
            case 8: customer_view_transactions(client_socket, username); break;
            case 9: return;
            default: send(client_socket, "Invalid choice!\nMENU_READY", 27, 0);
        }
    }
}

void customer_view_balance(int sock, const char* username) {
    double balance = get_customer_balance(username);
    
    if (balance < 0) {
        send(sock, "Failed to retrieve balance!\nMENU_READY", 39, 0);
        return;
    }
    
    char response[200];
    snprintf(response, sizeof(response),
            "=== Account Balance ===\nCurrent Balance: Rs.%.2f\nMENU_READY",
            balance);
    send(sock, response, strlen(response), 0);
}

void customer_deposit(int sock, const char* username) {
    char buffer[100];
    
    if (is_account_frozen(username)) {
        send(sock, "Account is FROZEN! Contact your bank.\nMENU_READY", 49, 0);
        return;
    }
    
    send(sock, "Enter amount to deposit: ", 25, 0);
    memset(buffer, 0, sizeof(buffer));
    int bytes = recv(sock, buffer, sizeof(buffer), 0);
    if (bytes <= 0) {
        send(sock, "MENU_READY", 10, 0);
        return;
    }
    
    double amount = atof(buffer);
    if (amount <= 0) {
        send(sock, "Invalid amount!\nMENU_READY", 27, 0);
        return;
    }
    
    if (atomic_deposit(username, amount) == 0) {
        double new_balance = get_customer_balance(username);
        char response[200];
        snprintf(response, sizeof(response),
                "Deposit successful!\nAmount: Rs.%.2f\nNew Balance: Rs.%.2f\nMENU_READY",
                amount, new_balance);
        send(sock, response, strlen(response), 0);
        log_transaction(username, "DEPOSIT", amount, "Cash deposit");
    } else {
        send(sock, "Deposit failed!\nMENU_READY", 27, 0);
    }
}

void customer_withdraw(int sock, const char* username) {
    char buffer[100];
    
    if (is_account_frozen(username)) {
        send(sock, "Account is FROZEN! Contact your bank.\nMENU_READY", 49, 0);
        return;
    }
    
    send(sock, "Enter amount to withdraw: ", 26, 0);
    memset(buffer, 0, sizeof(buffer));
    int bytes = recv(sock, buffer, sizeof(buffer), 0);
    if (bytes <= 0) {
        send(sock, "MENU_READY", 10, 0);
        return;
    }
    
    double amount = atof(buffer);
    if (amount <= 0) {
        send(sock, "Invalid amount!\nMENU_READY", 27, 0);
        return;
    }
    
    int result = atomic_withdraw(username, amount);
    if (result == 0) {
        double new_balance = get_customer_balance(username);
        char response[200];
        snprintf(response, sizeof(response),
                "Withdrawal successful!\nAmount: Rs.%.2f\nNew Balance: Rs.%.2f\nMENU_READY",
                amount, new_balance);
        send(sock, response, strlen(response), 0);
        log_transaction(username, "WITHDRAW", amount, "Cash withdrawal");
    } else if (result == -2) {
        send(sock, "Insufficient balance!\nMENU_READY", 33, 0);
    } else {
        send(sock, "Withdrawal failed!\nMENU_READY", 30, 0);
    }
}

void customer_transfer(int sock, const char* username) {
    char buffer[100];
    char target_user[50];
    
    if (is_account_frozen(username)) {
        send(sock, "Account is FROZEN! Contact your bank.\nMENU_READY", 49, 0);
        return;
    }
    
    send(sock, "Enter recipient username: ", 26, 0);
    memset(buffer, 0, sizeof(buffer));
    int bytes = recv(sock, buffer, sizeof(buffer), 0);
    if (bytes <= 0) {
        send(sock, "MENU_READY", 10, 0);
        return;
    }
    
    sscanf(buffer, "%49s", target_user);
    
    if (!verify_customer_exists(target_user)) {
        send(sock, "Recipient does not exist!\nMENU_READY", 37, 0);
        return;
    }
    
    if (strcmp(username, target_user) == 0) {
        send(sock, "Cannot transfer to yourself!\nMENU_READY", 40, 0);
        return;
    }
    
    send(sock, "Enter amount to transfer: ", 26, 0);
    memset(buffer, 0, sizeof(buffer));
    bytes = recv(sock, buffer, sizeof(buffer), 0);
    if (bytes <= 0) {
        send(sock, "MENU_READY", 10, 0);
        return;
    }
    
    double amount = atof(buffer);
    if (amount <= 0) {
        send(sock, "Invalid amount!\nMENU_READY", 27, 0);
        return;
    }
    
    int withdraw_result = atomic_withdraw(username, amount);
    if (withdraw_result == 0) {
        if (atomic_deposit(target_user, amount) == 0) {
            double new_balance = get_customer_balance(username);
            char response[200];
            snprintf(response, sizeof(response),
                    "Transfer successful!\nTo: %s\nAmount: Rs.%.2f\nNew Balance: Rs.%.2f\nMENU_READY",
                    target_user, amount, new_balance);
            send(sock, response, strlen(response), 0);
            
            char details[150];
            snprintf(details, sizeof(details), "Transfer to %s", target_user);
            log_transaction(username, "TRANSFER_OUT", amount, details);
            snprintf(details, sizeof(details), "Transfer from %s", username);
            log_transaction(target_user, "TRANSFER_IN", amount, details);
        } else {
            atomic_deposit(username, amount); // Refund
            send(sock, "Transfer failed! Amount refunded.\nMENU_READY", 45, 0);
        }
    } else if (withdraw_result == -2) {
        send(sock, "Insufficient balance!\nMENU_READY", 33, 0);
    } else {
        send(sock, "Transfer failed!\nMENU_READY", 28, 0);
    }
}

void customer_apply_loan(int sock, const char* username) {
    char buffer[100];
    
    if (is_account_frozen(username)) {
        send(sock, "Account is FROZEN! Contact your bank.\nMENU_READY", 49, 0);
        return;
    }
    
    const char* prompt1 = "Enter loan amount: ";
    send(sock, prompt1, strlen(prompt1), MSG_NOSIGNAL);
    
    memset(buffer, 0, sizeof(buffer));
    int bytes = recv(sock, buffer, sizeof(buffer), 0);
    if (bytes <= 0) {
        send(sock, "MENU_READY", 10, 0);
        return;
    }
    
    buffer[bytes] = '\0';
    double amount = atof(buffer);
    
    if (amount <= 0 || amount > 100000) {
        send(sock, "Invalid loan amount (min: Rs.1, max: Rs.100,000)\nMENU_READY", 56, 0);
        return;
    }
    
    const char* prompt2 = "Enter tenure (months 6-60): ";
    send(sock, prompt2, strlen(prompt2), MSG_NOSIGNAL);
    
    memset(buffer, 0, sizeof(buffer));
    bytes = recv(sock, buffer, sizeof(buffer), 0);
    if (bytes <= 0) {
        send(sock, "MENU_READY", 10, 0);
        return;
    }
    
    buffer[bytes] = '\0';
    int tenure = atoi(buffer);
    
    if (tenure < 6 || tenure > 60) {
        send(sock, "Invalid tenure (6-60 months only)\nMENU_READY", 45, 0);
        return;
    }
    
    double monthly_rate = 8.5 / 12.0 / 100.0;
    double emi = (amount * monthly_rate * pow(1 + monthly_rate, tenure)) /
                 (pow(1 + monthly_rate, tenure) - 1);
    
    int loan_id = get_next_loan_id();
    int fd = open("data/loans.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
    
    if (fd != -1) {
        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = 0;
        lock.l_len = 0;
        fcntl(fd, F_SETLKW, &lock);
        
        time_t now = time(NULL);
        char *time_str = ctime(&now);
        time_str[strlen(time_str) - 1] = '\0';
        
        char loan_entry[300];
        int len = snprintf(loan_entry, sizeof(loan_entry),
                          "%d|%s|%.2f|%d|%.2f|PENDING|%s|unassigned\n",
                          loan_id, username, amount, tenure, emi, time_str);
        
        write(fd, loan_entry, len);
        
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);
        close(fd);
        
        char response[300];
        snprintf(response, sizeof(response),
                "Loan Application Submitted!\n"
                "Loan ID: %d\n"
                "Amount: Rs.%.2f\n"
                "Tenure: %d months\n"
                "EMI: Rs.%.2f\n"
                "Status: Pending\nMENU_READY",
                loan_id, amount, tenure, emi);
        send(sock, response, strlen(response), 0);
        
        log_transaction(username, "LOAN_APPLICATION", amount, "Loan applied");
    } else {
        send(sock, "Failed to submit loan application.\nMENU_READY", 46, 0);
    }
}

void customer_change_password(int sock, const char* username) {
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
    
    if (update_password(username, new_password) == 0) {
        send(sock, "Password changed successfully!\nMENU_READY", 45, 0);
        log_transaction(username, "PASSWORD_CHANGE", 0, "Password updated");
    } else {
        send(sock, "Failed to change password!\nMENU_READY", 38, 0);
    }
}

void customer_add_feedback(int sock, const char* username) {
    char buffer[500];
    
    send(sock, "Enter your feedback: ", 21, 0);
    memset(buffer, 0, sizeof(buffer));
    int bytes = recv(sock, buffer, sizeof(buffer), 0);
    if (bytes <= 0) {
        send(sock, "MENU_READY", 10, 0);
        return;
    }
    
    buffer[bytes] = '\0';
    
    if (strlen(buffer) < 5) {
        send(sock, "Feedback too short!\nMENU_READY", 31, 0);
        return;
    }
    
    int fd = open("data/feedback.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd != -1) {
        time_t now = time(NULL);
        char *time_str = ctime(&now);
        time_str[strlen(time_str) - 1] = '\0';
        
        char feedback_entry[600];
        int len = snprintf(feedback_entry, sizeof(feedback_entry),
                          "[%s] %s: %s\n", time_str, username, buffer);
        
        write(fd, feedback_entry, len);
        close(fd);
        
        send(sock, "Thank you for your feedback!\nMENU_READY", 40, 0);
        log_transaction(username, "FEEDBACK", 0, "Feedback submitted");
    } else {
        send(sock, "Failed to submit feedback.\nMENU_READY", 38, 0);
    }
}

void customer_view_transactions(int sock, const char* username) {
    int fd = open("data/transactions.txt", O_RDONLY);
    if (fd == -1) {
        send(sock, "No transactions found\nMENU_READY", 33, 0);
        return;
    }
    
    char line[500];
    char response[4096] = "=== Your Transaction History ===\n";
    int count = 0;
    
    while (read(fd, line, sizeof(line)) > 0) {
        if (strstr(line, username)) {
            strncat(response, line, sizeof(response) - strlen(response) - 1);
            count++;
            if (count >= 10) break;
        }
        memset(line, 0, sizeof(line));
    }
    
    close(fd);
    if (count == 0) strcpy(response, "No transactions found\n");
    strncat(response, "\nMENU_READY", sizeof(response) - strlen(response) - 1);
    send(sock, response, strlen(response), 0);
}
