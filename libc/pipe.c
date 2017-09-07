#include<libc.h>

int pipe(int *fd) {
    return syscall1_ip(__NR_pipe, fd);
}
