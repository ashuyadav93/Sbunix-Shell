#include<libc.h>
#include<sys/defs.h>

pid_t waitpid(pid_t pid, int *wstatus, int options) {
	int ret;
        __asm__ volatile
        (
                "syscall"
                : "=a" (ret)
                : "0"(__NR_wait4), "D"(pid), "S"(wstatus), "d"(options), "c"(NULL)
                : "cc","r11","memory"
        );
        return (pid_t)ret;
}
