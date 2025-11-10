#ifndef EMPLOYEE_HANDLER_H
#define EMPLOYEE_HANDLER_H

#include "../data/user.h"
#include "../data/account.h"
#include "../data/loan.h"
#include "../data/transaction.h"

void handle_employee(int client_sock, struct User *u);

#endif
