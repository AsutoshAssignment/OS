/*
===========================================================================================================================
Name : 26_exe_a.c
Author : Asutosh Panda
Description : Write a program to execute an executable program.
                a. use some executable program
                b. pass some input to an executable program

Date: 1st Sept, 2025.
===========================================================================================================================
*/
#include <stdio.h>

int main() {
    execl("/bin/ls", "ls", NULL);
    return 0;
}
/*
=====OUTPUT================================================================================================================
nani@Nani:~/handson1$ ./a.out
a.out		       q16_locking.c	       q6_stdin.c
file1.txt	       q17_ticket_a.c	       q7_copy.c
file2.txt	       q17_ticket_b.c	       q8_line_by_line.c
newfile.txt	       q18_record_locking_a.c  q9_file_info.c
proc.c		       q19_time_stamp.c        source_b.txt
q10_file_write.c       q20_nice.c	       source_c.tx
q11_a_file_dup.c       q21_fork.c	       source_c.txt
q11_b_file_dup.c       q22_fork_write.c        source.txt
q11_c_file_dup.c       q23_zombie.c	       textfile_12.txt
q12_opening_mode.c     q24_orphan.c	       textfile_22.txt
q13_stdin_10_select.c  q25_parent_wait.c       textfile_q18.txt
q14_type_of_file.c     q26_exe_a.c	       textfile.txt
q15_env_variable.c     q3_create_file.c        tickets.txt
q16_locking_a.c        q4_open_file.c
q16_locking_b.c        q5_stdin.c

===========================================================================================================================
*/
