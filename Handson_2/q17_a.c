/*
==========================================================================
==================================================
Name : q17_a.c
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
        dup(pipefd[1]);
        close(pipefd[0]);
        execlp("ls", "ls", "-l", NULL);
    } else { 
        dup(pipefd[0]); 
        close(pipefd[1]); 
        execlp("wc", "wc", NULL); 
    }
    
    return 0;
}
/*
=====OUTPUT===============================================================
==================================================
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ ./a.out
total 76
-rwxrwxr-x 1 asutosh asutosh 16176 Sep 28 12:07 a.out
-rw-rw-r-- 1 asutosh asutosh  1170 Sep 27 18:07 q11.c
-rw-rw-r-- 1 asutosh asutosh   991 Sep 28 11:18 q12.c
-rw-rw-r-- 1 asutosh asutosh     0 Sep 28 11:15 q13.c
-rw-rw-r-- 1 asutosh asutosh  1064 Sep 28 11:24 q14.c
-rw-rw-r-- 1 asutosh asutosh  1051 Sep 28 11:57 q15.c
-rw-rw-r-- 1 asutosh asutosh  1548 Sep 28 12:00 q16.c
-rw-rw-r-- 1 asutosh asutosh  1041 Sep 28 12:07 q17_a.c
-rw-rw-r-- 1 asutosh asutosh  1286 Sep 28 11:24 q1_a.c
-rw-rw-r-- 1 asutosh asutosh  1428 Sep 23 15:05 q1_b.c
-rw-rw-r-- 1 asutosh asutosh  1375 Sep 23 15:05 q1_c.c
-rw-rw-r-- 1 asutosh asutosh     0 Sep 23 15:17 q2.c
-rw-rw-r-- 1 asutosh asutosh  1018 Sep 27 17:24 q3.c
-rw-rw-r-- 1 asutosh asutosh  1303 Sep 23 14:28 q4_time_stamp.c
-rw-rw-r-- 1 asutosh asutosh  2022 Sep 27 17:50 q5.c
-rw-rw-r-- 1 asutosh asutosh  1110 Sep 27 17:53 q6.c
-rw-rw-r-- 1 asutosh asutosh  1109 Sep 27 18:07 q7.c
-rw-rw-r-- 1 asutosh asutosh  1190 Sep 27 18:07 q9.c

==========================================================================
==================================================
*/


