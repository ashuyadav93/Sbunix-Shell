#include<libc.h>

void executecat(int argc, char* argv[]);
void readWrite(int fd);

int main(int argc, char* argv[], char* envp[]) {
	if(argc > 1) {
        	executecat(argc, argv);
	}
	return 0;
} 

void executecat(int argc, char* argv[]) {

	int fd;

	if(argc == 1) {
		while(1) {
			char ch[10];
        		while(read(0, ch, 1) >= 0) {
                		write(1, ch, 1);
        		}
		}
	} else {
		for(int i = 1; i < argc; i++) {
			fd = open(argv[i], O_RDONLY);
			if(fd < 0) {
				exit(1);
        		}
			readWrite(fd);
			close(fd);
		}
	}
}

void readWrite(int fd) {
	char ch = ' ';
	while(read(fd, &ch, 1) > 0) {
                write(1, &ch, 1);
        }
}
