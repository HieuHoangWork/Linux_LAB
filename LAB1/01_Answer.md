1. Khi nào orphane/zombie process được tạo ra?

Một tiến trình mồ côi (orphan process) được tạo ra khi tiến trình cha của nó kết thúc trước khi tiến trình con kết thúc. Khi điều này xảy ra, tiến trình con sẽ không còn có tiến trình cha để quản lý nó nữa và sẽ trở thành một tiến trình mồ côi. Trong hệ thống UNIX hoặc LINUX (Like unix), tiến trình "init" có id là 1 sẽ tự động nhận nuôi các tiến trình mồ côi và đảm nhận vai trò làm tiến trình cha của chúng. Tiến trình init sẽ gọi hàm wait hoặc waitpid để thu thập thông tin về trạng thái thoát của chúng khi chúng kết thúc.

Một tiến trình zombie (zombie process) được tạo ra khi một tiến trình con kết thúc nhưng tiến trình cha của nó không gọi hàm wait hoặc waitpid để thu thập thông tin về trạng thái thoát của tiến trình con. Khi điều này xảy ra, tiến trình con sẽ không được giải phóng khỏi bộ nhớ và vẫn còn tồn tại dưới dạng một tiến trình zombie. Để giải phóng một tiến trình zombie, tiến trình cha cần gọi hàm wait hoặc waitpid để thu thập thông tin về trạng thái thoát của tiến trình con.

2. Khi một child process kết thúc, nó sẽ gửi signal nào tới parent? signal value bằng bao nhiêu?

Khi một tiến trình con kết thúc, nó sẽ gửi tín hiệu SIGCHLD đến tiến trình cha của nó. Giá trị của tín hiệu SIGCHLD là 17 trên hầu hết các hệ thống UNIX.

Tín hiệu SIGCHLD được gửi đến tiến trình cha để thông báo rằng một trong các tiến trình con của nó đã kết thúc hoặc bị dừng lại. Khi nhận được tín hiệu này, tiến trình cha có thể gọi hàm wait hoặc waitpid để thu thập thông tin về trạng thái thoát của tiến trình con.

Tiến trình cha có thể cài đặt trình xử lý tín hiệu SIGCHLD theo cách của riêng của mình thông qua việc đăng ký một hàm sig_handle. Ngoài ra các tiến trình cũng có thể sử dụng signal processor mask để blocking hoặc unblocking các signal. 