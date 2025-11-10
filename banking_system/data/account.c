#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "account.h"
#include "transaction.h"



static void lock_file(int fd, short type) {
    struct flock lock;
    lock.l_type = type;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    fcntl(fd, F_SETLKW, &lock);
}

int get_account_by_userid(int user_id, struct Account *a) {
    int fd = open(ACCOUNT_FILE, O_RDONLY);
    if (fd < 0) return 0;
    lock_file(fd, F_RDLCK);
    printf("%d",user_id);
    FILE *fp = fdopen(fd, "r");
    if (!fp) { close(fd); return 0; }

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        struct Account tmp;
        if (sscanf(line, "%d|%d|%f|%9s", &tmp.account_id, &tmp.user_id, &tmp.balance, tmp.status) == 4) {
            if (tmp.user_id == user_id) {
                *a = tmp;
                lock_file(fd, F_UNLCK);
                fclose(fp);
                return 1;
            }
        }
    }

    lock_file(fd, F_UNLCK);
    fclose(fp);
    return 0;
}

int update_balance(int user_id, float new_balance) {
    int fd = open(ACCOUNT_FILE, O_RDWR);
    if (fd < 0) return 0;
    lock_file(fd, F_WRLCK);

    FILE *fp = fdopen(fd, "r+");
    if (!fp) { close(fd); return 0; }

    char line[256];
    long pos;
    while ((pos = ftell(fp)), fgets(line, sizeof(line), fp)) {
        struct Account tmp;
        if (sscanf(line, "%d|%d|%f|%9s", &tmp.account_id, &tmp.user_id, &tmp.balance, tmp.status) == 4) {
            if (tmp.user_id == user_id) {
                fseek(fp, pos, SEEK_SET);
                fprintf(fp, "%d|%d|%.2f|%s\n", tmp.account_id, tmp.user_id, new_balance, tmp.status);
                fflush(fp);
                break;
            }
        }
    }

    lock_file(fd, F_UNLCK);
    fclose(fp);
    return 1;
}


int deposit_money(int user_id, float amount) {
    struct Account a;
    if (!get_account_by_userid(user_id, &a)) return 0;
    a.balance += amount;
    return update_balance(user_id, a.balance);
}


int withdraw_money(int user_id, float amount) {
    struct Account a;
    if (!get_account_by_userid(user_id, &a)) return 0;
    if (a.balance < amount) return -1;  
    a.balance -= amount;
    return update_balance(user_id, a.balance);
}


int view_account_details(int user_id, int client_sock) {
    struct Account a;
    if (get_account_by_userid(user_id, &a)) {
        char buffer[256];
        snprintf(buffer, sizeof(buffer),
                 "Account ID: %d\nBalance: %.2f\nStatus: %s\n",
                 a.account_id, a.balance, a.status);
        write(client_sock, buffer, strlen(buffer));
    } else {
        char msg[] = "No account found.\n";
        write(client_sock, msg, strlen(msg));
    }
    return user_id;
}

int add_account(int user_id) {
    int fd = open(ACCOUNT_FILE, O_RDWR | O_CREAT, 0666);
    if (fd < 0) return 0;

    lock_file(fd, F_WRLCK);

    FILE *fp = fdopen(fd, "a+");
    if (!fp) { close(fd); return 0; }

    int max_id = 1000;
    char line[256];
    struct Account tmp;

    // Find last account_id
    rewind(fp);
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%d|%d|%f|%9s", &tmp.account_id, &tmp.user_id, &tmp.balance, tmp.status) == 4) {
            if (tmp.account_id > max_id) max_id = tmp.account_id;
        }
    }

    int new_acc_id = max_id + 1;

    fprintf(fp, "%d|%d|0.00|active\n", new_acc_id, user_id);
    fflush(fp);

    lock_file(fd, F_UNLCK);
    fclose(fp);
    return 1;
}

int transfer_money(int from_user_id, int to_user_id, float amount) {
    int fd = open(ACCOUNT_FILE, O_RDWR);
    if (fd < 0) return 0;
    lock_file(fd, F_WRLCK);

    FILE *fp = fdopen(fd, "r+");
    if (!fp) { close(fd); return 0; }

    struct Account from, to;
    int from_found = 0, to_found = 0;
    long from_pos = 0, to_pos = 0;
    char line[256];

    while ((from_pos = ftell(fp)), fgets(line, sizeof(line), fp)) {
        struct Account tmp;
        if (sscanf(line, "%d|%d|%f|%9s", &tmp.account_id, &tmp.user_id, &tmp.balance, tmp.status) == 4) {
            if (tmp.user_id == from_user_id) {
                from = tmp; from_found = 1;
            } else if (tmp.user_id == to_user_id) {
                to = tmp; to_found = 1;
            }
        }
        if (from_found && to_found) break;
    }

    if (!from_found || !to_found || from.balance < amount) {
        lock_file(fd, F_UNLCK);
        fclose(fp);
        return 0; 
    }

    from.balance -= amount;
    to.balance += amount;

    rewind(fp);

    while ((from_pos = ftell(fp)), fgets(line, sizeof(line), fp)) {
        struct Account tmp;
        if (sscanf(line, "%d|%d|%f|%9s", &tmp.account_id, &tmp.user_id, &tmp.balance, tmp.status) == 4) {
            if (tmp.user_id == from_user_id) {
                fseek(fp, from_pos, SEEK_SET);
                fprintf(fp, "%d|%d|%.2f|%s\n", tmp.account_id, tmp.user_id, from.balance, tmp.status);
                fflush(fp);
            } else if (tmp.user_id == to_user_id) {
                fseek(fp, from_pos, SEEK_SET);
                fprintf(fp, "%d|%d|%.2f|%s\n", tmp.account_id, tmp.user_id, to.balance, tmp.status);
                fflush(fp);
            }
        }
    }

    lock_file(fd, F_UNLCK);
    fclose(fp);

    log_transaction(from.user_id, to.user_id, amount, "transfer");

    return 1;
}


