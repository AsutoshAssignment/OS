#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "loan.h"

#define LOAN_FILE "../data/loans.txt"

static void lock_file(int fd, short type) {
    struct flock lock;
    lock.l_type = type;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    fcntl(fd, F_SETLKW, &lock);
}

int apply_for_loan(int user_id, float amount) {
    int fd = open(LOAN_FILE, O_WRONLY | O_CREAT | O_APPEND, 0666);
    if (fd < 0) return 0;
    lock_file(fd, F_WRLCK);

    FILE *fp = fdopen(fd, "a");
    if (!fp) { close(fd); return 0; }

    static int loan_id = 1000;
    time_t t = time(NULL);
    fprintf(fp, "%d|%d|%.2f|pending|%s", loan_id++, user_id, amount, ctime(&t));
    fflush(fp);

    lock_file(fd, F_UNLCK);
    fclose(fp);
    return 1;
}

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "loan.h"

#define LOAN_FILE "../data/loans.txt"
// View all loan applications — for employees
int view_all_loans(int client_sock) {
    FILE *file = fopen(LOAN_FILE, "r");
    if (!file) {
        perror("Error opening loans file");
        return 0;
    }

    char line[256];
    int loan_id, user_id;
    float amount;
    char status[20];
    int found = 0;

    // Header message
    snprintf(line, sizeof(line), "\n--- All Loan Applications ---\n");
    write(client_sock, line, strlen(line));

    // Each line format: loan_id user_id amount status
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%d %d %f %19s", &loan_id, &user_id, &amount, status) == 4) {
            char msg[256];
            snprintf(msg, sizeof(msg),
                     "Loan ID: %d | User ID: %d | Amount: %.2f | Status: %s\n",
                     loan_id, user_id, amount, status);
            write(client_sock, msg, strlen(msg));
            found = 1;
        }
    }

    if (!found) {
        snprintf(line, sizeof(line), "No loan applications found.\n");
        write(client_sock, line, strlen(line));
    }

    fclose(file);
    return found;
}
