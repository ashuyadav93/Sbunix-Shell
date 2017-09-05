#include<libc.h>

int syscall1(long syscall, void* arg1) {
        int ret;
        __asm__ volatile
        (
                "syscall"
                : "=a" (ret)
                : "0"(syscall), "D"(arg1)
                : "cc","rcx","memory"
        );
        return ret;
}

int syscall2(long syscall, void* arg1, int arg2) {
        int ret;
        __asm__ volatile
        (
                "syscall"
                : "=a" (ret)
                : "0"(syscall), "D"(arg1), "S"(arg2)
                : "cc","rcx","r11","memory"
        );
        return ret;
}

int syscall_ici(long syscall, int arg1, char* arg2, int arg3) {
        int ret;
        __asm__ volatile
        (
                "syscall"
                : "=a" (ret)
                : "0"(syscall), "D"(arg1), "S"(arg2), "d"(arg3)
                : "cc","rcx","r11","memory"
        );
        return ret;
}

int syscall3(long syscall, void* arg1, void* arg2, void* arg3) {
        int ret;
        __asm__ volatile
        (
                "syscall"
                : "=a" (ret)
                : "0"(syscall), "D"(arg1), "S"(arg2), "d"(arg3)
                : "cc","rcx","r11","memory"
        );
        return ret;
}
