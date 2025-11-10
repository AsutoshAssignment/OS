#ifndef ACCOUNT_H
#define ACCOUNT_H
#define ACCOUNT_FILE "../data/accounts.txt"

struct Account {
    int account_id;
    int user_id;    
    float balance;
    char status[10]; 
};

int add_account(int user_id);
int get_account_by_userid(int user_id, struct Account *a);
int update_balance(int user_id, float new_balance);
int view_account_details(int user_id, int client_sock);
int deposit_money(int user_id, float amount);
int withdraw_money(int user_id, float amount);

#endif
