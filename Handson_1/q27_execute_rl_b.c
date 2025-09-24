/*
===========================================================================================================================
Name : 27_execute_rl_b.c
Author : Asutosh Panda
Description : Write a program to execute ls -Rl by the following system calls
                b. execlp

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
a.out			  q16_locking.c		  q4_open_file.c
executable_program_q26.c  q17_ticket_a.c	  q5_stdin.c
file1.txt		  q17_ticket_b.c	  q6_stdin.c
file2.txt		  q18_record_locking_a.c  q7_copy.c
newfile.txt		  q19_time_stamp.c	  q8_line_by_line.c
proc.c			  q20_nice.c		  q9_file_info.c
q10_file_write.c	  q21_fork.c		  source_b.txt
q11_a_file_dup.c	  q22_fork_write.c	  source_c.tx
q11_b_file_dup.c	  q23_zombie.c		  source_c.txt
q11_c_file_dup.c	  q24_orphan.c		  source.txt
q12_opening_mode.c	  q25_parent_wait.c	  textfile_12.txt
q13_stdin_10_select.c	  q26_exe_a.c		  textfile_22.txt
q14_type_of_file.c	  q26_exe_b.c		  textfile_q18.txt
q15_env_variable.c	  q27_execute_rl_a.c	  textfile.txt
q16_locking_a.c		  q27_execute_rl_b.c	  tickets.txt
q16_locking_b.c		  q3_create_file.c

===========================================================================================================================
*/

