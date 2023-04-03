I. General Knowledge
1. Tạo một static library và shared lib chứa các hàm in ra các thông điệp "hello wolrd", "linux programming", vv... Số lượng các hàm là tùy ý (lớn hơn 2). Thư mục yêu cầu chứa cấu trúc mã nguồn như sau
- src: Chứa các file .c
- inc: Chứa các file .h
- obj: Chứa các file .o
- bin: Chứa file thực thi
- libs/static: Chứa thư viện static
- libs/shared: Chứa thư viện shared
II. FILE
1. Tạo file filesystem.txt và ghi một đoạn dự liệu bất kì. Thực hiện ghi ra màn hình các thông tin: Loại file,  thời gian chỉnh sửa file lần cuối, kích thước.
III. Proess
1. Viết một chương trình A tạo ra một tiến trình con B, in ra PID và PPID của chúng. Sử dụng system waitpid() để lấy được trạng thái kết thúc của tiến con B.
2. Từ kiến thức đã học, sử dụng SIGCHLD để ngăn ngừa việc hình thành zombie process.
3. Khi nào orphane/zombie process được tạo ra? 
4. Khi một child process kết thúc, nó sẽ gửi signal nào tới parent? signal value bằng bao nhiêu?

Note:
1. Thực hiện biên dịch và quản lý mã nguồn bằng Makefile
2. Tuân theo c coding convention (keywork: coding convention)
3. Thực hiện mem check, đảm bảo chương trình không leak memory
4. Thực hiện biên dịch chương trình với cờ -Werror, -Wall
