#include<libc.h>

int getdents(int fd, char* dirp, int count) {
	return syscall_ici(__NR_getdents, fd, dirp, count);
}
