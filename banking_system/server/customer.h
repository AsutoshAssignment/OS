#ifndef CUSTOMER_HANDLER_H
#define CUSTOMER_HANDLER_H

#include "../data/user.h"
#include "../data/account.h"
#include "../data/transaction.h"
#include "../data/loan.h"
#include "../data/feedback.h"

void handle_customer(int client_sock, struct User *u);

#endif
