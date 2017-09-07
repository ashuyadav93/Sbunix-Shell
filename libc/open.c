#include<libc.h>

int open(char *pathname, int flags) {
    return (int)syscall2(2, (void*)pathname, flags);
}

