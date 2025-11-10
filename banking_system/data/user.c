// user.c
#include "user.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>


int get_next_user_id() {
    FILE *fp = fopen(USER_FILE, "r");
    if (!fp) return 1;

    char line[256];
    int max_id = 0, id;
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%d|", &id) == 1) {
            if (id > max_id) max_id = id;
        }
    }
    fclose(fp);
    return max_id + 1;
}

int create_user(const char *username, const char *password, const char *role) {
    int fd = open(USER_FILE, O_RDWR | O_CREAT, 0644);
    if (fd < 0) return -1;


    struct flock lock = {F_WRLCK, SEEK_SET, 0, 0, 0};
    fcntl(fd, F_SETLKW, &lock);

    FILE *fp = fdopen(fd, "a+");
    if (!fp) {
        close(fd);
        return -1;
    }

   
    rewind(fp);
    char line[256], uname[50];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%*d|%49[^|]", uname);
        if (strcmp(uname, username) == 0) {
            lock.l_type = F_UNLCK;
            fcntl(fd, F_SETLK, &lock);
            fclose(fp);
            return 0; 
        }
    }

    // Append new user
    int id = get_next_user_id();
    fprintf(fp, "%d|%s|%s|%s|0\n", id, username, password, role);
    fflush(fp);

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    fclose(fp);
    return id; 
}



int update_user(struct User *u) {
    FILE *fp = fopen(USER_FILE, "r");
    if (!fp) return 0;

    FILE *temp = fopen("temp_users.txt", "w");
    if (!temp) { fclose(fp); return 0; }

    struct User tempUser;
    int updated = 0;

    while (fscanf(fp, "%d|%49[^|]|%49[^|]|%19[^|]|%d|%d\n",
                  &tempUser.id, tempUser.username, tempUser.password,
                  tempUser.role, &tempUser.logged_in, &tempUser.active) == 6) {
        if (tempUser.id == u->id) {
            fprintf(temp, "%d|%s|%s|%s|%d|%d\n",
                    u->id, u->username, u->password, u->role, u->logged_in, u->active);
            updated = 1;
        } else {
            fprintf(temp, "%d|%s|%s|%s|%d|%d\n",
                    tempUser.id, tempUser.username, tempUser.password,
                    tempUser.role, tempUser.logged_in, tempUser.active);
        }
    }

    fclose(fp);
    fclose(temp);

    remove(USER_FILE);
    rename("temp_users.txt", USER_FILE);
    return updated;
}

int authenticate_user(struct User *u) {
    int fd = open(USER_FILE, O_RDONLY);
    if (fd < 0) return 0;

    FILE *fp = fdopen(fd, "r");
    if (!fp) {
        close(fd);
        return 0;
    }

    struct flock lock = {F_RDLCK, SEEK_SET, 0, 0, 0};
    fcntl(fd, F_SETLKW, &lock);

    char line[256];
    struct User temp;

    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = '\0'; 

        if (sscanf(line, "%d|%49[^|]|%49[^|]|%19[^|]|%d",
                   &temp.id, temp.username, temp.password, temp.role, &temp.logged_in) == 5) {

            if (strcmp(u->username, temp.username) == 0 &&
                strcmp(u->password, temp.password) == 0) {
                
                temp.logged_in = 1;
                // Copy all details to the provided struct
                *u = temp;

                lock.l_type = F_UNLCK;
                fcntl(fd, F_SETLK, &lock);
                fclose(fp);
                return 1; // success
            }
        }
    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    fclose(fp);
    return 0; // invalid
 // not found
// 1 = success, -2 = already logged in, 0 = not found
}

