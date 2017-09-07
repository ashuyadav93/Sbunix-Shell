#ifndef _LIBC_H
#define _LIBC_H
#define __NR_read 0
#define __NR_write 1
#define __NR_open 2
#define __NR_close 3
#define __NR_fork 57
#define __NR_execve 59
#define __NR_exit 60
#define __NR_wait4 61
#define __NR_getdents 78
#define __NR_getcwd 79
#define __NR_chdir 80
#define __NR_pipe 22
#define __NR_dup2 33
#define BUF_SIZE 1024
#define O_RDONLY 00000000
#define O_DIRECTORY 00200000

#include<sys/defs.h>

int main(int argc, char *argv[], char* envp[]);

int read(int fd, char* buf, int count);

int write(int fd, char* buf, int count);

int open(char* pathname, int flags);

int close(int fd);

int fork();

int execve(char* filename, char* argv[], char* envp[]);

void exit(int status);

pid_t waitpid(pid_t pid, int *wstatus, int options);

int getdents(int fd, char* dirp, int count);

void getcwd(char* buf, int size);

int chdir(char* path);

int pipe(int* fd);

int dup2(int a, int b);

char* strcpy(char* d, const char* s);

char* strcat(char* d, const char* s);

int fgetc(int fd);

int syscall0(long syscall);

int syscall1(long syscall, void* arg1);

uint64_t syscall2(long syscall, void* arg1, int arg2);

int syscall3(long syscall, void* arg1, void* arg2, void* arg3);

int syscall_ici(long syscall, int arg1, char* arg2, int arg3);

#endif
