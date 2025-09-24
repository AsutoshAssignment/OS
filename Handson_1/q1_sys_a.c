/*
===========================================================================================================================
Name : 1_sys_a.c
Author : Asutosh Panda
Description : Create the following types of a files using (i) shell command (ii) system call
              a. soft link (symlink system call)
              b. hard link (link system call)
              c. FIFO (mkfifo Library Function or mknod system call)
Date: 21st Aug, 2025.
===========================================================================================================================
*/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main(){
    
    if(symlink("original.txt", "soft_link.txt")== -1){
        printf("Error while creating soft link\n");
    }
    else{
        printf("Soft link created");
    }
    return 0;
}
/*
=====OUTPUT================================================================================================================
nani@Nani:~/handson$ ls
a.out  original.txt  q1_sys_a.c
nani@Nani:~/handson$ ./a.out
Soft link created in soft_link.txt
nani@Nani:~/handson$ ls -l
total 20
-rwxrwxr-x 1 nani nani 16048 Sep  5 17:37 a.out
-rw-rw-r-- 1 nani nani     0 Sep  5 17:37 original.txt
-rw-rw-r-- 1 nani nani  1948 Sep  5 17:36 q1_sys_a.c
lrwxrwxrwx 1 nani nani    12 Sep  5 17:37 soft_link.txt -> original.txt
===========================================================================================================================
*/
