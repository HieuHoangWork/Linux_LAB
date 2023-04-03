#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int8_t read_buf[1024];
    int8_t write_buf[1024] = "Debug with strace\n";

    int fd = open("file.txt", O_RDWR | O_CREAT, 0666);

    if (fd < 0)
    {
        printf("Error opening file\n");
        return 0;
    }
    write(fd, write_buf, strlen(write_buf) + 1);

    // lseek(fd, 0, SEEK_SET);

    // pread(fd, read_buf, 1024, 0);
    read(fd, read_buf, 1024);

    printf("Data = %s\n", read_buf);
    close(fd);

    return 0;
}