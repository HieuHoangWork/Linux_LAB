/------------------------------------Bài toán--------------------------------------/

Hệ thống gồm 2 chương trình server.c và client.c

- Chương trình server.c bao gồm 2 process, process con có tên là sensor_gateway, process cha có tên là log_process. 
- Process con chạy theo 3 luồng: Connection manager, Data manager, Storage manager
1. Connection manager
- Luồng Connection manager lắng nghe các kết nối trên TCP socket, chờ đợi các yêu cầu đến từ một "new" client. Sử dụng command line argument để nhập port từ bàn phím, Ví dụ: ./server 8080
- Khi "conn_mgr" accept kết nối từ "new" client, nó sẽ nhận được dữ liệu từ client, dữ liệu có dạng "/ID/temperature/timestamp" trong đó: 
	ID: ID thiết bị client
	temperature: Một số được sinh ngẫu nhiên trong khoảng 9->22
	timestamp: thời gian khi nó được gửi sang server, lấy từ local PC
- Các bản tin này sẽ được lưu trữ vào một vùng shared data có thể được sử dụng chung bởi Data manager và Storage manager. Shared data này có thể là một danh sách liên kết. Việc ghi dữ liệu vào shared data sẽ phải được sử dụng
mutex để đồng bộ hóa.

2. Data manager
- Luồng Data manager đọc dữ liệu từ shared data. Nó lấy 5 bản tin cũ nhất dựa vào việc so sánh timestamp, 5 bản tin này sẽ có cùng ID thiết bị client. Sau đó nó thực hiện tính trung bình tham số temperature của 5 bản tin này. 
Nó sẽ in ra màn hình theo cú pháp "ID/timestamps/TEMP" trong đó ID là tên thiết bị client, timestamp là trung bình thời gian của 5 bản tin vừa lấy ra xử lý, nếu temperature trung bình của 5 bản tin này > 20 thì TEMP sẽ là "TOO COLD"
còn nếu temperature trung bình của 5 bản tin này < 13 thì TEMP sẽ là "TOO HOT". Các trường hợp khác thì TEMP sẽ là "NORMAL". Sau đó nó gửi một tín hiệu đến Storage manager cho biết nó vừa đọc 5 bản tin nào. 

2. Storage manager
- Sau khi nhận được tín hiệu Storage manager sẽ tiến hành ghi 5 bản tin này ra một file "data.txt", nếu file này chưa có sẵn thì tạo ra nó. Sau khi ghi dữ liệu 5 bản tin này, các bản tin này sẽ được giải phóng bộ nhớ khỏi link list.

Một số lưu ý: 
- server.c có thể kết nối với nhiều client khác nhau với các ID khác nhau tại cùng một thời điểm.
- Nếu chưa có client nào có gửi đủ 5 bản tin để xử lý nó sẽ được chờ trong các thread còn lại. Nếu có một client đã gửi 5 bản tin thì nó sẽ được ưu tiên thực hiện với luồng  Data manager và Storage manager

Log Process
- "log_process" nhận các log-events từ main process thông qua việc sử dụng FIFO, dữ liệu nhận được sẽ được ghi vào file "logFIFO.log".
- Mỗi một log-event có format như sau:
	<sequence number> <timep stamp> <log-event message>
- Ít nhất thì có các log-events sau đây bắt buộc phải có:
	1. log-event từ "conn_mgr":
		- A sensor node with <sensor_id> has opened a new connection.
		- The sensor node with <sensor_id> has closed the connection.
	2. log-event từ "data_mgr":
		- The sensor node with <sensor_id> report it's TOO COLD, (running avg temperature = <value>).
		- The sensor node with <sensor_id> report it's TOO HOT, (running avg temperature = <value>).
	3. log event từ "stor_mgr":
		- Connection to SQLite server established.
		- New table <name_of_table> created.
		- Connection to SQLite server lost.
		- Unable to connect to SQL server.

/------------------------------------Giải thích--------------------------------------/

Luồng connection manager

while (1)
    {
        // Clear the socket set
        FD_ZERO(&readfds);

        // Add server socket to set
        FD_SET(server_fd, &readfds);
        max_sd = server_fd;

        // Wait for activity on one of the sockets
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if (activity < 0)
        {
            handle_error("select failed");
        }

        // If something happened on the server socket, it is an incoming connection
        if (FD_ISSET(server_fd, &readfds))
        {
            if ((client_fd = accept(server_fd, (struct sockaddr *)&client_address, &addrlen)) < 0)
            {
                handle_error("accept failed");
            }
            printf("New client connected, IP: %s, Port: %d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

            // Else it is some IO operation on some other socket
            while (1) // Thêm vòng lặp vô hạn tại đây để đọc dữ liệu gửi đến
            {
                int valread;

                if ((valread = read(client_fd, buffer, BUFFER_SIZE)) == 0)
                {
                    // Somebody disconnected
                    printf("Device disconnected, IP: %s, Port: %d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
                    close(client_fd);
                }
                else
                {
                    buffer[valread] = '\0';
                    printf("Received from client: %s\n", buffer);

                    // Phân tích dữ liệu
                    char sensor_id[32];
                    float temperature;
                    time_t timestamp;

                    sscanf(buffer, "/%31[^/]/%f/%ld", sensor_id, &temperature, &timestamp);

                    // Tạo một node mới và thêm vào danh sách liên kết
                    SensorData *new_node = (SensorData *)malloc(sizeof(SensorData));
                    strcpy(new_node->sensor_id, sensor_id);
                    new_node->temperature = temperature;
                    new_node->timestamp = timestamp;
                    new_node->next = NULL;

                    pthread_mutex_lock(&data_mutex);
                    new_node->next = head;
                    head = new_node;
                    pthread_mutex_unlock(&data_mutex);
                }
            }
        }
    }

/*
    while (1): Vòng lặp vô hạn để xử lý các kết nối và dữ liệu đến máy chủ liên tục.
    FD_ZERO(&readfds): Xóa tập socket (làm sạch).
    FD_SET(server_fd, &readfds): Thêm server socket vào tập socket.
    max_sd = server_fd: Gán giá trị max_sd bằng server_fd.
    activity = select(max_sd + 1, &readfds, NULL, NULL, NULL): Chờ hoạt động trên một trong các socket trong tập socket. Nếu có hoạt động trên một socket nào đó, hàm select sẽ trả về giá trị khác -1.
    Nếu activity < 0, xuất lỗi và thoát chương trình.
    if (FD_ISSET(server_fd, &readfds)): Kiểm tra xem hoạt động có xảy ra trên server socket hay không. Nếu có, đó là một kết nối đến mới.
    client_fd = accept(server_fd, (struct sockaddr *)&client_address, &addrlen): Chấp nhận kết nối đến và lấy file descriptor cho kết nối đó.
    Nếu client_fd < 0, xuất lỗi và thoát chương trình.
    In thông tin kết nối mới, bao gồm IP và cổng.
    while (1): Vòng lặp vô hạn để đọc dữ liệu từ client.
    a. Đọc dữ liệu từ client.
    b. Nếu valread == 0, đóng kết nối và thoát khỏi vòng lặp.
    c. Nếu có dữ liệu, in dữ liệu đã nhận được.
    d. Phân tích dữ liệu thành sensor_id, temperature, và timestamp.
    e. Tạo một node mới và thêm vào danh sách liên kết.
    f. Khóa mutex và cập nhật danh sách liên kết.
    g. Mở khóa mutex.
*/