Điều gì xảy ra khi chúng ta sử dụng họ hàm execute (execl/exevp/exepv)? Giải thích và viết một chương trình để xác minh câu trả lời. Sau đó hãy cho biết bản chất khi lệnh system() hoạt động như thế nào.

Answer:

1. Khi sử dụng các hàm execute như execl(), execvp(), execv(), chúng thực hiện thay thế quá trình hiện tại bằng một chương trình mới. 

Nếu thực thi các câu lệnh sau lời gọi họ hàm execute thì chúng sẽ không được thực thi vì phần mã, dữ liệu, heap và stack của quá trình hiện tại sẽ bị thay thế bằng các thành phần tương ứng của chương trình mới. 
Trừ khi các hàm execute này bị lỗi và chúng sẽ thực thi được các câu lệnh tiếp theo sau nó.

Ví dụ chương trình main.c sau:

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    printf("Run command <ls -lah> after 2 seconds\n");
    sleep(2);
    execlp("ls", "ls", "-l", "-a", "-h", "/home/hieuhoangwork/work", NULL);
    perror("execlp");
    // printf("hello world\n");
    return 0;
}

Khi chạy chương trình trên output của lệnh "ls -l" sẽ được in ra màn hình. Nghĩa là quá trình hiện tại đã được thay thế bằng chương trình "ls"

2. Lệnh system() hoạt động bằng cách gọi shell để thực thi một lệnh. Nó tạo một quá trình con để chạy shell và chờ cho quá trình con đó kết thúc. Ví dụ, khi gọi system("ls -l"), nó sẽ tạo một quá trình con để chạy lệnh /bin/sh -c "ls -l". Điều này có nghĩa là lệnh system() sử dụng shell để phân tích cú pháp và thực thi lệnh được chỉ định.