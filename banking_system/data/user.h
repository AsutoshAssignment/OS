// user.h
#ifndef USER_H
#define USER_H 

#define USER_FILE "../data/users.txt"

struct User {
    int id;
    char username[50];
    char password[50];
    char role[20];
    int active;
    int logged_in;
};

int authenticate_user(struct User *u);
//int update_user_details(int user_id, const char *new_username, const char *new_password);
int update_user(struct User *u); 
int update_login_status(int user_id, int status);
int create_user(const char *username, const char *password, const char *role);
int get_next_user_id();
int change_password(int user_id, const char *old_pass, const char *new_pass);
int get_user_by_id(int id, struct User *u); 
void view_all_users(int client_sock);
int update_user_status(int id, int active);
int approve_reject_employee(int id, int approve);
#endif
