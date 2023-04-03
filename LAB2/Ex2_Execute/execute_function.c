#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    printf("Run command <ls -lah> after 2 seconds\n");
    sleep(2);
    execlp("ls", "ls", "-l", "-a", "-h", "/home/hieuhoangwork/work", NULL);
    // execlp("sudo", "sudo", "insmod", "/home/hieuhoangwork/work/Linux_system/00-driver-legacy/01-kernel-module/hello_module.ko", NULL);
    // execlp("sudo", "sudo", "rmmod", "/home/hieuhoangwork/work/Linux_system/00-driver-legacy/01-kernel-module/hello_module.ko", NULL);
    perror("execlp");
    // printf("hello world\n");
    return 0;
}
