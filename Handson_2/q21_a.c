/*
===========================================================================================================================
Name : q21_a.c
Author : Asutosh Panda
Description : Write two programs so that both can communicate by FIFO -Use two way communications.
Date: 28th Sep, 2025
===========================================================================================================================
*/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    char msg[100];
    int fd1 = open("fifo_first", O_WRONLY);
    int fd2 = open("fifo_second", O_RDONLY);
    while (1) {
        printf("Me: ");
        fgets(msg, 100, stdin);
        write(fd1, msg, 100);
        read(fd2, msg, 100);
        printf("User2: %s", msg);
    }
    return 0;
}
/*
=====OUTPUT================================================================================================================
---------------Terminal 1 (Writer)----------------

asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ mkfifo fifo_first
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ mkfifo fifo_second
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ cc 21_user1.c
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ ./a.out
Me: Hello from user 1
User2: Response from user 2
Me: Good luck
User2: thanks
Me: ^C

---------------Terminal 2 (Reader)---------------
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ cc 21_user2.c -o u2
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ ./u2
Me: Hello from user 1
User2: Response from user 2
Me: Good luck
User2: thanks
Me: ^C
===========================================================================================================================
*/
