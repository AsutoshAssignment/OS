/*
===========================================================================================================================
Name : q2.c
Author : Asutosh Panda
Description : Write a program to print the system resource limits. Use getrlimit system call.
Date: 19th Sep, 2025
===========================================================================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>

void rsrc_lmt(int resource, const char *resource_name) {
    struct rlimit limit;
    if (getrlimit(resource, &limit) == 0) {
        printf("%s:\n", resource_name);
        printf("Soft limit: %ld\n", limit.rlim_cur);
        printf("Hard limit: %ld\n\n", limit.rlim_max);
    } else {
        printf("getrlimit failed");
    }
}

int main() {
    rsrc_lmt(RLIMIT_CPU, "CPU time limit");
    rsrc_lmt(RLIMIT_FSIZE, "File size limit");
    rsrc_lmt(RLIMIT_STACK, "Stack size limit");
    rsrc_lmt(RLIMIT_NOFILE, "Number of open files limit");
    rsrc_lmt(RLIMIT_NPROC, "Number of processes limit");

    return 0;
}
/*
=====OUTPUT================================================================================================================
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ cc 2.c
asutosh@asutosh-Nitro-AN515-43:~/Documents/OS/Handson_2$ ./a.out
CPU time limit:
Soft limit: -1
Hard limit: -1

File size limit:
Soft limit: -1
Hard limit: -1

Stack size limit:
Soft limit: 8388608
Hard limit: -1

Number of open files limit:
Soft limit: 1024
Hard limit: 1048576

Number of processes limit:
Soft limit: 62040
Hard limit: 62040
===========================================================================================================================
*/
