#include<libc.h>

void executecat(char* file);

int main(int argc, char* argv[], char* envp[]) {
        executecat("/home/aksinghal/workdir/testCat.txt");
	return 0;
} 

void executecat(char *file) {

	int fd;
	char ch = ' ';
	fd = open(file, O_RDONLY);
	if(fd < 0) {
		exit(1);
	}

    	while(read(fd, &ch, 1) > 0) {
		write(1, &ch, 1);
	}
	close(fd);
}
