//#include <stdlib.h>
#include <sys/defs.h>
#include <libc.h>

void _start(void) {

       int argc = 1;
       char** argv = NULL;
       char** envp = NULL;
       uint64_t* add = NULL; 

	__asm__(
		"movq %%rsp, %0 \n\t"
		:"=r"(add)
		:
		: 
	);

        argc = *(add + 9);
	argv = (char **)(add + 6 + sizeof(argc));
        envp = (char **)(argv + argc + 1);  
	while(argc >= 0) {
		char ch[] = {(char)(argc%10 + '0')};
        	write(1, ch, 1);
		argc = argc/10;
		if(argc == 0)
			break;
	}
        puts(argv[1]);

        //write(1,argv[1],1000);
        puts("\n");
        int a = main(argc, argv, envp);
	exit(a);
}
