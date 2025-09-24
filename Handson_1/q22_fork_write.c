/*
===========================================================================================================================
Name : 22_fork_write.c
Author : Asutosh Panda
Description : Write a program, open a file, call fork, and then write to the file by both the child as well as the
parent processes. Check output of the file.
Date: 28th Aug, 2025.
===========================================================================================================================
*/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>  
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int main() {
    
    int fd = open("textfile_22.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(fd==-1) {
    printf("Error");
    return 1;
    }
    int pid = fork();

    if (pid == 0) {
        const char *child = "Child process message.\n";
	if (write(fd, child ,strlen(child))==-1){
		printf("error in child writing"); 
		return 1;
	}
        close(fd);
        printf("Child written.\n");
    } 
    else {
        const char *parent = "parent process message.\n";
              if (write(fd, parent  ,strlen(parent))==-1){
                printf("error in child writing"); 
                return 1;
        }
        close(fd);
        printf("Parent written.\n");
    }
    close(fd);
    return 0;
}
/*
=====OUTPUT================================================================================================================
nani@Nani:~/handson1$ ./a.out
Parent written.
Child written.
nani@Nani:~/handson1$ cat textfile_22.txt
parent process message.
Child process message.

===========================================================================================================================
*/
