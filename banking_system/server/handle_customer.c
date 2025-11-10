#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "customer.h"
#include "../data/user.h"
#include "../data/loan.h"
#include "../data/transaction.h"
#include "../data/account.h"
#include "../data/feedback.h"

void handle_customer(int client_sock, struct User *u) {
    char buffer[512];

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int n = read(client_sock, buffer, sizeof(buffer) - 1);
        if (n <= 0) break;
        buffer[n] = '\0';

        // Trim trailing newline if any
        buffer[strcspn(buffer, "\n")] = '\0';

        // 1️⃣ VIEW ACCOUNT
        if (strncmp(buffer, "VIEW_ACC", 8) == 0) {
            view_account_details(u->id, client_sock);
        }

        // 2️⃣ DEPOSIT
        else if (strncmp(buffer, "DEPOSIT|", 8) == 0) {
            float amt;
            sscanf(buffer + 8, "%f", &amt);
            if (deposit_money(u->id, amt))
                write(client_sock, "Deposit successful.\n", 21);
            else
                write(client_sock, "Deposit failed.\n", 17);
        }

        
        else if (strncmp(buffer, "WITHDRAW|", 9) == 0) {
            float amt;
            sscanf(buffer + 9, "%f", &amt);
            int res = withdraw_money(u->id, amt);
            if (res == 1)
                write(client_sock, "Withdrawal successful.\n", 23);
            else if (res == -1)
                write(client_sock, "Insufficient funds.\n", 21);
            else
                write(client_sock, "Withdrawal failed.\n", 20);
        }

        
        else if (strncmp(buffer, "TRANSFER", 8) == 0) {
            int target_id;
            float amt;
            sscanf(buffer + 8, "%d %f", &target_id, &amt);

            struct Account from_acc, to_acc;
            if (!get_account_by_userid(u->id, &from_acc) ||
                !get_account_by_userid(target_id, &to_acc)) {
                write(client_sock, "Invalid account(s).\n", 21);
            } else if (from_acc.balance < amt) {
                write(client_sock, "Insufficient funds.\n", 21);
            } else {
                withdraw_money(u->id, amt);
                deposit_money(target_id, amt);
                log_transaction(u->id, target_id, amt, "transfer");
                write(client_sock, "Transfer successful.\n", 22);
            }
        }

      
        else if (strncmp(buffer, "LOAN|", 5) == 0) {
            float amt;
            sscanf(buffer + 5, "%f", &amt);
            apply_for_loan(u->id, amt);
            write(client_sock, "Loan application submitted.\n", 29);
        }

      
        else if (strncmp(buffer, "CHPASS", 6) == 0) {
            char old_pass[50], new_pass[50];
            sscanf(buffer + 6, "%49s %49s", old_pass, new_pass);

            if (change_password(u->id, old_pass, new_pass))
                write(client_sock, "Password updated successfully.\n", 32);
            else
                write(client_sock, "Password update failed.\n", 25);
        }

    
        else if (strncmp(buffer, "FEEDBACK|", 9) == 0) {
            char feedback[256];
            strcpy(feedback, buffer + 9);
            feedback[strcspn(feedback, "\n")] = '\0';
            add_feedback(u->id, feedback);
            write(client_sock, "Feedback recorded.\n", 20);
        }

       
        else if (strncmp(buffer, "HISTORY", 7) == 0) {
            view_transaction_history(u->id, client_sock);
        }

       
        else if (strncmp(buffer, "LOGOUT", 6) == 0) {
            write(client_sock, "Logging out...\n", 16);
            return;
        }

     
        else if (strncmp(buffer, "EXIT", 4) == 0) {
            write(client_sock, "Goodbye!\n", 10);
            close(client_sock);
            return;
        }

       
        else {
            write(client_sock, "Invalid command.\n", 18);
        }
    }
}
