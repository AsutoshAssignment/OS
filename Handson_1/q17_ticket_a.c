/*
===========================================================================================================================
Name : 17_ticket_a.c
Author : Asutosh Panda
Description : Write a program to simulate online ticket reservation. Implement write lock. Write a program to open a file, store a ticket number and exit. Write a separate program, to
open the file, implement write lock, read the ticket number, increment the number and print the new ticket number then close the file.
Date: 26th Aug, 2025.
===========================================================================================================================
*/
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>

int main(){
	char tn[10];
	int fd = open("tickets.txt", O_RDWR | O_CREAT, 0644);
	if (fd==-1){
		printf("Error\n");
		return 1;
	}
	
	printf("Enter ticket number\n");
	scanf("%s", tn);
	strcat(tn,"\n");
	if (write(fd, &tn, sizeof(tn))==-1){
		printf("Error while writing the ticket number\n");
		close(fd);
		return 1;
	}
	printf("Ticket number has been stored succesfully.\n", tn);
	close(fd);
	return 0;
}
/*
=====OUTPUT================================================================================================================
nani@Nani:~/handson1$ ./a.out
Enter ticket number
999
Ticket number has been stored succesfully.
nani@Nani:~/handson1$ cat tickets.txt
999
===========================================================================================================================
*/
