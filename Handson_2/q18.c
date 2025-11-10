/*
==========================================================================
==================================================
Name : q18.c
Author : Asutosh Panda
Descrioption:Write a program to find out total number of directories on the pwd.
execute ls -l | grep ^d | wc ? Use only dup2.
Date: 28th Sept, 2025
==========================================================================
==================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int fd1[2], fd2[2];

    if (pipe(fd1) == -1 || pipe(fd2) == -1) {
        exit(1);
    }

    if (fork() == 0) {
        dup2(fd1[1], STDOUT_FILENO);
        close(fd1[0]); 
	close(fd1[1]);
        close(fd2[0]);
       	close(fd2[1]);
        execlp("ls", "ls", "-l", NULL);
        exit(1);
    }

    if (fork() == 0) {
        dup2(fd1[0], STDIN_FILENO);  
        dup2(fd2[1], STDOUT_FILENO);
        close(fd1[0]);
       	close(fd1[1]);
        close(fd2[0]); 
	close(fd2[1]);
        execlp("grep", "grep", "^d", NULL);
        exit(1);
    }

    if (fork() == 0) {
        dup2(fd2[0], STDIN_FILENO); 
        close(fd1[0]);
       	close(fd1[1]);
        close(fd2[0]); 
	close(fd2[1]);
        execlp("wc", "wc", "-l", NULL);
        exit(1);
    }

    close(fd1[0]); 
    close(fd1[1]);
    close(fd2[0]); 
    close(fd2[1]);

    while (wait(NULL) > 0);
    return 0;
}
/*
=====OUTPUT===============================================================
==================================================
drwxrwxr-x 2 asutosh asutosh  4096 Sep 30 12:50 test1
drwxrwxr-x 2 asutosh asutosh  4096 Sep 30 12:50 test2
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ ./a.out
2
==========================================================================
==================================================
*/


