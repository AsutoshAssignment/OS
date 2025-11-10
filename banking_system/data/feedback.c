#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "feedback.h"

#define FEEDBACK_FILE "data/feedback.txt"

static void lock_file(int fd, short type) {
    struct flock lock;
    lock.l_type = type;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    fcntl(fd, F_SETLKW, &lock);
}

void add_feedback(int user_id, const char *message) {
    int fd = open(FEEDBACK_FILE, O_WRONLY | O_CREAT | O_APPEND, 0666);
    if (fd < 0) return;
    lock_file(fd, F_WRLCK);

    FILE *fp = fdopen(fd, "a");
    if (!fp) { close(fd); return; }

    time_t t = time(NULL);
    fprintf(fp, "%d|%s|%s", user_id, message, ctime(&t));
    fflush(fp);

    lock_file(fd, F_UNLCK);
    fclose(fp);
}

void view_feedback(int client_sock) {
    int fd = open(FEEDBACK_FILE, O_RDONLY);
    if (fd < 0) {
        write(client_sock, "ERROR|Unable to open feedback file\n", 35);
        return;
    }

    struct flock lock = {F_RDLCK, SEEK_SET, 0, 0, 0};
    fcntl(fd, F_SETLKW, &lock);

    FILE *fp = fdopen(fd, "r");
    if (!fp) {
        close(fd);
        write(client_sock, "ERROR|Unable to read feedback file\n", 35);
        return;
    }

    char line[512];
    int found = 0;

    while (fgets(line, sizeof(line), fp)) {
        found = 1;
        write(client_sock, line, strlen(line));
    }

    if (!found)
        write(client_sock, "No feedback entries found.\n", 28);

    // Signal the client that feedback reading is done
    write(client_sock, "END", 3);

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    fclose(fp);
}