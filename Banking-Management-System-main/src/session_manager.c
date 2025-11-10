#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/file.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <sys/socket.h> 
#include <sys/types.h> 
#include "../include/data_structures.h"

#define SESSION_FILE "data/active_sessions.dat"
#define SESSION_TIMEOUT 300

__thread char current_session_username[50] = {0};
__thread int session_created = 0;

extern int authenticate_user(const char* username, const char* password, char* role);
extern void handle_customer_operations(int client_socket, const char* username);
extern void handle_employee_operations(int client_socket, const char* username);
extern void handle_manager_operations(int client_socket, const char* username);
extern void handle_admin_operations(int client_socket, const char* username);

void cleanup_stale_sessions() {
    int fd = open(SESSION_FILE, O_RDWR);
    if (fd < 0) return;
    
    struct flock lock = {F_WRLCK, SEEK_SET, 0, 0, 0};
    fcntl(fd, F_SETLKW, &lock);
    
    int temp_fd = open("data/active_sessions.tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (temp_fd < 0) {
        close(fd);
        return;
    }
    
    Session session;
    time_t now = time(NULL);
    
    while (read(fd, &session, sizeof(session)) == sizeof(session)) {
        if (session.is_active && (now - session.login_time) < SESSION_TIMEOUT) {
            write(temp_fd, &session, sizeof(session));
        }
    }
    
    close(temp_fd);
    close(fd);
    rename("data/active_sessions.tmp", SESSION_FILE);
}

int check_existing_session(const char* username) {
    cleanup_stale_sessions();
    
    int fd = open(SESSION_FILE, O_RDONLY);
    if (fd < 0) return 0;
    
    struct flock lock = {F_RDLCK, SEEK_SET, 0, 0, 0};
    fcntl(fd, F_SETLKW, &lock);
    
    Session session;
    time_t now = time(NULL);
    
    while (read(fd, &session, sizeof(session)) == sizeof(session)) {
        if (strcmp(session.username, username) == 0 && session.is_active) {
            if ((now - session.login_time) < SESSION_TIMEOUT) {
                close(fd);
                return 1;
            }
        }
    }
    
    close(fd);
    return 0;
}

int create_session(const char* username, pthread_t thread_id) {
    int fd = open(SESSION_FILE, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd < 0) return -1;
    
    struct flock lock = {F_WRLCK, SEEK_SET, 0, 0, 0};
    fcntl(fd, F_SETLKW, &lock);
    
    Session session;
    strncpy(session.username, username, sizeof(session.username) - 1);
    session.username[sizeof(session.username) - 1] = '\0';
    session.process_id = (int)thread_id;
    session.login_time = time(NULL);
    session.is_active = 1;
    
    write(fd, &session, sizeof(session));
    
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
    
    strncpy(current_session_username, username, sizeof(current_session_username) - 1);
    session_created = 1;
    
    return 0;
}

int close_session(const char* username) {
    int fd = open(SESSION_FILE, O_RDWR);
    if (fd < 0) return -1;
    
    struct flock lock = {F_WRLCK, SEEK_SET, 0, 0, 0};
    fcntl(fd, F_SETLKW, &lock);
    
    Session session;
    off_t pos;
    int found = 0;
    
    while ((pos = lseek(fd, 0, SEEK_CUR)) >= 0 &&
           read(fd, &session, sizeof(session)) == sizeof(session)) {
        if (strcmp(session.username, username) == 0 && session.is_active) {
            session.is_active = 0;
            lseek(fd, pos, SEEK_SET);
            write(fd, &session, sizeof(session));
            found = 1;
            break;
        }
    }
    
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);
    
    cleanup_stale_sessions();
    return found ? 0 : -1;
}

void cleanup_current_session() {
    if (session_created && current_session_username[0] != '\0') {
        printf("Cleaning up session for: %s\n", current_session_username);
        close_session(current_session_username);
        session_created = 0;
        current_session_username[0] = '\0';
    }
}

void handle_session(int client_socket) {
     cleanup_current_session();
    char buffer[100];
    char username[50], password[50], role[20];
    int authenticated = 0;
    
    while (!authenticated) {
        memset(buffer, 0, sizeof(buffer));
        memset(username, 0, sizeof(username));
        memset(password, 0, sizeof(password));
        
        send(client_socket, "Username: ", 10, 0);
        int bytes = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes <= 0) {
            cleanup_current_session();
            return;
        }
        buffer[bytes] = '\0';
        char *newline = strchr(buffer, '\n');
        if (newline) *newline = '\0';
        sscanf(buffer, "%49s", username);
        
        send(client_socket, "Password: ", 10, 0);
        memset(buffer, 0, sizeof(buffer));
        bytes = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes <= 0) {
            cleanup_current_session();
            return;
        }
        buffer[bytes] = '\0';
        newline = strchr(buffer, '\n');
        if (newline) *newline = '\0';
        sscanf(buffer, "%49s", password);
        
        if (authenticate_user(username, password, role) != 0) {
            send(client_socket, "LOGIN_FAILED\n", 13, 0);
            continue;
        }
        
        if (check_existing_session(username)) {
            send(client_socket, "ALREADY_LOGGED_IN\n", 18, 0);
            continue;
        }
        
        authenticated = 1;
    }
    
    create_session(username, pthread_self());
    
    char login_msg[100];
    snprintf(login_msg, sizeof(login_msg), "LOGIN_SUCCESS|%s\n", role);
    send(client_socket, login_msg, strlen(login_msg), 0);
    
    if (strcmp(role, "customer") == 0) {
        handle_customer_operations(client_socket, username);
    } else if (strcmp(role, "employee") == 0) {
        handle_employee_operations(client_socket, username);
    } else if (strcmp(role, "manager") == 0) {
        handle_manager_operations(client_socket, username);
    } else if (strcmp(role, "admin") == 0) {
        handle_admin_operations(client_socket, username);
    }
    
    cleanup_current_session();
}
