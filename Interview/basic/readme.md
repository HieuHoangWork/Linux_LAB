> 4 bước biên dịch chương trình C
1. Giai đoạn tiền xử lý – Preprocessor
Giai đoạn này sẽ thực hiện:
    + Nhận mã nguồn
    + Xóa bỏ tất cả chú thích, comments của chương trình
    + Chỉ thị tiền xử lý (bắt đầu bằng #) cũng được xử lý
Ví dụ: chỉ thị #include cho phép ghép thêm mã chương trình của một tệp tiêu để vào mã nguồn cần dịch. Các hằng số được định nghĩa bằng #define sẽ được thay thế bằng giá trị cụ thể tại mỗi nơi sử dụng trong chương trình.

2. Cộng đoạn dịch Ngôn Ngữ Bậc Cao sang Assembly
    + Phân tích cú pháp (syntax) của mã nguồn NNBC
    + Chuyển chúng sang dạng mã Assembly là một ngôn ngữ bậc thấp (hợp ngữ) gần với tập lệnh của bộ vi xử lý.

3. Công đoạn dịch Assembly
    + Dich chương trình => Sang mã máy 0 và 1
    + Một tệp mã máy (.obj) sinh ra trong hệ thống sau đó.
 
4. Giai đoạn Linker
    + Trong giai đoạn này mã máy của một chương trình dịch từ nhiều nguồn (file .c hoặc file thư viện .lib) được liên kết lại với nhau để tạo thành chương trình đích duy nhất
    + Mã máy của các hàm thư viện gọi trong chương trình cũng được đưa vào chương trình cuối trong giai đoạn này.
    + Chính vì vậy mà các lỗi liên quan đến việc gọi hàm hay sử dụng biến tổng thể mà không tồn tại sẽ bị phát hiện. Kể cả lỗi viết chương trình chính không có hàm main() cũng được phát hiện trong liên kết.

> Cấu trúc của Makefile 
all:	hello.c main.c
	gcc -o hello hello.c main.c -I .
clean: 
	rm -rf hello


    + Target: all, clean
    + Dependences: hello.c main.c //Kiem tra neu co file se thuc hien action
    + Action: gcc -o hello hello.c main.c -I . // rm -rf hello

> Phân biệt static_lib và shared_lib
- Static Libraries:
    + Toàn bộ nội dung của thư viện đó được "sao chép" vào chương trình đích tại thời điểm biên dịch.
    + Chương trình sẽ có bản sao riêng của thư viện, và nếu thư viện được cập nhật sau đó, chương trình sẽ không nhận được những cập nhật đó trừ khi nó được biên dịch lại
    + Lợi thế của việc sử dụng thư viện tĩnh là chương trình đích sẽ không phụ thuộc vào bất kỳ thư viện nào khác khi nó được chạy. 
    + Tuy nhiên, điều này cũng có nghĩa là chương trình có thể trở nên khá lớn nếu nó liên kết với nhiều thư viện tĩnh lớn.

- Shared Libraries:
    + Khi một chương trình liên kết với một thư viện chia sẻ, nó không sao chép thư viện vào mã nguồn của mình. Thay vào đó, nó chỉ giữ một tham chiếu đến thư viện.
    + Khi chương trình được chạy, hệ điều hành sẽ tìm thư viện chia sẻ đó và "liên kết" nó vào chương trình tại thời gian chạy.
    Điều này có nghĩa là nếu thư viện được cập nhật, chương trình sẽ tự động nhận được những cập nhật đó mà không cần phải biên dịch lại.
    + Tuy nhiên, nếu thư viện chia sẻ không có sẵn tại thời điểm chạy, chương trình sẽ không thể chạy.
    + Một lợi thế khác của thư viện chia sẻ là chúng giúp tiết kiệm không gian bộ nhớ, vì nhiều chương trình có thể chia sẻ cùng một thư viện chia sẻ thay vì mỗi chương trình đều phải có bản sao riêng của thư viện


> Spinlock
Hoạt động của spinlock có thể được tóm tắt như sau:

    + Khi spinlock đang ở trạng thái UNLOCKED, nếu một thread gọi hàm acquire_spinlock, thì spinlock bị chuyển sang trạng thái LOCKED. Ta nói rằng, thread đã khóa spinlock lại (hay thread đã chiếm dụng spinlock). Sau đó, CPU bắt đầu thực thi critical section của thread (nói theo ngôn ngữ của CPU), hay thread đang sử dụng critical resource (nói theo ngôn ngữ của Linux kernel).

    + Khi spinlock đang ở trạng thái LOCKED, nếu một thread gọi hàm acquire_spinlock, thì CPU liên tục thực thi hàm acquire_spinlock cho tới khi spinlock chuyển sang trạng thái UNLOCKED. Tình trạng này được gọi là giao tranh (contended), còn cơ chế chờ đợi này được gọi là chờ bận (busy-waiting). Do đó, spinlock thuộc loại khóa busy lock.

    + Khi spinlock đang ở trạng thái LOCKED, nếu một thread gọi hàm release_spinlock, thì spinlock chuyển sang trạng thái UNLOCKED. Ta nói rằng, thread đã mở ổ khóa spinlock (hay thread đã giải phóng spinlock). Nếu vẫn còn một vài thread đang chờ đợi để chiếm dụng spinlock, thì một trong số các thread đó sẽ chiếm được spinlock và bắt đầu sử dụng critical resource. Khi ấy, spinlock lại trở về trạng thái LOCKED.