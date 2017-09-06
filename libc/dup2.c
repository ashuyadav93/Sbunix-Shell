#include<libc.h>

int dup2(int a, int b) {
    return syscall1(__NR_dup2, a, b);
}
