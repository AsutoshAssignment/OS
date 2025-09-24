/*
===========================================================================================================================
Name : 1_sys_b.c
Author : Asutosh Panda
Description : Create the following types of a files using (i) shell command (ii) system call
              a. soft link (symlink system call)
              b. hard link (link system call)
              c. FIFO (mkfifo Library Function or mknod system call)
Date: 21st Aug, 2025.
===========================================================================================================================
*/
#include<stdio.h>
#include<stdlib.h>

int main(){
    if(link("original.txt", "hard_link.txt";)== -1){
        printf("Error\n");
    }
    else{
        printf("Hard link created");
    }
    return 0;
}/*
=====OUTPUT================================================================================================================
nani@Nani:~/handson$ ls
a.out  hard_link.txt  original.txt  q1_sys_a.c  q1_sys_b.c  soft_link.txt
nani@Nani:~/handson$ ls -l
total 24
-rwxrwxr-x 1 nani nani 16048 Sep  5 17:42 a.out
-rw-rw-r-- 2 nani nani     0 Sep  5 17:37 hard_link.txt
-rw-rw-r-- 2 nani nani     0 Sep  5 17:37 original.txt
-rw-rw-r-- 1 nani nani  1948 Sep  5 17:36 q1_sys_a.c
-rw-rw-r-- 1 nani nani   204 Sep  5 17:42 q1_sys_b.c
lrwxrwxrwx 1 nani nani    12 Sep  5 17:37 soft_link.txt -> original.txt
===========================================================================================================================
*/
~  
