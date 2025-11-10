#ifndef SERVER_H
#define SERVER_H

#include <semaphore.h>

// Server configuration
#define PORT 8080
#define MAX_CLIENTS 10

// Global semaphore for critical sections
extern sem_t *balance_sem;

// Function declarations
void* handle_client(void* socket_desc);

#endif // SERVER_H
