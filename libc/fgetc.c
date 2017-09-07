#include<libc.h>
#include<stdio.h>

int fgetc(int fd) {
	char ch;
	if(read(fd, &ch, 1) > 0)
		return (int)ch;
	return EOF;
}
