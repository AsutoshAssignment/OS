/*
===========================================================================================================================
Name : 30_daemon.c
Author : Asutosh Panda
Description : Write a program to run a script at a specific time using a Daemon process

Date: 5th Sept, 2025.
===========================================================================================================================
*/
#include <stdio.h>
#include <sys/stat.h> 
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main() {
    int pid = fork();
    if (pid < 0) {
        printf("fork failed");
        return 1;
    }
    if (pid > 0) {
        printf("Daemon started with PID %d\n", pid);
        return 0;
    }
    setsid();
    umask(0);
    chdir("/");
    
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    
    while (1) {
        time_t now = time(NULL);
        struct tm *t = localtime(&now);


        if (t->tm_hour == 10 && t->tm_min == 44) {
            system("/home/asutosh/Documents/OS/Handson_1/script.sh");
	    printf("Success \n");
            sleep(60);
        }

        sleep(1);
    }

    return 0;
}
/*
=====OUTPUT================================================================================================================
nani@Nani:~/handson1$ ./a.out
Daemon started with PID 4246
nani@Nani:~/handson1$ Success 
nani@Nani:~/handson1$ cat output_q30.txt
this was written by Daemon at Friday 05 September 2025 06:52:00 PM IST
===========================================================================================================================
*/

