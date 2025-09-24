/*
===========================================================================================================================
Name : 18_record_locking_a.c
Author : Asutosh Panda
Description : Write a program to simulate online ticket reservation. Implement write lock. Write a program to open a file, store a ticket number and exit. Write a separate program, to
open the file, implement write lock, read the ticket number, increment the number and print the new ticket number then close the file.
Date: 27th Aug, 2025.
===========================================================================================================================
*/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h> 

int apply_lock(int file_desc, off_t pos, int size, int type) {
    struct flock fl;
    fl.l_type= type;
    fl.l_whence = SEEK_SET;
    fl.l_start= pos;
    fl.l_len= size;

    return fcntl(file_desc, F_SETLKW, &fl);
}

void handle_record(int file_desc, int rec_index, int lock_mode, const char *new_data) {
    off_t start= rec_index * 20; 

    if (apply_lock(file_desc, start, 20, lock_mode) == -1) {
        perror("Lock failed");
        exit(1);
    }

    if (lock_mode== F_WRLCK) {
        lseek(file_desc, start, SEEK_SET);
        write(file_desc, new_data, 20);
    } else {
        char buf[21];  
        lseek(file_desc, start, SEEK_SET);
        read(file_desc, buf, 20);
        buf[20] = '\0';
        printf("Record %d => %s\n", rec_index + 1, buf);
    }

    apply_lock(file_desc, start, 20, F_UNLCK);
}

int main() {
    const char *fname = "textfile_q18.txt";
    int fd = open(fname, O_RDWR | O_CREAT, 0644);
    if (fd < 0) {
        perror("open failed");
        exit(1);
    }

    char records[3][20] = {
        "Original Rec 1 ",
        "Original Rec 2 ",
        "Original Rec 3 "
    };

    for (int idx = 0; idx < 3; idx++) {
        lseek(fd, idx * 20, SEEK_SET);
        write(fd, records[idx], 20);
    }

    handle_record(fd, 0, F_WRLCK, "Changed Rec 1");
    handle_record(fd, 0, F_RDLCK, NULL);
    handle_record(fd, 1, F_RDLCK, NULL);
    handle_record(fd, 2, F_WRLCK, "Changed Rec 3");
    handle_record(fd, 2, F_RDLCK, NULL);

    close(fd);
    return 0;
}

/*
=====OUTPUT================================================================================================================
nani@Nani:~/handson1$ ./a.out
Record 1 => Changed Rec 1
Record 2 => Original Rec 2 
Record 3 => Changed Rec 3
nani@Nani:~/handson1$ cat textfile_q18.txt
Changed Rec 1ChangeOriginal Rec 2 Changed Rec 3
===========================================================================================================================
*/
