/* Sensor Monitoring System - Hệ thống giám sát cảm biến

Hệ thống giám sát cảm biến bao gồm Sensor node đo nhiệt độ phòng, một Sensor gateway thu nhận tất cả dữ liệu cảm biến từ các Sensor node và cơ sở dữ liệu SQL để lưu trữ tất cả dữ liệu cảm biến được xử lý bởi Sensor gateway. 
Một Sensor node sử dụng một private TCP kết nối để truyền dữ liệu cảm biến đến Sensor gateway. Cơ sở dữ liệu SQL là một SQLite.

Sensor gateway có thể không đảm nhận số lượng cảm biến tối đa khi khởi động. Trên thực tế số lượng cảm biến kết nối với Sensor gateway không cố định và có thể thay đổi thời gian.

/* Sensor Gateway
Minimal requirements

1. Sensor gateway bao gồm 2 quy trình là main process và log process. Log process bắt đầu với fork(), là một tiến trình con của tiến trình chính.
2. Main process chạy 3 luồng thread: connection, data và storage. Quyền read/write/update-access cần được an toàn (thread-safe).
3. Luồng connection lắng nghe trên TCP socket chờ các yêu cầu kết nối từ các nút cảm biến mới.
Port number được cung cấp dưới dạng đối số truyền vào. Ex: ./server 1234
4. Luồng connection xử lý các gói đến từ sensor node theo định nghĩa trong lab7
5. Luồng data được triển khai theo lab5. Nó đọc các phép đo cảm biến từ dữ liệu được chia sẻ, tính toán thời gian chạy về nhiệt độ và sử dụng kết quả đo đói để quyết định "nóng or lạnh". Nó không ghi các giá trị trung bình đang chạy vào dữ liệu được chia sẻ - chỉ sử dụng để tự ra quyết định.
6. Luồng storage đọc các phép đo cảm biến từ dữ liệu được chia sẻ và chèn chúng và cơ sở dữ liệu SQL (lab6). Nếu kết nối với cơ sở dữ liệu SQL không thành công, nó sẽ được một lúc và kết nối lại. Các phép đo cảm biến sẽ được ở trong dữ liệu được chia sẻ cho đến khi kết nối với cơ sở dữ liệu được hoạt động ok. Nếu kết nối không thành công sau 3 lần thử cổng sẽ đóng.
7. Log process ghi lại các log-events từ main process sử dụng FIFO - called name "logFIFO". Nếu FIFO này không tồn tại khi khởi động main process or log process thì nó sẽ được tại ra bởi một trong các tiến trình. Tất cả các luồng của chương trình chính có thể tạo ra sự kiện và ghi các sự kiện này vào FIFO. Điều này có nghĩa là FIFO được chia sẻ cho nhiều luồng và do đó, quyền truy cập vào FIFO phải an toàn
8. Log-event chứa thông báo về thông tin ASCII mô tả loại sự kiện. Cho mỗi log-event nhận được, log process sẽ viết một tin nhắn ASCII theo định dạng <sequence number> <timestamp> <log-event info message> đến một dòng mới trên nhật ký tệp có tên là gateway.log
9. Log-event cần được hỗ trợ:
9.1 Từ luồng connection
a, Một nút cảm biến có <sensorNodeID> đã mở một kết nối mới
b, Nút cảm biến có <sensorNodeID> đã đóng kết nối
9.2 Từ luồng data
a, Nút cảm biến <sensorNodeID> báo cáo trời quá lạnh (running avg
temperature = <value>)
b, Nút cảm biến <sensorNodeID> báo cáo trời quá nóng (running avg
temperature = <value>)
c, Nhận được dữ liệu không hợp lệ từ một sensor node ID <node-ID>
9.3 Từ luồng storage
a, Đã thiết lập kết nối tới máy chủ SQL
b, Bảng mới < tên bảng > được tạo
c, Mất kết nối với máy chủ SQL
d, Không thể kết nối với máy chủ SQL