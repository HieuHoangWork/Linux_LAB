/* Mô tả hoạt động chương trình client */

1. Chương trình bắt đầu bằng việc kiểm tra xem có đủ số lượng đối số đầu vào hay không. Nếu không đủ, nó sẽ thông báo cách sử dụng và thoát.
2. Khởi tạo biến buffer, server_fd, serv_addr và đặt giá trị ban đầu cho serv_addr.
3. Thiết lập địa chỉ máy chủ bằng cách chuyển đổi địa chỉ IP (hostname) và số cổng (port) được nhập từ đối số đầu vào.
4. Tạo socket và kết nối tới máy chủ.
5. In thông báo "Connected to server" khi đã kết nối thành công.
6. Khởi tạo hạt giống ngẫu nhiên (random seed) để tạo số ngẫu nhiên.
7. Trong vòng lặp, chương trình sẽ thực hiện các bước sau:
a. Sinh nhiệt độ ngẫu nhiên từ 9 đến 22 độ.
b. Lấy thời gian hiện tại làm dấu thời gian (timestamp).
c. Định dạng chuỗi gửi tới máy chủ theo định dạng "DEVICE_ID/nhiệt độ/dấu thời gian" và lưu vào buffer.
d. Gửi thông điệp đến máy chủ. Nếu gửi không thành công, chương trình sẽ thông báo lỗi và thoát.
e. In thông điệp đã gửi ra màn hình.
f. Xóa nội dung trong buffer.
g. Dừng chương trình trong 2 giây trước khi tiếp tục lặp.
Sau khi gửi được 10 thông điệp, chương trình sẽ thoát khỏi vòng lặp, đóng kết nối socket với máy chủ và kết thúc.

/* struct sockaddr_in */
struct sockaddr_in là một cấu trúc dữ liệu trong ngôn ngữ lập trình C, dùng để lưu trữ thông tin về địa chỉ Internet (IP) và số cổng (port) cho giao thức IPv4 trong lập trình mạng. Cấu trúc này thuộc thư viện <netinet/in.h> và thường được sử dụng trong các hàm liên quan đến socket như bind(), connect(), sendto(), recvfrom() và nhiều hàm khác.
Cấu trúc struct sockaddr_in bao gồm các trường sau:
    - sin_family: Trường này lưu trữ thông tin về họ địa chỉ (address family). Trong trường hợp của IPv4, giá trị này sẽ là AF_INET.
    - sin_port: Trường này lưu trữ số cổng (port number) liên quan đến địa chỉ. Giá trị của số cổng cần được chuyển đổi sang định dạng network byte order bằng hàm htons().
    - sin_addr: Trường này lưu trữ địa chỉ IPv4. Cấu trúc struct in_addr bên trong trường này chứa một trường duy nhất là s_addr, kiểu in_addr_t, chứa địa chỉ IP dưới dạng network byte order. Để chuyển đổi địa chỉ IP dưới dạng chuỗi sang dạng này, bạn có thể sử dụng hàm inet_pton().
    - sin_zero: Một mảng 8 byte không sử dụng, chỉ để đảm bảo kích thước của cấu trúc struct sockaddr_in bằng với kích thước của cấu trúc struct sockaddr. Trong thực tế, chúng ta thường đặt giá trị này là 0.
Khi làm việc với các hàm socket, struct sockaddr_in thường được ép kiểu (typecast) sang struct sockaddr để truyền vào các hàm.

/* send(server_fd, buffer, strlen(buffer), 0) */
Trong hàm send(), tham số thứ 4 là một biến kiểu int được gọi là flags. Tham số này dùng để điều chỉnh cách thức gửi dữ liệu của hàm send() thông qua việc thiết lập các cờ (flags) tương ứng.
Trong trường hợp của bạn, giá trị của flags được đặt là 0, nghĩa là hàm send() sẽ hoạt động theo chế độ mặc định, không sử dụng các cờ đặc biệt nào.
Có một số cờ (flags) khác nhau có thể được sử dụng để điều chỉnh hàm send(). Một số cờ thông dụng bao gồm:

    MSG_DONTWAIT: Hàm send() sẽ hoạt động theo chế độ không chặn (non-blocking). Nếu không có đủ không gian trong bộ đệm để gửi dữ liệu, hàm sẽ trả về lỗi ngay lập tức thay vì chờ đợi.

    MSG_OOB: Gửi dữ liệu "out-of-band". Đây là một cách thức truyền dữ liệu đặc biệt trong giao thức TCP, cho phép gửi một lượng nhỏ dữ liệu với độ ưu tiên cao hơn so với dữ liệu thông thường.
Tuy nhiên, trong ví dụ của bạn, giá trị 0 cho flags chỉ ra rằng không có cờ nào được sử dụng, và hàm send() sẽ hoạt động theo chế độ mặc định.
