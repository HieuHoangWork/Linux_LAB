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



