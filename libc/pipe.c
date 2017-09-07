#include<libc.h>

int pipe(int *fd) {
    return syscall1(__NR_pipe, (void*)fd);
}
