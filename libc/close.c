#include<libc.h>

int close(int fd) {
    return syscall1_i(__NR_close, fd);
}
