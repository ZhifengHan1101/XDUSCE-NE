#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    struct stat src_st;
    int src_fd;
    FILE *dst_fd;
    char *buf;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source_file> <destination_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // 判断source file是否成功打开
    if ((src_fd = open(argv[1], O_RDONLY)) == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // 判断destination file是否成功打开
    if ((dst_fd = fopen(argv[2], "wb+")) == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    fstat(src_fd, &src_st);
    
    // buf读取source file长度，并开内存
    if((buf = (char*)malloc(sizeof(char) * src_st.st_size)) == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    // 读source file
    if(read(src_fd, buf, src_st.st_size) == -1)
    {
        perror("read");
        exit(EXIT_FAILURE);
    }

    // 写destination file
    if(fwrite(buf, sizeof(char), src_st.st_size, dst_fd) != src_st.st_size)
    {
        perror("fwrite");
        exit(EXIT_FAILURE);
    }

    // 关闭source file
    if (close(src_fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    // 关闭destination file
    if (fclose(dst_fd) == -1) {
        perror("fclose");
        exit(EXIT_FAILURE);
    }

    printf("File Copied Successfully.\n");

    return 0;
}
