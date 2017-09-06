#include<libc.h>

int close(int fd) {
    return _syscall1(__NR_close,fd);
}
