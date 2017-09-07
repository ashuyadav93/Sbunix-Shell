#include<libc.h>
#include<stdio.h>
#include<string.h>

int main(int argc, char* argv[], char* envp[]) {
	char output[256*BUF_SIZE];
	for(int k = 1; k < argc; k++) {
		if(strlen(output)) {
			strcat(output, " ");
		}
        	if(argv[k][0] == '$') {
        		for(int i = 0; *envp[i] != '\0'; i++) {
				int j = 0;
                        	for(j = 0; (argv[k][j+1] == envp[i][j]) && (envp[i][j] != '=') && argv[k][j+1] != '\0'; j++);
                        	if(j > 0) {
                        		char* envVal = envp[i] + j + 1;
                                	strcat(output, envVal);
                                	break;
                        	}
                	}
        	} else {
        		strcat(output, argv[k]);
        	}
	}
	if(!strlen(output))
		strcpy(output, "");
	puts(output);
        return 0;
}
