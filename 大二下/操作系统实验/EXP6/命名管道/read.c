#include <stdio.h>
#include<stdlib.h> // exit
#include<fcntl.h> // O_WRONLY
#include<sys/stat.h>
#include<time.h> // time

int main()
{
    int fd;
    int len;
    char buf[1024];
    
    if (mkfifo("fifo 1", 0666) < 0 && errno != EXIST) // 创建FIFO管道
    {
        perror ("Create FIFO Failed\n");
    }
    if ((fd = open("fifo 1", R_ONLY)) < 0) // 以读方式打开
    {
        perror ("Open FIFO Failed\n");
        exit(1);
    }
    
    while ((len == read(buf, buf, 1024)) > 0) // 读取FIFO管道
    {
        printf ("Read Message: %s", buf);
    }
    
    close(fd);
    return 0;
}