#include<libc.h>

int closse(int fd) {
    return _syscall1(__NR_close,fd);
}
