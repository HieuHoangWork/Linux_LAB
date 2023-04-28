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

    + Tiến hành chạy chương trình trong vòng lặp while
        > FD_ZERO(&readfds) xóa sạch tập hợp các file descriptor trong cấu trúc fd_set readfds.
        > FD_SET(server_fd, &readfds) thêm file descriptor của server socket (server_fd) vào tập hợp readfds.
        > Vòng lặp for kế tiếp thêm các socket máy khách vào tập hợp readfds.
        > Thiết lập thời gian chờ cho hàm select() thông qua cấu trúc timeval. Thời gian chờ được thiết lập là 7 giây.
        > activity = select(max_sd + 1, &readfds, NULL, NULL, &timeout) gọi hàm select() để theo dõi sự kiện trên tất cả các socket trong tập hợp readfds. Nếu có sự kiện xảy ra trên một socket, hàm select() sẽ trả về số lượng socket có sự kiện.
        > Nếu activity < 0, có lỗi xảy ra trong quá trình gọi hàm select(). Hàm handle_error() được gọi để xử lý lỗi.
        > Nếu activity == 0, không có sự kiện xảy ra trong khoảng thời gian chờ, chương trình sẽ thoát khỏi vòng lặp và dừng lại.
        > Nếu có sự kiện trên server socket, tức là có kết nối đến từ máy khách, chương trình sẽ chấp nhận kết nối và thêm socket của máy khách vào mảng client_socket.
        > Nếu sự kiện không phải trên server socket, điều đó có nghĩa là có hoạt động IO trên một socket máy khách khác. Chương trình sẽ xử lý thông tin nhận được từ socket máy khách và lưu vào danh sách liên kết.
        > Khi chương trình kết thúc, tất cả các socket sẽ được đóng lại và luồng sẽ kết thúc bằng cách gọi pthread_exit(NULL).

        > LƯU Ý: Mỗi khi có một kết nối mới đến, thông tin về kết nối sẽ được ghi lại và FIFO, mỗi khi có một dữ liệu mới được gửi đến, nó sẽ được phân tách thành các trường dữ liệu tương ứng và dữ liệu sẽ được thêm vào một node mới trong danh sách liên kết. 

- Thread data_mgr: Đảm nhận nhiệm vụ xử lý các dữ liệu đã được thêm vào danh sách liên kết.
    + Đầu tiên nó sẽ đợi tín hiệu thông qua sem_wait, nghĩa là nó sẽ bị block tại đây cho nó khi có dữ liệu mới đến (Mỗi khi có dữ liệu mới nó sẽ được thêm vào dslk và tín hiệu sẽ được gửi đến data_mgr báo hiệu xử lý dữ liệu mới).
    + Dữ liệu mới đến sẽ được sắp xếp theo ID và Time, các bản tin có ID nhỏ nhất và thời gian (timestamps) bé nhất sẽ được xếp lên đầu.
    + Tiếp theo nó sẽ tiến hành duyệt số lượng bản tin trong danh sách liên kết. Nếu có >= 5 bản tin trở lên của mỗi loại ID nó sẽ tính giá trị trung bình của 5 bản tin này về nhiệt độ và timestamps, tiếp theo đó nó sẽ ghi các thông tin này vào một FIFO và gắn nhãn DELETE = 1 cho bản tin có chỉ số timstamps nhỏ nhất ( bản tin cũ nhất)
    + Tiếp theo nó gửi tín hiệu đến thread stor_mgr để xử lý dữ liệu đã tính giá trị trung bình.

- Thread stor_mgr: Đảm nhận nhiệm vụ ghi dữ liệu ra file thực và xóa các bản tin được đánh dấu DELETE = 1
    + Các bản tin được đánh dấu DELETE = 1 sẽ được ghi dữ liệu ra một file thực, tiếp sau đó nó sẽ được xóa khỏi danh sách liên kết. 

/* Log process */
- Log process tiến hành ghi nhật ký dữ liệu bao gồm:
    + Khi có một kết nối mới đến
    + Khi có một client ngắt kết nối
    + Khi có một loại bản tin được tính giá trị trung bình

    > Trong đó nó gọi ra hàm read(fifo_fd, buffer, BUFFER_SIZE - 1); bên trong hàm write_to_LOG_file có nghĩa là nó sẽ luôn đọc dữ liệu từ bên trong FIFO, nếu không có dữ liệu nó sẽ bị block lại điểm này. Nếu có dữ liệu mới đến từ FIFO nó sẽ đọc dữ liệu này và ghi ra log file