/*
===========================================================================================================================
Name : 2_proc.c
Author : Asutosh Panda
Description : Write a simple program to execute in an infinite loop at the background. Go to /proc directory and
identify all the process related information in the corresponding proc directory.
Date: 19th Aug, 2025.
===========================================================================================================================
*/

#include <unistd.h>
int main() {
    while(1) {
        sleep(1);
    }
    return 0;
}

/*
=====OUTPUT================================================================================================================
nani@Nani:~/handson1$ ./a.out &
[1] 3908
nani@Nani:~/handson1$ cat /proc/3908/status
Name:	a.out
Umask:	0002
State:	S (sleeping)
Tgid:	3908
Ngid:	0
Pid:	3908
PPid:	3223
TracerPid:	0
Uid:	1000	1000	1000	1000
Gid:	1000	1000	1000	1000
FDSize:	256
Groups:	4 24 27 30 46 125 137 1000 
NStgid:	3908
NSpid:	3908
NSpgid:	3908
NSsid:	3223
Kthread:	0
VmPeak:	    2712 kB
VmSize:	    2644 kB
VmLck:	       0 kB
VmPin:	       0 kB
VmHWM:	    1024 kB
VmRSS:	    1024 kB
RssAnon:	       0 kB
RssFile:	    1024 kB
RssShmem:	       0 kB
VmData:	      92 kB
VmStk:	     132 kB
VmExe:	       4 kB
VmLib:	    1796 kB
VmPTE:	      44 kB
VmSwap:	       0 kB
HugetlbPages:	       0 kB
CoreDumping:	0
THP_enabled:	1
untag_mask:	0xffffffffffffffff
Threads:	1
SigQ:	0/22753
SigPnd:	0000000000000000
ShdPnd:	0000000000000000
SigBlk:	0000000000000000
SigIgn:	0000000000000000
SigCgt:	0000000000000000
CapInh:	0000000000000000
CapPrm:	0000000000000000
CapEff:	0000000000000000
CapBnd:	000001ffffffffff
CapAmb:	0000000000000000
NoNewPrivs:	0
Seccomp:	0
Seccomp_filters:	0
Speculation_Store_Bypass:	thread vulnerable
SpeculationIndirectBranch:	conditional enabled
Cpus_allowed:	ffff
Cpus_allowed_list:	0-15
Mems_allowed:	00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000000,00000001
Mems_allowed_list:	0
voluntary_ctxt_switches:	11
nonvoluntary_ctxt_switches:	0
x86_Thread_features:	
x86_Thread_features_locked:
===========================================================================================================================
*/
