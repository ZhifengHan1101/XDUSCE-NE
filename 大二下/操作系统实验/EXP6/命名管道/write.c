#include<stdio.h>
#include<stdlib.h> // exit
#include<fcntl.h> // O_WRONLY
#include<sys/stat.h>
#include<time.h> // time

int main()
{
    int fd;
    int n, i;
    char buf[1024];
    time_t tp;

    printf("I am %d process.\n", getpid()); // 说明进程ID

    if((fd = open("fifo1", O_WRONLY)) < 0) // 以写打开一个FIFO
    {
        perror("Open FIFO Failed");
        exit(1);
    }

    for(i=0; i<10; ++i)
    {
        time(&tp); // 取系统当前时间
        n=sprintf(buf,"Process %d's time is %s",getpid(),ctime(&tp));
        printf("Send message: %s", buf); // 打印
        if(write(fd, buf, n+1) < 0) // 写入到FIFO中
        {
            perror("Write FIFO Failed");
            close(fd);
            exit(1);
        }
        sleep(1); // 休眠1秒
    }
    close(fd); // 关闭FIFO文件
    return 0;
}