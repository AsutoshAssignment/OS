/*
===========================================================================================================================
Name : 27_execute_rl_a.c
Author : Asutosh Panda
Description : 27. Write a program to execute ls -Rl by the following system calls
		a. execl

Date: 1st Sept, 2025.
===========================================================================================================================
*/
#include <stdio.h>
#include <unistd.h>

int main() {
   if( execl("/bin/ls", "ls", "-Rl", NULL)==-1)
	    printf("something went wrong");
    return 1;
}

/*
=====OUTPUT================================================================================================================

nani@Nani:~/handson1$ ./a.out
.:
total 184
-rwxrwxr-x 1 nani nani 16016 Sep  4 22:57 a.out
-rw-rw-r-- 1 nani nani   206 Sep  4 14:35 executable_program_q26.c
-rw-rw-r-- 1 nani nani    30 Sep  4 00:36 file1.txt
-rw-rw-r-- 1 nani nani    21 Sep  2 14:21 file2.txt
-rwxrwxr-x 1 nani nani     0 Sep  2 00:57 newfile.txt
-rw-rw-r-- 1 nani nani   203 Aug 18 15:54 proc.c
-rw-rw-r-- 1 nani nani  1812 Sep  2 22:49 q10_file_write.c
-rw-rw-r-- 1 nani nani  1796 Sep  3 15:09 q11_a_file_dup.c
-rw-rw-r-- 1 nani nani  1814 Sep  3 15:25 q11_b_file_dup.c
-rw-rw-r-- 1 nani nani  1847 Sep  3 15:38 q11_c_file_dup.c
-rw-rw-r-- 1 nani nani  1472 Sep  3 15:48 q12_opening_mode.c
-rw-rw-r-- 1 nani nani  1671 Sep  4 00:16 q13_stdin_10_select.c
-rw-rw-r-- 1 nani nani  1968 Sep  4 00:22 q14_type_of_file.c
-rw-rw-r-- 1 nani nani  3848 Sep  3 15:53 q15_env_variable.c
-rw-rw-r-- 1 nani nani  1593 Sep  4 00:28 q16_locking_a.c
-rw-rw-r-- 1 nani nani  1502 Sep  4 00:36 q16_locking_b.c
-rw-rw-r-- 1 nani nani     0 Sep  4 00:26 q16_locking.c
-rw-rw-r-- 1 nani nani  1515 Sep  4 01:08 q17_ticket_a.c
-rw-rw-r-- 1 nani nani  1830 Sep  4 01:30 q17_ticket_b.c
-rw-rw-r-- 1 nani nani  2708 Sep  4 01:47 q18_record_locking_a.c
-rw-rw-r-- 1 nani nani  1167 Sep  4 02:00 q19_time_stamp.c
-rw-rw-r-- 1 nani nani  1129 Sep  4 02:14 q20_nice.c
-rw-rw-r-- 1 nani nani  1061 Sep  4 10:29 q21_fork.c
-rw-rw-r-- 1 nani nani  1727 Sep  4 10:29 q22_fork_write.c
-rw-rw-r-- 1 nani nani  1062 Sep  4 10:34 q23_zombie.c
-rw-rw-r-- 1 nani nani  1226 Sep  4 11:50 q24_orphan.c
-rw-rw-r-- 1 nani nani  1477 Sep  4 11:59 q25_parent_wait.c
-rw-rw-r-- 1 nani nani  1652 Sep  4 14:32 q26_exe_a.c
-rw-rw-r-- 1 nani nani     0 Sep  4 14:33 q26_exe_b.c
-rw-rw-r-- 1 nani nani   155 Sep  4 22:57 q27_execute_rl_a.c
-rw-rw-r-- 1 nani nani   320 Sep  2 00:56 q3_create_file.c
-rw-rw-r-- 1 nani nani   345 Sep  2 00:59 q4_open_file.c
-rw-rw-r-- 1 nani nani   183 Sep  2 12:32 q5_stdin.c
-rw-rw-r-- 1 nani nani   958 Sep  2 14:17 q6_stdin.c
-rw-rw-r-- 1 nani nani   921 Sep  2 14:22 q7_copy.c
-rw-rw-r-- 1 nani nani  1246 Sep  2 14:37 q8_line_by_line.c
-rw-rw-r-- 1 nani nani  2221 Sep  2 14:59 q9_file_info.c
-rw-rw-r-- 1 nani nani    66 Sep  3 15:24 source_b.txt
-rw-rw-r-- 1 nani nani    17 Sep  3 15:37 source_c.tx
-rw-rw-r-- 1 nani nani    72 Sep  3 15:38 source_c.txt
-rw-rw-r-- 1 nani nani    63 Sep  3 15:08 source.txt
-rw-rw-r-- 1 nani nani    30 Sep  3 15:47 textfile_12.txt
-rw-r--r-- 1 nani nani    47 Sep  4 10:28 textfile_22.txt
-rw-r--r-- 1 nani nani    60 Sep  4 01:47 textfile_q18.txt
-rw-r--r-- 1 nani nani    60 Sep  4 01:45 textfile.txt
-rw-r--r-- 1 nani nani     4 Sep  4 01:28 tickets.txt
===========================================================================================================================
*/
