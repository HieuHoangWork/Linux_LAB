#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void sigint_handler(int signum)
{
    printf("Caught SIGINT\n");
}

int main(void)
{
    // Đăng ký trình xử lý tín hiệu cho SIGINT
    signal(SIGINT, sigint_handler);

    // Vòng lặp vô hạn
    while (1)
    {
        printf("Sleeping for 1 second...\n");
        sleep(1);
    }

    return 0;
}