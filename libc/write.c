#include<libc.h>

int write(int fd, char* buf, int count) {
        return syscall_ici(__NR_write, fd, buf, count);
}
