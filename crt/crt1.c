#include <sys/defs.h>
#include <libc.h>

void _start(void) {

	int argc;
        char** argv = NULL;
        char** envp = NULL;
        uint64_t* add = NULL; 
	
	__asm__(
		"movq %%rsp, %0 \n\t"
		:"=r"(add)
		:
		:"cc", "memory"
	);

        argc = *(add + 1);
	argv = (char**)(add + 2);
        envp = (char**)(argv + argc + 1);

        int a = main(argc, argv, envp);

	exit(a);
}
