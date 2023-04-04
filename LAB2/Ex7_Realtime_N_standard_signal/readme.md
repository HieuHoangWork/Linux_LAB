Realtime signal và standard signal là gì? Phân biệt sự khác nhau giữa chúng.

Standard signal là các tín hiệu được định nghĩa trước trong hệ thống và có các hành động mặc định được xác định trước. Chỉ có một instance (instance - một lần gửi tín hiệu) của một standard signal cụ thể có thể được đưa vào hàng đợi tại một thời điểm. Nếu nhiều instance của cùng một standard signal được gửi đến một process trong khi tín hiệu đó đang bị block, thì chỉ có một instance sẽ được đưa vào hàng đợi.

Real-time signal là một lớp mới của tín hiệu được giới thiệu bởi tiêu chuẩn POSIX. Số tín hiệu tương ứng nằm trong khoảng từ 32 đến 63. Sự khác biệt chính so với standard signal là các real-time signal cùng loại có thể được đưa vào hàng đợi. Điều này đảm bảo rằng nhiều tín hiệu được gửi sẽ được nhận.


Một số khác biệt khác giữa real-time signal so với standard signal bao gồm:

-> Nếu tín hiệu được gửi bằng sigqueue, một giá trị đi kèm (hoặc là một số nguyên hoặc là một con trỏ) có thể được gửi cùng với tín hiệu.
-> Các real-time signal được giao theo thứ tự được đảm bảo. Nhiều real-time signal cùng loại được giao theo thứ tự chúng được gửi. Nếu các real-time signal khác nhau được gửi đến một process, chúng sẽ được giao bắt đầu từ tín hiệu có số thấp nhất (tức là tín hiệu có số thấp có ưu tiên cao nhất).

Ví dụ:

/* Standard signal */
Một ví dụ về standard signal là tín hiệu SIGINT. Tín hiệu này được gửi đến một process khi người dùng nhấn tổ hợp phím Ctrl-C trong terminal. Hành động mặc định của SIGINT là kết thúc process.

#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void sigint_handler(int signum) {
    printf("Caught SIGINT\n");
}

int main(void) {
    // Đăng ký trình xử lý tín hiệu cho SIGINT
    signal(SIGINT, sigint_handler);

    // Vòng lặp vô hạn
    while (1) {
        printf("Sleeping for 1 second...\n");
        sleep(1);
    }

    return 0;
}

Trong ví dụ trên, chúng ta định nghĩa một hàm sigint_handler để xử lý tín hiệu SIGINT. Sau đó, chúng ta sử dụng hàm signal để đăng ký hàm này làm trình xử lý tín hiệu cho SIGINT.
Khi chạy chương trình và nhấn Ctrl-C, bạn sẽ thấy dòng chữ “Caught SIGINT” được in ra màn hình thay vì kết thúc chương trình.

/* Realtime signal */

#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void signal_handler(int signum, siginfo_t *info, void *context) {
    printf("Received signal %d with value %d\n", signum, info->si_value.sival_int);
}

int main(void) {
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

Trong ví dụ trên, chúng ta định nghĩa một hàm signal_handler để xử lý real-time signal SIGRTMIN. Sau đó, chúng ta sử dụng hàm sigaction để đăng ký hàm này làm trình xử lý tín hiệu cho SIGRTMIN.
Tiếp theo, chúng ta sử dụng hàm sigprocmask để block SIGRTMIN và sau đó sử dụng hàm sigqueue để gửi hai instance của SIGRTMIN với các giá trị khác nhau (42 và 84) đến process hiện tại.
Cuối cùng, chúng ta sử dụng hàm sigprocmask để unblock SIGRTMIN. Khi chạy chương trình, bạn sẽ thấy hai dòng chữ được in ra màn hình cho biết hai instance của SIGRTMIN đã được nhận và xử lý.

/* Note */

union sigval là một kiểu dữ liệu được định nghĩa trong thư viện signal.h và được sử dụng để chứa giá trị đi kèm với một real-time signal. Nó là một union có hai thành phần: sival_int và sival_ptr, cho phép bạn lưu trữ một giá trị số nguyên hoặc một con trỏ.

Trong ví dụ về real-time signal mà tôi đã đưa ra ở trên, chúng ta sử dụng union sigval để lưu trữ giá trị đi kèm với tín hiệu SIGRTMIN:

// Gửi SIGRTMIN với giá trị 42
union sigval value;
value.sival_int = 42;
sigqueue(getpid(), SIGRTMIN, value);

// Gửi SIGRTMIN với giá trị 84
value.sival_int = 84;
sigqueue(getpid(), SIGRTMIN, value);
Trong đoạn code trên, chúng ta khai báo một biến value có kiểu union sigval và sau đó gán giá trị cho thành phần sival_int của nó. Sau đó, chúng ta sử dụng hàm sigqueue để gửi tín hiệu SIGRTMIN cùng với giá trị đi kèm đến process hiện tại.