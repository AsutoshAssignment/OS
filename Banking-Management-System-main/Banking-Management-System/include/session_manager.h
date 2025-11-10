#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

int check_existing_session(const char* username);
int create_session(const char* username, int pid);
int close_session(const char* username);

#endif
