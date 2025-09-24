/*
===========================================================================================================================
Name : 1_sys_c.c
Author : Asutosh Panda
Description : Create the following types of a files using (i) shell command (ii) system call
              a. soft link (symlink system call)
              b. hard link (link system call)
              c. FIFO (mkfifo Library Function or mknod system call)
Date: 21st Aug, 2025.
===========================================================================================================================
*/

#include<stdio.h>
#include<sys/types.h>

int main(){
    if(mkfifo("self_pipe", 0644)== -1){
        printf("Error\n");
    }
    else{
        printf("Createed\n");
    }
    return 0;
}
/*
=====OUTPUT================================================================================================================
Createed
nani@Nani:~/handson$ ls -l
total 28
-rwxrwxr-x 1 nani nani 16008 Sep  5 17:48 a.out
-rw-rw-r-- 2 nani nani     0 Sep  5 17:37 hard_link.txt
prw-r--r-- 1 nani nani     0 Sep  5 17:48 self_pipe
-rw-rw-r-- 2 nani nani     0 Sep  5 17:37 original.txt
-rw-rw-r-- 1 nani nani  1948 Sep  5 17:36 q1_sys_a.c
-rw-rw-r-- 1 nani nani   204 Sep  5 17:42 q1_sys_b.c
-rw-rw-r-- 1 nani nani   186 Sep  5 17:48 q1_sys_c.c
lrwxrwxrwx 1 nani nani    12 Sep  5 17:37 soft_link.txt -> original.txt===========================================================================================================================
*/
