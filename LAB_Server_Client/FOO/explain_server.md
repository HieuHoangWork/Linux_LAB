Chương trình bao gồm 2 process:
- Sensor gateway process là process dùng để xử lý dữ liệu 
- Log process là process dùng để ghi lại nhật ký dữ liệu

/* Sensor gateway process */
Tiến hành khởi tạo 3 thread conn_mgr, data_mgr, stor_mgr

- Thread conn_mgr: Đảm nhận nhiệm vụ xử lý các kết nối mới và dữ liệu mới đến.
    + Đầu tiên nó xử lý dữ liệu được truyền vào từ hàm main thông qua THR_DATA_T. Dữ liệu này là port_num được khởi tạo bởi người dùng thông qua việc chạy chương trình. Ví dụ: ./server 8080
    + Tiếp đến nó khởi tạo một mảng chứa tất cả các client socket.
        > Mảng client_socket[MAX_CLIENTS] trong hàm socket_server_processing() được sử dụng để lưu trữ các file descriptor (FD) của các socket client kết nối tới máy chủ. Mỗi phần tử của mảng đại diện cho một kết nối client riêng biệt.
        > Giá trị MAX_CLIENTS xác định số lượng tối đa các kết nối client mà máy chủ có thể quản lý đồng thời. Khi một client mới kết nối tới máy chủ, nó sẽ được thêm vào mảng này (thêm vào vị trí đầu tiên có giá trị 0, tức chưa được sử dụng).

        > Thông tin lưu trữ trong mảng client_socket gồm có:
            . File descriptor (FD) của các socket client: Mỗi phần tử trong mảng là một số nguyên đại diện cho một file descriptor (FD) của một socket client kết nối tới máy chủ.
            . Tình trạng kết nối của các socket client: Nếu phần tử trong mảng có giá trị 0, điều đó có nghĩa là không có kết nối client tại vị trí đó. Ngược lại, nếu giá trị khác 0, đó là file descriptor của kết nối client tương ứng.
        > Mục đích của việc sử dụng mảng client_socket là để quản lý và theo dõi tất cả các kết nối client hiện có với máy chủ. Hàm socket_server_processing() sử dụng mảng này để xác định các kết nối nào đang hoạt động và thực hiện các thao tác đọc/ghi dữ liệu từ/tới các client đó. 
    + Tiếp theo nó khởi tạo địa socket thông qua các quy trình như sau: Khởi tạo địa chỉ server, tạo socket, bind và listen
        > Quá trình bind trong việc tạo socket được sử dụng để liên kết một địa chỉ IP và cổng (port) cụ thể với socket. Đối với một máy chủ, việc bind socket với địa chỉ IP và cổng cho phép máy chủ nhận các kết nối đến từ các client trên địa chỉ và cổng đó.
            Khi bạn tạo một socket, nó chưa được gán địa chỉ nào cả. Để socket có thể nhận kết nối từ client, bạn cần chỉ định địa chỉ IP và cổng mà socket sẽ lắng nghe. Đó là lúc hàm bind được sử dụng.
        > Sau khi gọi hàm bind thành công, socket sẽ được liên kết với địa chỉ IP và cổng đã chỉ định. Tiếp theo, bạn có thể gọi hàm listen để bắt đầu lắng nghe các kết nối đến từ client trên địa chỉ và cổng đã bind.
