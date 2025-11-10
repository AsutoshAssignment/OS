#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <time.h>

// User structure
typedef struct {
    char username[50];
    char password[50];
    char role[20];  // customer, employee, manager, admin
    double balance;
    char status[20]; // active, frozen
    int account_number;
} User;

// Session structure
typedef struct {
    char username[50];
    int process_id;
    time_t login_time;
    int is_active;
} Session;

// Loan structure
typedef struct {
    int loan_id;
    char customer_username[50];
    double amount;
    int tenure_months;
    double emi;
    char status[20]; // PENDING, APPROVED, REJECTED
    char timestamp[100];
    char assigned_to[50]; // employee username
} Loan;

// Transaction structure
typedef struct {
    char timestamp[100];
    char username[50];
    char type[30];
    double amount;
    char details[200];
} Transaction;

// Feedback structure
typedef struct {
    char timestamp[100];
    char username[50];
    char message[500];
} Feedback;

#endif
