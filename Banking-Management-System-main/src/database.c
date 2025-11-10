#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/file.h>
#include <fcntl.h>
#include <math.h>
#include <semaphore.h>
#include "../include/data_structures.h"

#define CUSTOMERS_FILE "data/customers.txt"
#define TRANSACTIONS_FILE "data/transactions.txt"
#define LOANS_FILE "data/loans.txt"

extern sem_t *balance_sem;

ssize_t read_line(int fd, char *buffer, size_t size) {
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

double get_customer_balance(const char* username) {
    int fd = open(CUSTOMERS_FILE, O_RDONLY);
    if (fd == -1) return -1.0;
    
    char line[200];
    User user;
    
    while (read_line(fd, line, sizeof(line)) > 0) {
        sscanf(line, "%49[^|]|%49[^|]|%19[^|]|%lf|%19[^|]|%d",
               user.username, user.password, user.role, &user.balance,
               user.status, &user.account_number);
        
        if (strcmp(username, user.username) == 0) {
            close(fd);
            return user.balance;
        }
    }
    
    close(fd);
    return -1.0;
}

int is_account_frozen(const char* username) {
    int fd = open(CUSTOMERS_FILE, O_RDONLY);
    if (fd == -1) return 0;
    
    char line[200];
    User user;
    
    while (read_line(fd, line, sizeof(line)) > 0) {
        sscanf(line, "%49[^|]|%49[^|]|%19[^|]|%lf|%19[^|]|%d",
               user.username, user.password, user.role, &user.balance,
               user.status, &user.account_number);
        
        if (strcmp(username, user.username) == 0) {
            close(fd);
            return (strcmp(user.status, "frozen") == 0) ? 1 : 0;
        }
    }
    
    close(fd);
    return 0;
}

int atomic_deposit(const char* username, double amount) {
    if (amount <= 0) return -1;
    
    sem_wait(balance_sem);
    
    int fd = open(CUSTOMERS_FILE, O_RDONLY);
    if (fd == -1) {
        sem_post(balance_sem);
        return -1;
    }
    
    int temp_fd = open("data/customers.tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (temp_fd == -1) {
        close(fd);
        sem_post(balance_sem);
        return -1;
    }
    
    flock(fd, LOCK_EX);
    
    char line[200];
    User user;
    int found = 0;
    
    while (read_line(fd, line, sizeof(line)) > 0) {
        sscanf(line, "%49[^|]|%49[^|]|%19[^|]|%lf|%19[^|]|%d",
               user.username, user.password, user.role, &user.balance,
               user.status, &user.account_number);
        
        if (strcmp(username, user.username) == 0) {
            user.balance += amount;
            found = 1;
        }
        
        char output[200];
        int len = snprintf(output, sizeof(output), "%s|%s|%s|%.2f|%s|%d\n",
                user.username, user.password, user.role, user.balance,
                user.status, user.account_number);
        write(temp_fd, output, len);
    }
    
    flock(fd, LOCK_UN);
    close(fd);
    close(temp_fd);
    
    if (found) {
        rename("data/customers.tmp", CUSTOMERS_FILE);
        sem_post(balance_sem);
        return 0;
    }
    
    unlink("data/customers.tmp");
    sem_post(balance_sem);
    return -1;
}

int atomic_withdraw(const char* username, double amount) {
    if (amount <= 0) return -1;
    
    sem_wait(balance_sem);
    
    int fd = open(CUSTOMERS_FILE, O_RDONLY);
    if (fd == -1) {
        sem_post(balance_sem);
        return -1;
    }
    
    int temp_fd = open("data/customers.tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (temp_fd == -1) {
        close(fd);
        sem_post(balance_sem);
        return -1;
    }
    
    flock(fd, LOCK_EX);
    
    char line[200];
    User user;
    int found = 0;
    int result = -1;
    
    while (read_line(fd, line, sizeof(line)) > 0) {
        sscanf(line, "%49[^|]|%49[^|]|%19[^|]|%lf|%19[^|]|%d",
               user.username, user.password, user.role, &user.balance,
               user.status, &user.account_number);
        
        if (strcmp(username, user.username) == 0) {
            found = 1;
            if (user.balance >= amount) {
                user.balance -= amount;
                result = 0;
            } else {
                result = -2;
            }
        }
        
        char output[200];
        int len = snprintf(output, sizeof(output), "%s|%s|%s|%.2f|%s|%d\n",
                user.username, user.password, user.role, user.balance,
                user.status, user.account_number);
        write(temp_fd, output, len);
    }
    
    flock(fd, LOCK_UN);
    close(fd);
    close(temp_fd);
    
    if (found && result == 0) {
        rename("data/customers.tmp", CUSTOMERS_FILE);
        sem_post(balance_sem);
        return 0;
    }
    
    unlink("data/customers.tmp");
    sem_post(balance_sem);
    return result;
}

int update_password(const char* username, const char* new_password) {
    int fd = open(CUSTOMERS_FILE, O_RDONLY);
    if (fd == -1) return -1;
    
    int temp_fd = open("data/customers.tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (temp_fd == -1) {
        close(fd);
        return -1;
    }
    
    flock(fd, LOCK_EX);
    
    char line[200];
    User user;
    int found = 0;
    
    while (read_line(fd, line, sizeof(line)) > 0) {
        sscanf(line, "%49[^|]|%49[^|]|%19[^|]|%lf|%19[^|]|%d",
               user.username, user.password, user.role, &user.balance,
               user.status, &user.account_number);
        
        if (strcmp(username, user.username) == 0) {
            strncpy(user.password, new_password, sizeof(user.password) - 1);
            found = 1;
        }
        
        char output[200];
        int len = snprintf(output, sizeof(output), "%s|%s|%s|%.2f|%s|%d\n",
                user.username, user.password, user.role, user.balance,
                user.status, user.account_number);
        write(temp_fd, output, len);
    }
    
    flock(fd, LOCK_UN);
    close(fd);
    close(temp_fd);
    
    if (found) {
        rename("data/customers.tmp", CUSTOMERS_FILE);
        return 0;
    }
    
    unlink("data/customers.tmp");
    return -1;
}

int verify_customer_exists(const char* username) {
    int fd = open(CUSTOMERS_FILE, O_RDONLY);
    if (fd == -1) return 0;
    
    char line[200];
    User user;
    
    while (read_line(fd, line, sizeof(line)) > 0) {
        sscanf(line, "%49[^|]|%49[^|]|%19[^|]|%lf|%19[^|]|%d",
               user.username, user.password, user.role, &user.balance,
               user.status, &user.account_number);
        
        if (strcmp(username, user.username) == 0 && strcmp(user.role, "customer") == 0) {
            close(fd);
            return 1;
        }
    }
    
    close(fd);
    return 0;
}

int verify_user_exists(const char* username) {
    int fd = open(CUSTOMERS_FILE, O_RDONLY);
    if (fd == -1) return 0;
    
    char line[200];
    User user;
    
    while (read_line(fd, line, sizeof(line)) > 0) {
        sscanf(line, "%49[^|]|%49[^|]|%19[^|]|%lf|%19[^|]|%d",
               user.username, user.password, user.role, &user.balance,
               user.status, &user.account_number);
        
        if (strcmp(username, user.username) == 0) {
            close(fd);
            return 1;
        }
    }
    
    close(fd);
    return 0;
}

int get_next_account_number(void) {
    int fd = open(CUSTOMERS_FILE, O_RDONLY);
    if (fd == -1) return 1001;
    
    char line[200];
    User user;
    int max_account = 1000;
    
    while (read_line(fd, line, sizeof(line)) > 0) {
        sscanf(line, "%49[^|]|%49[^|]|%19[^|]|%lf|%19[^|]|%d",
               user.username, user.password, user.role, &user.balance,
               user.status, &user.account_number);
        
        if (user.account_number > max_account) {
            max_account = user.account_number;
        }
    }
    
    close(fd);
    return max_account + 1;
}

int get_next_loan_id(void) {
    int fd = open(LOANS_FILE, O_RDONLY);
    if (fd == -1) return 5001;
    
    char line[300];
    int max_loan_id = 5000;
    int loan_id;
    
    while (read_line(fd, line, sizeof(line)) > 0) {
        if (sscanf(line, "%d|", &loan_id) == 1) {
            if (loan_id > max_loan_id) {
                max_loan_id = loan_id;
            }
        }
    }
    
    close(fd);
    return max_loan_id + 1;
}

void log_transaction(const char* username, const char* type, double amount, const char* details) {
    int fd = open(TRANSACTIONS_FILE, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1) return;
    
    flock(fd, LOCK_EX);
    
    time_t now = time(NULL);
    char *time_str = ctime(&now);
    time_str[strlen(time_str) - 1] = '\0';
    
    char log_entry[500];
    int len = snprintf(log_entry, sizeof(log_entry), "%s | %s | %s | Rs%.2f | %s\n",
            time_str, username, type, amount, details);
    write(fd, log_entry, len);
    
    flock(fd, LOCK_UN);
    close(fd);
}

int freeze_account(const char* username) {
    int fd = open(CUSTOMERS_FILE, O_RDONLY);
    if (fd == -1) return -1;
    
    int temp_fd = open("data/customers.tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (temp_fd == -1) {
        close(fd);
        return -1;
    }
    
    char line[200];
    User user;
    int found = 0;
    
    while (read_line(fd, line, sizeof(line)) > 0) {
        sscanf(line, "%49[^|]|%49[^|]|%19[^|]|%lf|%19[^|]|%d",
               user.username, user.password, user.role, &user.balance,
               user.status, &user.account_number);
        
        if (strcmp(username, user.username) == 0) {
            strcpy(user.status, "frozen");
            found = 1;
        }
        
        char output[200];
        int len = snprintf(output, sizeof(output), "%s|%s|%s|%.2f|%s|%d\n",
                user.username, user.password, user.role, user.balance,
                user.status, user.account_number);
        write(temp_fd, output, len);
    }
    
    close(fd);
    close(temp_fd);
    
    if (found) {
        rename("data/customers.tmp", CUSTOMERS_FILE);
        return 0;
    }
    
    unlink("data/customers.tmp");
    return -1;
}

int unfreeze_account(const char* username) {
    int fd = open(CUSTOMERS_FILE, O_RDONLY);
    if (fd == -1) return -1;
    
    int temp_fd = open("data/customers.tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (temp_fd == -1) {
        close(fd);
        return -1;
    }
    
    char line[200];
    User user;
    int found = 0;
    
    while (read_line(fd, line, sizeof(line)) > 0) {
        sscanf(line, "%49[^|]|%49[^|]|%19[^|]|%lf|%19[^|]|%d",
               user.username, user.password, user.role, &user.balance,
               user.status, &user.account_number);
        
        if (strcmp(username, user.username) == 0) {
            strcpy(user.status, "active");
            found = 1;
        }
        
        char output[200];
        int len = snprintf(output, sizeof(output), "%s|%s|%s|%.2f|%s|%d\n",
                user.username, user.password, user.role, user.balance,
                user.status, user.account_number);
        write(temp_fd, output, len);
    }
    
    close(fd);
    close(temp_fd);
    
    if (found) {
        rename("data/customers.tmp", CUSTOMERS_FILE);
        return 0;
    }
    
    unlink("data/customers.tmp");
    return -1;
}

int approve_loan(int loan_id) {
    int fd = open(LOANS_FILE, O_RDONLY);
    if (fd == -1) return -1;
    
    int temp_fd = open("data/loans.tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (temp_fd == -1) {
        close(fd);
        return -1;
    }
    
    char line[300];
    Loan loan;
    int found = 0;
    
    while (read_line(fd, line, sizeof(line)) > 0) {
        sscanf(line, "%d|%49[^|]|%lf|%d|%lf|%19[^|]|%99[^|]|%49[^\n]",
               &loan.loan_id, loan.customer_username, &loan.amount,
               &loan.tenure_months, &loan.emi, loan.status,
               loan.timestamp, loan.assigned_to);
        
        if (loan.loan_id == loan_id) {
            strcpy(loan.status, "APPROVED");
            found = 1;
        }
        
        char output[300];
        int len = snprintf(output, sizeof(output), "%d|%s|%.2f|%d|%.2f|%s|%s|%s\n",
                loan.loan_id, loan.customer_username, loan.amount,
                loan.tenure_months, loan.emi, loan.status,
                loan.timestamp, loan.assigned_to);
        write(temp_fd, output, len);
    }
    
    close(fd);
    close(temp_fd);
    
    if (found) {
        rename("data/loans.tmp", LOANS_FILE);
        return 0;
    }
    
    unlink("data/loans.tmp");
    return -1;
}

int reject_loan(int loan_id) {
    int fd = open(LOANS_FILE, O_RDONLY);
    if (fd == -1) return -1;
    
    int temp_fd = open("data/loans.tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (temp_fd == -1) {
        close(fd);
        return -1;
    }
    
    char line[300];
    Loan loan;
    int found = 0;
    
    while (read_line(fd, line, sizeof(line)) > 0) {
        sscanf(line, "%d|%49[^|]|%lf|%d|%lf|%19[^|]|%99[^|]|%49[^\n]",
               &loan.loan_id, loan.customer_username, &loan.amount,
               &loan.tenure_months, &loan.emi, loan.status,
               loan.timestamp, loan.assigned_to);
        
        if (loan.loan_id == loan_id) {
            strcpy(loan.status, "REJECTED");
            found = 1;
        }
        
        char output[300];
        int len = snprintf(output, sizeof(output), "%d|%s|%.2f|%d|%.2f|%s|%s|%s\n",
                loan.loan_id, loan.customer_username, loan.amount,
                loan.tenure_months, loan.emi, loan.status,
                loan.timestamp, loan.assigned_to);
        write(temp_fd, output, len);
    }
    
    close(fd);
    close(temp_fd);
    
    if (found) {
        rename("data/loans.tmp", LOANS_FILE);
        return 0;
    }
    
    unlink("data/loans.tmp");
    return -1;
}

int add_new_customer(const char* username, const char* password, double initial_balance) {
    if (verify_user_exists(username)) return -1;
    
    int fd = open(CUSTOMERS_FILE, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1) return -1;
    
    flock(fd, LOCK_EX);
    
    int account_num = get_next_account_number();
    
    char output[200];
    int len = snprintf(output, sizeof(output), "%s|%s|customer|%.2f|active|%d\n",
            username, password, initial_balance, account_num);
    write(fd, output, len);
    
    flock(fd, LOCK_UN);
    close(fd);
    
    return account_num;
}

int change_user_role(const char* username, const char* new_role) {
    int fd = open(CUSTOMERS_FILE, O_RDONLY);
    if (fd == -1) return -1;
    
    int temp_fd = open("data/customers.tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (temp_fd == -1) {
        close(fd);
        return -1;
    }
    
    char line[200];
    User user;
    int found = 0;
    
    while (read_line(fd, line, sizeof(line)) > 0) {
        sscanf(line, "%49[^|]|%49[^|]|%19[^|]|%lf|%19[^|]|%d",
               user.username, user.password, user.role, &user.balance,
               user.status, &user.account_number);
        
        if (strcmp(username, user.username) == 0) {
            strncpy(user.role, new_role, sizeof(user.role) - 1);
            found = 1;
        }
        
        char output[200];
        int len = snprintf(output, sizeof(output), "%s|%s|%s|%.2f|%s|%d\n",
                user.username, user.password, user.role, user.balance,
                user.status, user.account_number);
        write(temp_fd, output, len);
    }
    
    close(fd);
    close(temp_fd);
    
    if (found) {
        rename("data/customers.tmp", CUSTOMERS_FILE);
        return 0;
    }
    
    unlink("data/customers.tmp");
    return -1;
}
int authenticate_user(const char* username, const char* password, char* role) {
    int fd = open(CUSTOMERS_FILE, O_RDONLY);
    if (fd == -1) return -1;
    
    char line[200];
    User user;
    
    while (read_line(fd, line, sizeof(line)) > 0) {
        sscanf(line, "%49[^|]|%49[^|]|%19[^|]|%lf|%19[^|]|%d",
               user.username, user.password, user.role, &user.balance,
               user.status, &user.account_number);
        
        if (strcmp(username, user.username) == 0 && 
            strcmp(password, user.password) == 0) {
            strcpy(role, user.role);
            close(fd);
            return 0;
        }
    }
    
    close(fd);
    return -1;
}
