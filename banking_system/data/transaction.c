#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include "transaction.h"

#define TXN_FILE "../data/transactions.txt"

static void lock_file(int fd, short type) {
    struct flock lock;
    lock.l_type = type;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    fcntl(fd, F_SETLKW, &lock);
}

void log_transaction(int from_user, int to_user, float amount, const char *type) {
    int fd = open(TXN_FILE, O_WRONLY | O_CREAT | O_APPEND, 0666);
    if (fd < 0) return;
    lock_file(fd, F_WRLCK);

    FILE *fp = fdopen(fd, "a");
    if (!fp) { close(fd); return; }

    time_t t = time(NULL);
    fprintf(fp, "%d|%d|%.2f|%s|%s", from_user, to_user, amount, type, ctime(&t));
    fflush(fp);

    lock_file(fd, F_UNLCK);
    fclose(fp);
}

void view_transaction_history(int user_id, int client_sock) {
    int fd = open(TXN_FILE, O_RDONLY);
    if (fd < 0) {
        write(client_sock, "Unable to open transaction file.\nEND", 37);
        return;
    }

    lock_file(fd, F_RDLCK);

    FILE *fp = fdopen(fd, "r");
    if (!fp) {
        close(fd);
        write(client_sock, "Error reading transaction file.\nEND", 36);
        return;
    }

    char line[256];
    char response[4096] = "";  // collect everything here
    int found = 0;

    while (fgets(line, sizeof(line), fp)) {
        int from, to;
        float amt;
        char type[20], timebuf[64];

        if (sscanf(line, "%d|%d|%f|%19[^|]|%63[^\n]",
                   &from, &to, &amt, type, timebuf) == 5) {
            if (from == user_id || to == user_id) {
                found = 1;
                char entry[256];
                snprintf(entry, sizeof(entry),
                         "From: %d | To: %d | Amount: %.2f | Type: %s | Time: %s\n",
                         from, to, amt, type, timebuf);
                strncat(response, entry, sizeof(response) - strlen(response) - 1);
            }
        }
    }

    if (!found)
        strncat(response, "No transactions found.\n", sizeof(response) - strlen(response) - 1);

    strncat(response, "END", sizeof(response) - strlen(response) - 1);
    write(client_sock, response, strlen(response));

    lock_file(fd, F_UNLCK);
    fclose(fp);
}

