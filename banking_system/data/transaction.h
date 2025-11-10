#ifndef TRANSACTION_H
#define TRANSACTION_H

void log_transaction(int from_user, int to_user, float amount, const char *type);
void view_transaction_history(int user_id, int client_sock);

#endif
