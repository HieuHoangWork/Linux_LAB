#include <signal.h>
#include <stdio.h>
#include <unistd.h>

/*NSIG là một hằng số được định nghĩa trong thư viện signal.h và biểu diễn số lượng tín hiệu được hỗ trợ bởi hệ thống.
Giá trị của NSIG phụ thuộc vào cách triển khai của hệ điều hành và thường lớn hơn số lượng tín hiệu thực tế được hỗ trợ.

NSIG thường được sử dụng để khởi tạo mảng hoặc vòng lặp để duyệt qua tất cả các tín hiệu được hỗ trợ bởi hệ thống.*/

void print_signal_mask()
{
    sigset_t mask;
    sigprocmask(SIG_BLOCK, NULL, &mask);

    printf("Signal mask: ");
    for (int i = 1; i < NSIG; i++)
    {
        if (sigismember(&mask, i))
        {
            printf("%d ", i);
        }
    }
    printf("\n");
}

void main(void)
{
    sigset_t mask;

    // Khởi tạo tập hợp tín hiệu rỗng
    sigemptyset(&mask);

    // Thêm SIGINT vào tập hợp
    sigaddset(&mask, SIGINT);

    // Block SIGINT
    sigprocmask(SIG_BLOCK, &mask, NULL);

    // In ra signal mask của process hiện tại
    print_signal_mask();
    printf("PID proccess = %d\n", getpid());

    while (1)
        ;
}