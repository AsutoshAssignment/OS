#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "manager.h"
#include "../data/user.h"
#include "../data/feedback.h"

void handle_manager(int client_sock, struct User user) {
    char buffer[512];
    int n;

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        n = read(client_sock, buffer, sizeof(buffer) - 1);
        if (n <= 0) break; // client disconnected
        buffer[n] = '\0';

        printf("[DEBUG] Manager cmd: %s\n", buffer); // helpful for debugging

        if (strncmp(buffer, "VIEW_USERS", 10) == 0) {
            view_all_users(client_sock);
        }

        else if (strncmp(buffer, "UPDATE_CUST|", 12) == 0) {
            int id, status;
            if (sscanf(buffer + 12, "%d|%d", &id, &status) == 2) {
                if (update_user_status(id, status))
                    write(client_sock, "SUCCESS|Customer status updated\n", 33);
                else
                    write(client_sock, "ERROR|Unable to update customer\n", 32);
            } else {
                write(client_sock, "ERROR|Invalid format\n", 22);
            }
        }

        else if (strncmp(buffer, "APPROVE_EMP|", 12) == 0) {
            int id, decision;
            if (sscanf(buffer + 12, "%d|%d", &id, &decision) == 2) {
                if (approve_reject_employee(id, decision))
                    write(client_sock, "SUCCESS|Employee updated\n", 26);
                else
                    write(client_sock, "ERROR|Unable to update employee\n", 32);
            } else {
                write(client_sock, "ERROR|Invalid format\n", 22);
            }
        }

        else if (strncmp(buffer, "VIEW_FEEDBACK", 13) == 0) {
            view_feedback(client_sock);
        }

        else if (strncmp(buffer, "CHANGE_PASS|", 12) == 0) {
            char oldpass[50], newpass[50];
            if (sscanf(buffer + 12, "%49[^|]|%49s", oldpass, newpass) == 2) {
                if (change_password(user.id, oldpass, newpass))
                    write(client_sock, "SUCCESS|Password changed\n", 26);
                else
                    write(client_sock, "ERROR|Invalid old password\n", 28);
            } else {
                write(client_sock, "ERROR|Invalid format\n", 22);
            }
        }

        else if (strncmp(buffer, "LOGOUT", 6) == 0) {
            user.logged_in = 0;
            update_user(&user);
            write(client_sock, "SUCCESS|Logged out\n", 20);
            return;
        }

        else if (strncmp(buffer, "EXIT", 4) == 0) {
            write(client_sock, "Goodbye!\n", 9);
            close(client_sock);
            return;
        }

        else {
            write(client_sock, "ERROR|Unknown command\n", 23);
        }
    }
}
