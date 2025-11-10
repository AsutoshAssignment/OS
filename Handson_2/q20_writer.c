/*
==========================================================================
==================================================
Name : q20_writer.c
Author : Asutosh Panda
Descrioption:Write two programs so that both can communicate by FIFO -Use one way communication.
Date: 29th Sept, 2025
==========================================================================
==================================================
*/

#include <fcntl.h>
#include <unistd.h>


int main() {
    char msg[100];
    int fd = open("my_fifo", O_WRONLY);
    while (1) {
        write(fd, "\nThis string was written in the writer program\n", 46);
        sleep(1);
    }
    return 0;
}

/*
=====OUTPUT================================================================================================================
---------------Terminal 1 (Writer)----------------
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ mkfifo my_fifo
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ cc 20writer.c
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ ./a.out

---------------Terminal 2 (Reader)---------------
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ cc 20reader.c -o reader
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ ./reader
This string was written in the writer program
This string was written in the writer program
This string was written in the writer program
This string was written in the writer program
This string was written in the writer program
This string was written in the writer program
This string was written in the writer program
This string was written in the writer program
This string was written in the writer program
^C
===========================================================================================================================
*/
