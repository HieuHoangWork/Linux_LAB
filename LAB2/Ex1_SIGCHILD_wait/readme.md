Giả sử rằng một parent process đã thiết lập một handler cho SIGCHLD và cũng block tín hiệu này. Sau đó, một trong các child process của nó thoát ra và parent process sau đó thực hiện wait() để thu thập trạng thái của child process. Điều gì xảy ra khi parent process bỏ chặn SIGCHLD?  Viết một chương trình để xác minh câu trả lời. 


Answer:
Mỗi tiến trình sẽ duy trì 2 thành phần là signal mask và signal queue. Signal mask cho phép lưu trữ trạng thái của các signal là blocking hay unblocking. Signal queue là hàng đợi lưu trữ các signal mà process đấy nhận được nhưng chưa được xử lý. 

Trong trường hợp trên process con đã gửi SIGCHILD sang process cha nhưng process cha đã block SIGCHILD nên tín hiệu này sẽ nằm đợi bên trong signal queue cho đến khi tiến trình cha unblocking và nó sẽ xử lý tín hiệu này. 

Process có thể truy cập sigqueue này thông qua hàm sigwait(), sigwaitinfo().
