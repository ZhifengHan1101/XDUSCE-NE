#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    struct stat src_st;
    int src_fd, dst_fd;
    char *buf;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source_file> <destination_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // 判断source file是否成功打开
    if ((src_fd = open(argv[1], O_RDONLY)) == -1) {
        perror("Open Source File Error");
        exit(EXIT_FAILURE);
    }

    // 判断destination file是否成功打开
    if ((dst_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1) {
        perror("Open Destination File Error");
        exit(EXIT_FAILURE);
    }

    fstat(src_fd, &src_st);
    
    // buf读取source file长度，并开内存
    if((buf = (char*)malloc(sizeof(char) * src_st.st_size)) == NULL)
    {
        perror("Malloc Error");
        exit(EXIT_FAILURE);
    }

    // 读source file
    if(read(src_fd, buf, src_st.st_size) == -1)
    {
        perror("Read Source File Error");
        exit(EXIT_FAILURE);
    }

    // 写destination file
    if(write(dst_fd, buf, src_st.st_size) != src_st.st_size)
    {
        perror("Write Destination File Error");
        exit(EXIT_FAILURE);
    }

    // 关闭source file
    if (close(src_fd) == -1) {
        perror("Close Source File Error");
        exit(EXIT_FAILURE);
    }

    // 关闭destination file
    if (close(dst_fd) == -1) {
        perror("Close Destination File Error");
        exit(EXIT_FAILURE);
    }

    printf("File Copied Successfully.\n");

    return 0;
}
