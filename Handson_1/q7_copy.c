/*
===========================================================================================================================
Name : 7_copy.c
Author : Asutosh Panda
Description : Write a program to copy file1 into file2 ($cp file1 file2).
Date: 20th Aug, 2025.
=============================================================================================================
*/

#include <stdlib.h>
int main() {
    system("cp file1.txt file2.txt");
    return 0;
}
/*
=====OUTPUT==================================================================================================
nani@Nani:~/handson1$ cat file1.txt
Hello this is file 1
nani@Nani:~/handson1$ cat file2.txt
Hello this is file 2.
nani@Nani:~/handson1$ cc q7_copy.c
nani@Nani:~/handson1$ ./a.out
nani@Nani:~/handson1$ cat file2.txt
Hello this is file 1
=============================================================================================================
*/
