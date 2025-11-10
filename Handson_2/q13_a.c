/*
===========================================================================================================================
Name : q13_a.c
Author : Asutosh Panda
Description :Write two programs: first program is waiting to catch SIGSTOP signal, the second program will send the signal 
            (using kill system call). Find out whether the first program is able to catch the signal or not.
Date: 19th Sep, 2025
===========================================================================================================================
*/
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int sig) {
    printf("Signal %d caught\n", sig);
}

int main() {
    if (signal(SIGSTOP, handler) == SIG_ERR) {
        printf("Signal handler for SIGSTOP\n");
    }
    printf("PID: %d waiting for SIGSTOP\n", getpid());
    while (1) {
        pause();
    }
    return 0;
}
/*
=====OUTPUT================================================================================================================
-----------Terminal 1----------------
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ cc 13a.c -o 13a 
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ ./13a
Signal handler for SIGSTOP
PID: 9304 waiting for SIGSTOP

[1]+  Stopped                 ./13a
-----------Terminal 2----------------
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ ps aux| grep 13a
mihir       9304  0.0  0.0   2680  1440 pts/0    S+   12:54   0:00 ./13a
mihir       9338  0.0  0.0   9144  2256 pts/1    S+   12:55   0:00 grep --color=auto 13a
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ cc 13b.c
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ ./a.out 9304
Sent SIGSTOP to PID 9304
===========================================================================================================================
*/
