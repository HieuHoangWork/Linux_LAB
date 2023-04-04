/************** Major *******************/ 
Viết một chương trình để chứng minh rằng các thread khác nhau trong cùng một process có thể có các tập hợp signal đang chờ xử lý (set of pending signal) khác nhau,được trả về bằng sigpending(). Bạn có thể làm điều này bằng cách sử dụng pthread_kill() để gửi các tín hiệu khác nhau đến hai thread khác nhau đã bị block các tín hiệu này, và sau với mỗi thread gọi sigpending() hãy hiển thị thông tin về các tín hiệu đang chờ xử lý.



/************** Minor *******************/ 
1. Thread là một phần của process. Tạo ra 2 thread A và B cùng thuộc một process, nếu thread A block sigusr1 thì thread B có bị block sigusr1 không ?

Một thread kế thừa tập hợp tín hiệu bị chặn của thread đã tạo nó1. Do đó, nếu thread A chặn tín hiệu SIGUSR1 trước khi tạo ra thread B, thì thread B cũng sẽ chặn tín hiệu SIGUSR1. Tuy nhiên, nếu thread A chặn tín hiệu SIGUSR1 sau khi đã tạo ra thread B, thì thread B sẽ không bị ảnh hưởng và vẫn có thể nhận được tín hiệu SIGUSR1.

2. Nếu trong process thiết lập chặn tín hiệu sigusr1 sau đó tạo ra thread A. Bên trong thread A bỏ chặn sigusr1 thì thread A có nhận được sigusr1 không ?

Nếu trong một process chặn tín hiệu SIGUSR1 trước khi tạo ra thread A, thì thread A sẽ kế thừa tập hợp tín hiệu bị chặn của process và cũng sẽ chặn tín hiệu SIGUSR1. Tuy nhiên, nếu bên trong thread A, bạn sử dụng hàm sigprocmask() để bỏ chặn tín hiệu SIGUSR1, thì thread A sẽ có thể nhận được tín hiệu SIGUSR1.

3. Nếu thread A nhận được tín hiệu sigusr1 thì các luồng khác có nhận được tín hiệu này không ?

Khi một tín hiệu được gửi đến một process đa luồng, chỉ có một thread duy nhất trong process đó sẽ nhận được tín hiệu. Hệ điều hành sẽ chọn một thread để giao tín hiệu dựa trên các quy tắc cụ thể. Nếu một thread đã đăng ký trình xử lý tín hiệu cho tín hiệu đó và không chặn tín hiệu đó, thì thread đó sẽ được chọn để nhận tín hiệu. Nếu không có thread nào đăng ký trình xử lý tín hiệu hoặc tất cả các thread đều chặn tín hiệu đó, thì hệ điều hành sẽ chọn một thread ngẫu nhiên để giao tín hiệu.

Do đó, nếu thread A nhận được tín hiệu SIGUSR1, thì các thread khác trong cùng một process sẽ không nhận được tín hiệu này.

4. Nếu thread A và B cùng đăng ký nhận sigusr1 thì thread nào sẽ nhận được tín hiệu này ?

Nếu cả thread A và thread B đều đăng ký trình xử lý tín hiệu cho SIGUSR1 và không chặn tín hiệu này, thì hệ điều hành sẽ chọn một trong hai thread để giao tín hiệu. Quyết định chọn thread nào để giao tín hiệu phụ thuộc vào cách triển khai của hệ điều hành và có thể thay đổi mỗi lần chạy chương trình. Do đó, không thể xác định chắc chắn thread nào sẽ nhận được tín hiệu.

/************** Explain main.c for answer Major *******************/
- Tạo ra 2 thread là thread1 và thread2. 
- Thread1 đăng ký hàm xử lý cho 2 tín hiệu là sigusr1 và sigusr2, sử dụng sigprocmask() để block tín hiệu sigusr1. Tiếp theo là sử dụng pthread_kill() để gửi sigusr1 và sigusr2 đến thread_id hiện tại của nó. Khi này sigusr1 sẽ được đưa vào hàng đợi để chờ xử lý. Tín hiệu sigusr2 sẽ handle đến hàm xử lý dữ liệu của nó và in ra thông báo "Signal user 2 running". 
Tiếp theo sử dụng hàm sigpending để lấy ra các các tín hiệu lưu trong hàng đợi và gán nó vào một con trỏ kiểu sigset_t. Cuối cùng là kiểm tra giá trị của signal lưu trong hàng đợi bằng hàm sigismember().

- Tương tự với thread2.

Ví dụ trên đã chứng minh thread1 và thread2 có 2 hàng đợi signalpending() khác nhau. 