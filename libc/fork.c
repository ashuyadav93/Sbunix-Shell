#include<libc.h>

int fork() {
	return syscall0(__NR_fork);
}
