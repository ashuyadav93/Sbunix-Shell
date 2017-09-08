#include<libc.h>

void getcwd(char* buf, int size) {
 	__asm__ volatile
        (
                "syscall"
                : "=r"(buf)
                : "0"(__NR_getcwd), "D"(buf), "S"(size)
                : "cc","rcx", "r11", "memory"
        );
}
