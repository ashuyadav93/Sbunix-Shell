#include<libc.h>

int dup2(int a, int b) {
    return syscall2(__NR_dup2, (void*)&a, b);
}
