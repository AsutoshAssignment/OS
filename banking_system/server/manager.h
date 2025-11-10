#ifndef MANAGER_H
#define MANAGER_H
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "../data/user.h"
#include "../data/account.h"
#include "../data/transaction.h"
#include "../data/loan.h"
#include "../data/feedback.h"

#define FEEDBACK_FILE "data/feedback.txt"
  // for struct User, BUFFER_SIZE, etc.

void handle_manager(int client_sock, struct User user);

// Manager-specific helper functions



#endif

