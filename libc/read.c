#include<libc.h>

int read(int fd, char* buffer, int count) {
    return syscall_ici(0, fd, buffer, count);
}
