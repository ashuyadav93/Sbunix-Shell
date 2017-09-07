#include<libc.h>

int close(int fd) {
    return syscall1(__NR_close, (void*)&fd);
}
