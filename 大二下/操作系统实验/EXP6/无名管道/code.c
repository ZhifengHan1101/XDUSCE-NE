#include <stdio.h>
#include <stdlib.h>
int main()
{
    int fd[2]; // 两个文件描述符
    pid_t pid;
    char buff[20];
    if (pipe(fd) < 0)
    {
        printf("Create Pipe Error!\n");
    }
    if ((pid = fork()) < 0) // 创建子进程
    {
        printf("Fork Error!\n");
    }
    else if (pid > 0) // 父进程
    {
        close(fd[0]); // 关闭读端
        write(fd[1], "Hello World\n", 12);
    }
    else
    {
        close(fd[1]); // 关闭写端
        read(fd[0], buff, 20);
        printf ("%s", buff);
    }
    return 0;
}