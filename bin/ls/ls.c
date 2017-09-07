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
	
	char buffer[256*BUF_SIZE];
        struct linux_dirent *dir;
        int bytesRead;
	char* fileName = ".";
	char output[256*BUF_SIZE];

	if(argc > 1) {
		fileName = argv[argc-1];
	}
        int fd = open((void*)fileName, O_RDONLY | O_DIRECTORY);
	if(fd < 0) {
		return -1;
	}
        while(1) {
                bytesRead = getdents(fd, buffer, 256*BUF_SIZE);
                if(bytesRead < 0 )
                        exit(1);
                if(bytesRead == 0)
                        break;

                for(int i = 0; i < bytesRead;) {
			dir = (struct linux_dirent *) (buffer + i);
			if(dir->d_name[0] != '.') {
				if(strlen(output)) {
					strcat(output, "  ");
				}
                        	strcat(output, dir->d_name);
			}
			i += dir->d_reclen;
                }

        }
	if(output != NULL)
		puts(output);

        return 0;
}
