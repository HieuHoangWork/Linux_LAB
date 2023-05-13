Chương trình bao gồm 2 process:
- Sensor gateway process là process dùng để xử lý dữ liệu 
- Log process là process dùng để ghi lại nhật ký dữ liệu

/* Sensor gateway process */
Tiến hành khởi tạo 3 thread conn_mgr, data_mgr, stor_mgr

- Thread conn_mgr: Đảm nhận nhiệm vụ xử lý các kết nối mới và dữ liệu mới đến.
    + Mỗi khi có một kết nối mới đến, thông tin về kết nối sẽ được ghi lại và FIFO, mỗi khi có một dữ liệu mới được gửi đến, nó sẽ được phân tách thành các trường dữ liệu tương ứng và dữ liệu sẽ được thêm vào một node mới trong danh sách liên kết. 
    + Sau khi dữ liệu mới đến nó sẽ được phân tách thành các trường và được thêm vào node mới trong dslk. Tiếp theo tiến hành gửi tín hiệu semaphore đến data_mgr

- Thread data_mgr: Đảm nhận nhiệm vụ xử lý các dữ liệu đã được thêm vào danh sách liên kết.
    + Đầu tiên nó sẽ đợi tín hiệu thông qua sem_wait, nghĩa là nó sẽ bị block tại đây cho nó khi có dữ liệu mới đến
    + Tiến hành sắp xếp danh sách liên kết sắp xếp theo ID và Time, các bản tin có ID nhỏ nhất và thời gian (timestamps) bé nhất sẽ được xếp lên đầu.
    + Tiếp theo nó sẽ tiến hành duyệt số lượng bản tin trong danh sách liên kết. Nếu có >= 5 bản tin trở lên của mỗi loại ID nó sẽ tính giá trị trung bình của 5 bản tin này về nhiệt độ và timestamps, tiếp theo đó nó sẽ ghi các thông tin này vào một FIFO và gắn nhãn DELETE = 1 cho bản tin có chỉ số timstamps nhỏ nhất ( bản tin cũ nhất)
    + Tiếp theo nó gửi tín hiệu semaphore đến thread stor_mgr để xử lý dữ liệu đã tính giá trị trung bình.

- Thread stor_mgr: Đảm nhận nhiệm vụ ghi dữ liệu ra file thực và xóa các bản tin được đánh dấu DELETE = 1
    + Các bản tin được đánh dấu DELETE = 1 sẽ được ghi dữ liệu ra một file thực, tiếp sau đó nó sẽ được xóa khỏi danh sách liên kết. 

/* Log process */
- Log process tiến hành ghi nhật ký dữ liệu bao gồm:
    + Khi có một kết nối mới đến
    + Khi có một client ngắt kết nối
    + Khi có một loại bản tin được tính giá trị trung bình

    > Trong đó nó gọi ra hàm read(fifo_fd, buffer, BUFFER_SIZE - 1); bên trong hàm write_to_LOG_file có nghĩa là nó sẽ luôn đọc dữ liệu từ bên trong FIFO, nếu không có dữ liệu nó sẽ bị block lại điểm này. Nếu có dữ liệu mới đến từ FIFO nó sẽ đọc dữ liệu này và ghi ra log file