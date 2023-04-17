/* Các bước build chương trình */
-> Đứng lại thư mục chứ server.c
1. Thay cờ IP_CLIE trong Makefile thành IP tương ứng của server
2. Chạy lệnh "make" để build ra file thực thi
3. Chạy chương trình
cd bin 
./server 8080
./client_1 IP 8080
./client_2 IP 8080

/* Check valgrind */
-> Đứng lại thư mục chứ server.c
Chạy lệnh "make" để build ra file thực thi
1. "make valgrind_server" để start server. Nếu trong 7s không có kết nối hoặc dữ liệu gửi đến chương trình sẽ tự kết thúc. 
2. "make valgrind_client_1" để start client_1
3. "make valgrind_client_2" để start client_2

Chương trình không bị memleak

/* BUG */

Bên trong // Log event: new connection (A search đoạn này trong server.c nhé)
    Sau khi chạy lệnh write_log_event(FIFO_FILE, log_event);
    Dữ liệu đã được ghi vào FIFO
    Tiếp đến gọi sem_post(&ser_ready_for_log_process) để gửi thông báo đến Log_Process nhưng gặp lỗi chỗ nào đó mà Log_Process không nhận được tín hiệu. 
    Đây là phần em code chờ tín hiệu trong Log_Process
    /*
                while (running)
            {
                sem_wait(&ser_ready_for_log_process);
                printf("hehe\n");
    */ Em có để printf ra dòng này để biết nó có nhận được tín hiệu hay không nhưng mà nó không in ra nên em đoán BUG ở đây.

