#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "../data/user.h"
#include "../data/account.h"
#include "../data/transaction.h"
#include "../data/loan.h"
#include "customer.h"



void handle_employee(int client_sock, struct User *u) {
    char buffer[512];

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int n = read(client_sock, buffer, sizeof(buffer) - 1);
        if (n <= 0) break;
        buffer[n] = '\0';
        printf("Employee Command: %s\n", buffer);

       
        if (strncmp(buffer, "ADD_CUST|", 9) == 0) {
            char username[50], password[50];
            sscanf(buffer + 9, "%49[^|]|%49s", username, password);
            int res = create_user(username, password, "customer");
            if (res > 0) {
                add_account(res);
                snprintf(buffer, sizeof(buffer), "SUCCESS|Customer %s added (ID %d)\n", username, res);
            } else if (res == 0) {
                snprintf(buffer, sizeof(buffer), "ERROR|User already exists\n");
            } else {
                snprintf(buffer, sizeof(buffer), "ERROR|Failed to add customer\n");
            }
            write(client_sock, buffer, strlen(buffer));
        }

   
else if (strncmp(buffer, "MOD_CUST|", 9) == 0) {
    int id;
    char newuser[50] = "";
    char newpass[50] = "";

    // Format: MOD_CUST|<id>|<newusername>|<newpassword>
    // Either field can be "-"
    sscanf(buffer + 9, "%d|%49[^|]|%49s", &id, newuser, newpass);
  //printf("[DEBUG] Parsed modify: id=%d, newuser=%s, newpass=%s\n", id, newuser, newpass);
    struct User target;
    if (!get_user_by_id(id, &target)) {
        snprintf(buffer, sizeof(buffer), "ERROR|Customer not found\n");

        write(client_sock, buffer, strlen(buffer));
        return;
    }


    if (strcmp(newuser, "-") != 0)
        strcpy(target.username, newuser);
    if (strcmp(newpass, "-") != 0)
        strcpy(target.password, newpass);

    if (update_user(&target))
        snprintf(buffer, sizeof(buffer), "SUCCESS|Customer details updated\n");
    else
        snprintf(buffer, sizeof(buffer), "ERROR|Update failed\n");

    write(client_sock, buffer, strlen(buffer));
}


        
else if (strncmp(buffer, "VIEW_TXNS|", 10) == 0) {
    int cust_id = 0;
    if (sscanf(buffer + 10, "%d", &cust_id) == 1)
        view_transaction_history(cust_id, client_sock);
    else
        write(client_sock, "ERROR|Invalid customer ID\n", 27);
}


        else if (strncmp(buffer, "VIEW_LOANS", 10) == 0) {
            view_all_loans(client_sock);  // or view_all_loans(client_sock)
        }

        
        else if (strncmp(buffer, "CHPASS", 6) == 0) {
            char oldpass[50], newpass[50];
            sscanf(buffer + 6, "%s %s", oldpass, newpass);
            if (change_password(u->id, oldpass, newpass))
                snprintf(buffer, sizeof(buffer), "SUCCESS|Password updated\n");
            else
                snprintf(buffer, sizeof(buffer), "ERROR|Password update failed\n");
            write(client_sock, buffer, strlen(buffer));
        }

 
        else if (strncmp(buffer, "LOGOUT", 6) == 0) {
            update_login_status(u->id, 0);
            snprintf(buffer, sizeof(buffer), "Logged out successfully.\n");
            write(client_sock, buffer, strlen(buffer));
            break;
        }


        else if (strncmp(buffer, "EXIT", 4) == 0) {
            update_login_status(u->id, 0);
            close(client_sock);
            break;
        }

        else {
            snprintf(buffer, sizeof(buffer), "ERROR|Unknown command\n");
            write(client_sock, buffer, strlen(buffer));
        }
    }
}
