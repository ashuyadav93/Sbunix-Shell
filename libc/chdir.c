#include<libc.h>

int chdir(char* path) {
	return syscall1(__NR_chdir, path);
}
