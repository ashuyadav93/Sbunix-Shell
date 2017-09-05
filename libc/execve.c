#include<libc.h>

int execve(char* filename, char* argv[], char* envp[]) {
        return syscall3(__NR_execve, (void*)filename, (void*)argv, (void*)envp);
}
