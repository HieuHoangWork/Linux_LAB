#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void signal_handler(int signum, siginfo_t *info, void *context)
{
    printf("Received signal %d with value %d\n", signum, info->si_value.sival_int);
}

int main(void)
{
    struct sigaction action;
    sigset_t mask;

    // Khởi tạo cấu trúc sigaction
    action.sa_sigaction = signal_handler;
    action.sa_flags = SA_SIGINFO;
    sigemptyset(&action.sa_mask);

    // Đăng ký trình xử lý tín hiệu cho SIGRTMIN
    sigaction(SIGRTMIN, &action, NULL);

    // Khởi tạo tập hợp tín hiệu rỗng
    sigemptyset(&mask);

    // Thêm SIGRTMIN vào tập hợp
    sigaddset(&mask, SIGRTMIN);

    // Block SIGRTMIN
    sigprocmask(SIG_BLOCK, &mask, NULL);

    // Gửi SIGRTMIN với giá trị 42
    union sigval value;
    value.sival_int = 42;
    sigqueue(getpid(), SIGRTMIN, value);

    // Gửi SIGRTMIN với giá trị 84
    value.sival_int = 84;
    sigqueue(getpid(), SIGRTMIN, value);

    // Unblock SIGRTMIN
    sigprocmask(SIG_UNBLOCK, &mask, NULL);

    return 0;
}