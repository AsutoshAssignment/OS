/*
==========================================================================
==================================================
Name : q17_b.c
Author : Asutosh Panda
Description : Write a program to execute ls -l | wc.
            a. use dup
            b. use dup2
            c. use fcntl
Date: 28th Sept, 2025
==========================================================================
==================================================
*/
#include <stdio.h>
#include <unistd.h>

int main() {
    int pipefd[2];
    pipe(pipefd);

    if (fork() == 0) { 
        dup2(pipefd[1], STDOUT_FILENO); 
        close(pipefd[0]);
        close(pipefd[1]);
        execlp("ls", "ls", "-l", NULL);
    } else {
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[1]);
        close(pipefd[0]); 
        execlp("wc", "wc", NULL); 
    }

    return 0;
}
/*
=====OUTPUT===============================================================
==================================================
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ ./a.out
     20     173    1046

==========================================================================
==================================================
*/

