/*
===========================================================================================================================
Name : 17_ticket_b.c
Author : Asutosh Panda
Description : Write a program to simulate online ticket reservation. Implement write lock. Write a program to open a file, store a ticket number and exit. Write a separate program, to
open the file, implement write lock, read the ticket number, increment the number and print the new ticket number then close the file.
Date: 26th Aug, 2025.
===========================================================================================================================
*/
#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>

int main()
{
	struct flock lock;
	char tn[10];
	int fd = open("tickets.txt", O_RDWR);
	if(fd == -1){
		printf("Error\n");
		return 1;
	}

	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_pid = getpid();
	if (fcntl(fd, F_SETLKW, &lock)==-1){
		printf("Error acquiring write lock\n");
		return 1;
	}

	lseek(fd, 0, SEEK_SET);
	if (read(fd, &tn, sizeof(tn))==-1){
		printf("Error reading file\n");
		return 1;
	}
	printf("Current ticket number: %s\n", tn);
	int new = atoi(tn)+1;
	lseek(fd, 0, SEEK_SET);
	if (write(fd, &new, sizeof(new))==-1)
	{
		printf("Error writing file\n");
		return 1;
	}
	printf("New ticket number: %d\n", new);

	lock.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &lock);
	close(fd);
	return 0;
}
/*
=====OUTPUT================================================================================================================
nani@Nani:~/handson1$ ./a.out
Current ticket number: 999

New ticket number: 1000

===========================================================================================================================
*/
