#ifndef _LIBC_H
#define _LIBC_H
#define __NR_read 0
#define __NR_write 1
#define __NR_open 2
#define __NR_execve 59
#define __NR_exit 60
#define __NR_chdir 80
#define __NR_pipe 22
#define __NR_dup2 33
#define __NR_close 3
#define O_RDONLY        00000000

int main(int argc, char *argv[], char* envp[]);

int read(int fd, char* buf, int count);

int write(int fd, char* buf, int count);

int open(char* pathname, int flags);

int execve(char* filename, char* argv[], char* envp[]);

void exit(int status);

int chdir(char* path);

int pipe(int* fd);

int close(int fd):

int dup2(int a, int b);

int syscall1(long syscall, void* arg1);

int syscall2(long syscall, void* arg1, int arg2);

int syscall3(long syscall, void* arg1, void* arg2, void* arg3);

int syscall_ici(long syscall, int arg1, char* arg2, int arg3);

#endif
