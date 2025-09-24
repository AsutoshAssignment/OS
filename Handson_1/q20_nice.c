/*
===========================================================================================================================
Name : 20_nice.c
Author : Asutosh Panda
Description : Find out the priority of your running program. Modify the priority with nice command.
Date: 27th Aug, 2025.
===========================================================================================================================
*/

#include <stdio.h>
#include <unistd.h>

int main() {
    while (1) {
        
    }
    return 0;
}
/*
=====OUTPUT================================================================================================================
nani@Nani:~/handson1$ ./a.out &
[1] 5970
nani@Nani:~/handson1$ ps -o pid,pri,ni,comm -p 5970
    PID PRI  NI COMMAND
   5970  19   0 a.out
nani@Nani:~/handson1$ kill 5970
nani@Nani:~/handson1$ nice -n 9 ./a.out &
[2] 5980
[1]   Terminated              ./a.out
nani@Nani:~/handson1$ ps -o pid,pri,ni,comm -p 5980
    PID PRI  NI COMMAND
   5980  10   9 a.out
===========================================================================================================================
*/
