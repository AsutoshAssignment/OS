/*
==========================================================================================================================
Name : 16_locking_b.c
Author : Asutosh Panda
Description : Write a program to perform mandatory locking.
            a. Implement write lock
            b. Implement read lock
Date: 26th Aug, 2025.
===========================================================================================================================
*/

#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

int main(){
	struct flock lock;
	int fd = open("file1.txt", O_RDWR);
        if(fd==-1){
                printf("Error opening file\n");
        }
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_pid = getpid();
	if (fcntl(fd, F_SETLKW, &lock)==-1)
        {
                printf("Locking failed\n");
        }
	printf("Write lock acquired\n");
	printf("Press enter to unlock\n");
	getchar();
	lock.l_type = F_UNLCK;
	if (fcntl(fd, F_SETLK, &lock)==-1)
        {
                printf("Lock release failed\n");
        }
        printf("Successfully unlocked\n");
        close(fd);
	return 0;
}
/*
=====OUTPUT================================================================================================================
nani@Nani:~/handson1$ ./a.out
Read lock acquired
Press enter to unlock

Successfully unlocked

===========================================================================================================================
*/
