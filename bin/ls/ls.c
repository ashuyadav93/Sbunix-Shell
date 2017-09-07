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

int list(char* fileName);

int main(int argc, char* argv[], char* envp[]) {
	
	char* fileName = ".";
	//char output[256*BUF_SIZE];
	if(argc == 1) {
		list(fileName);
		//if(output != NULL)
                //	puts(output);
	}

	if(argc > 1) {
		for(int i = 1; i < argc; i++) {
			//output[0] = '\0';
			if(argv[i][0] == '-') {
				puts("Not supported");
				continue;
			}
			if(list(argv[i]) == -1)
				continue;
			//if(output != NULL)
	     		//	puts(output);
		}
	}
        return 0;
}

int list(char* fileName) {

	char buffer[256*BUF_SIZE];
        struct linux_dirent *dir;
        int bytesRead;

	int fd = open((void*)fileName, O_RDONLY | O_DIRECTORY);
        if(fd < 0) {
        	puts("No such file or directory");
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
                        	//if(strlen(output)) {
                                //	strcat(output, "  ");
                                //}
                                puts(dir->d_name);
                        }
         		i += dir->d_reclen;
         	}
         }
         close(fd);
	return 0;
}