int change_password(int user_id, const char *old_pass, const char *new_pass) {
    int fd = open(USER_FILE, O_RDWR);
    if (fd < 0) return 0;

    FILE *fp = fdopen(fd, "r+");
    if (!fp) { close(fd); return 0; }

    struct flock lock = {F_WRLCK, SEEK_SET, 0, 0, 0};
    fcntl(fd, F_SETLKW, &lock);

    char line[256];
    long pos;
    int updated = 0;

    while ((pos = ftell(fp)), fgets(line, sizeof(line), fp)) {
        struct User tmp;
        // Strip newline from the line
        line[strcspn(line, "\n")] = 0;

        if (sscanf(line, "%d|%49[^|]|%49[^|]|%19[^|]|%d",
                   &tmp.id, tmp.username, tmp.password, tmp.role, &tmp.logged_in) == 5) {
            
            // Debug print (optional)
            printf("DEBUG: checking id=%d pass=%s\n", tmp.id, tmp.password);
            fflush(stdout);
            printf("DEBUG: [%s] vs [%s]\n", tmp.password, old_pass);
            fflush(stdout);

            if (tmp.id == user_id && strcmp(tmp.password, old_pass) == 0) {
                fseek(fp, pos, SEEK_SET);
                fprintf(fp, "%d|%s|%s|%s|%d\n",
                        tmp.id, tmp.username, new_pass, tmp.role, tmp.logged_in);
                fflush(fp);
                updated = 1;
                break;
            }
        }
    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    fclose(fp);
    return updated;
}

int get_user_by_id(int id, struct User *u) {
    FILE *fp = fopen(USER_FILE, "r");
    if (!fp) return 0;

while (fscanf(fp, "%d|%49[^|]|%49[^|]|%19[^|]|%d|%d\n",
              &u->id, u->username, u->password, u->role,
              &u->logged_in, &u->active) == 6) {

 

    if (u->id == id) {
        fclose(fp);
        return 1;
    }
}



    fclose(fp);
    return 0;  
}

void view_all_users(int client_sock) {
    FILE *fp = fopen(USER_FILE, "r");
    if (!fp) {
        write(client_sock, "Unable to open user file.\n", 27);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        write(client_sock, line, strlen(line));
    }
    fclose(fp);
}




int update_login_status(int user_id, int status) {
    int fd = open(USER_FILE, O_RDWR);
    if(fd < 0) {
        perror("open");
        return -1;
    }

    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    fcntl(fd, F_SETLKW, &lock);  // write lock

    off_t pos = 0;
    char line[512];
    char buffer[10000] = "";
    lseek(fd, 0, SEEK_SET);

    FILE *fp = fdopen(fd, "r+");
    while(fgets(line, sizeof(line), fp)) {
        struct User temp;
        sscanf(line, "%d|%49[^|]|%49[^|]|%19[^|]|%d|%d",
               &temp.id, temp.username, temp.password,
               temp.role, &temp.active, &temp.logged_in);
        if(temp.id == user_id)
            temp.logged_in = status;

        char newline[256];
        snprintf(newline, sizeof(newline),
                 "%d|%s|%s|%s|%d|%d\n",
                 temp.id, temp.username, temp.password,
                 temp.role, temp.active, temp.logged_in);
        strcat(buffer, newline);
    }

    freopen(USER_FILE, "w", fp);
    fputs(buffer, fp);
    fclose(fp);

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
    return 0;
}

int update_user_status(int id, int active) {
    FILE *fp = fopen(USER_FILE, "r");
    if (!fp) return 0;

    FILE *temp = fopen("temp_users.txt", "w");
    if (!temp) { fclose(fp); return 0; }

    struct User u;
    int found = 0;
    while (fscanf(fp, "%d|%49[^|]|%49[^|]|%19[^|]|%d|%d\n",
                  &u.id, u.username, u.password, u.role, &u.logged_in, &u.active) == 6) {
        if (u.id == id) {
            u.active = active;
            found = 1;
        }
        fprintf(temp, "%d|%s|%s|%s|%d|%d\n",
                u.id, u.username, u.password, u.role, u.logged_in, u.active);
    }

    fclose(fp);
    fclose(temp);
    remove(USER_FILE);
    rename("temp_users.txt", USER_FILE);
    return found;
}


// int approve_reject_employee(int id, int approve) {
//     FILE *fp = fopen(USER_FILE, "r");
//     if (!fp) return 0;

//     FILE *temp = fopen("temp_users.txt", "w");
//     if (!temp) { fclose(fp); return 0; }

//     struct User u;
//     int found = 0;
//     while (fscanf(fp, "%d|%49[^|]|%49[^|]|%19[^|]|%d|%d\n",
//                   &u.id, u.username, u.password, u.role, &u.logged_in, &u.active) == 6) {
//         if (u.id == id && strcmp(u.role, "employee") == 0) {
//             u.active = approve;
//             found = 1;
//         }
//         fprintf(temp, "%d|%s|%s|%s|%d|%d\n",
//                 u.id, u.username, u.password, u.role, u.logged_in, u.active);
//     }

//     fclose(fp);
//     fclose(temp);
//     remove(USER_FILE);
//     rename("temp_users.txt", USER_FILE);
//     return found;
// }
