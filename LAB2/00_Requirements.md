I. Process
BT1. Giả sử rằng một parent process đã thiết lập một handler cho SIGCHLD và cũng block tín hiệu này. Sau đó, một trong các child process của nó thoát ra và parent process sau đó thực hiện wait() để thu thập trạng thái của child process. Điều gì xảy ra khi parent process bỏ chặn SIGCHLD?  Viết một chương trình để xác minh câu trả lời. 
BT2. Điều gì xảy ra khi chúng ta sử dụng họ hàm execute (execl/exevp/exepv)? Giải thích và viết một chương trình để xác minh câu trả lời. Sau đó hãy cho biết bản chất khi lệnh system() hoạt động như thế nào.
BT3. Debug là một công việc quan trọng trong việc lập trình do đó hãy tìm hiểu về segmentation fault, core dumped và cho biết chúng là gì? Viết một chương trình tái hiện lại lỗi. Sau khi tái hiện thành công, tìm hiểu về gdb và trình bày các bước fix cho lỗi này.
II. Thread
BT1. Viết một chương trình để chứng minh rằng các thread khác nhau trong cùng một process có thể có các tập hợp signal đang chờ xử lý (set of pending signal) khác nhau,được trả về bằng sigpending(). Bạn có thể làm điều này bằng cách sử dụng pthread_kill() để gửi các tín hiệu khác nhau đến hai thread khác nhau đã bị block các tín hiệu này, và sau với mỗi thread gọi sigpending() hãy hiển thị thông tin về các tín hiệu đang chờ xử lý.
III. Signal
BT1. Viết chương trình block tín hiệu SIGINT và sau đó in ra signal mask của process hiện tại.
BT2. Realtime signal và standard signal là gì? Phân biệt sự khác nhau giữa chúng.


