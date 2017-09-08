#include<libc.h>

int dup2(int a, int b) {
    return syscall2_ii(__NR_dup2, a, b);
}
