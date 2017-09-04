#define _GNU_SOURCE
#include<dirent.h>
#include<string.h>
#include<stdio.h>
#include<libc.h>

struct linux_dirent {
    unsigned long  d_ino;     /* Inode number */
    unsigned long  d_off;     /* Offset to next linux_dirent */
    unsigned short d_reclen;  /* Length of this linux_dirent */
    char           d_name[];  /* Filename (null-terminated) */
                      /* length is actually (d_reclen - 2 -
                         offsetof(struct linux_dirent, d_name)) */
};

int main(int argc, char* argv[], char* envp[]) {
	int BUF_SIZE = 256*1000;
	char buffer[BUF_SIZE];
        struct linux_dirent *dir;
        int bytesRead;

	char* fileName = ".";
        int fd = syscall2(2, (void*)fileName, O_RDONLY);
        while(1) {
                bytesRead = syscall_ici(78, fd, buffer, BUF_SIZE);
                if(bytesRead < 0 )
                        exit(1);
                if(bytesRead == 0)
                        break;

                for(int i = 0; i < bytesRead;) {
			dir = (struct linux_dirent *) (buffer + i);
                        puts(dir->d_name);
			i += dir->d_reclen;
                }

        }
        return 0;
}
