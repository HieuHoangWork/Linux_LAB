> 1. Keyword volatile trong C
Biến volatile trong ngôn ngữ lập trình C là một từ khóa được sử dụng để khai báo một biến có thể bị thay đổi bất ngờ bởi một số quá trình bên ngoài chương trình, chẳng hạn như các ngắt phần cứng, hoạt động của hệ thống, hoặc các luồng chạy song song trong chương trình đa luồng.

Một biến cần được khai báo dưới dạng volatile khi:
    + Biến đó được sử dụng trong các trình xử lý ngắt (interrupt handlers) hoặc các chương trình đa luồng, trong đó các luồng khác nhau có thể truy cập và thay đổi giá trị của biến mà không có sự đồng bộ hóa.

    + Biến đó được ánh xạ đến một thanh ghi phần cứng hoặc một vị trí nhớ đặc biệt mà giá trị của nó có thể thay đổi do hoạt động của phần cứng.

    + Khi bạn muốn chống lại việc tối ưu hóa của trình biên dịch, ví dụ: trình biên dịch có thể loại bỏ hoặc thay đổi thứ tự của các hoạt động liên quan đến biến không được sử dụng, điều này có thể dẫn đến những hành vi không mong muốn trong chương trình.

Khi một biến được khai báo là volatile, trình biên dịch sẽ không thực hiện các tối ưu hóa liên quan đến biến đó, đảm bảo rằng mọi truy cập hoặc sửa đổi đều được thực hiện một cách chính xác theo thứ tự xuất hiện trong mã nguồn.

> 2. Synchronization
Hiện tượng race condition khi nhiều luồng cùng truy cập đến một shared resource hoặc critical section mà không có cơ chế đồng bộ dữ liệu. 

VD: Khi hai hoặc nhiều luồng truy cập cùng lúc đến một tài nguyên chia sẻ (shared resource) và ít nhất một trong số đó thực hiện thao tác ghi (write), có thể xảy ra race condition. Điều này dẫn đến kết quả không đúng hoặc không nhất quán nếu các luồng không được đồng bộ hóa đúng cách.

Để ngăn chặn race condition, có một số phương pháp đồng bộ hóa được sử dụng trong lập trình đa luồng:

    + Khóa (Locks): Sử dụng khóa để đảm bảo rằng chỉ một luồng có thể truy cập vào tài nguyên chia sẻ tại một thời điểm. 
    + Biến điều kiện (Condition variables): Kết hợp với khóa để cho phép các luồng chờ đợi cho đến khi một điều kiện nào đó được đáp ứng, sau đó tiếp tục thực thi.
    + Semaphore: Sử dụng để kiểm soát số lượng các luồng truy cập và vùng nhớ dùng chung trong 1 thời điểm
    + Đồng bộ hóa thông qua tin nhắn (Message passing): Trong mô hình này, các luồng giao tiếp với nhau thông qua việc trao đổi tin nhắn chứa thông tin về tài nguyên chia sẻ, thay vì truy cập trực tiếp đến tài nguyên đó. Điều này giúp ngăn chặn race condition do việc truy cập đồng thời vào tài nguyên chia sẻ.
    + Atomic operations: Là các phép toán được thực hiện trong một bước không thể bị chia cắt, không bị gián đoạn bởi các luồng khác. Sử dụng các phép toán nguyên tử (atomic) có thể giúp đảm bảo tính nhất quán của dữ liệu và ngăn chặn race condition mà không cần sử dụng khóa hoặc cơ chế đồng bộ hóa khác.

> 3. Spinlock, mutex, semaphore là gì?
1. Spinlock:

Spinlock là một cơ chế đồng bộ hóa đơn giản dựa trên việc "vòng quay" (busy waiting). Khi một luồng muốn truy cập vào tài nguyên chia sẻ được bảo vệ bởi spinlock, nếu spinlock đã bị khóa, luồng sẽ vòng quay liên tục kiểm tra trạng thái của spinlock cho đến khi nó được mở khóa. Spinlock thích hợp khi thời gian chờ đợi dự kiến là ngắn.

    + Ưu điểm: Spinlock đơn giản và thường nhanh hơn khi thời gian chờ đợi ngắn do không cần chuyển trạng thái luồng.

    + Nhược điểm: Nếu thời gian chờ đợi dài, spinlock sẽ tiêu tốn nhiều tài nguyên CPU do vòng quay liên tục.

2. Mutex (Mutual Exclusion):

Mutex là một cơ chế đồng bộ hóa được thiết kế để đảm bảo rằng chỉ có một luồng có thể truy cập vào tài nguyên chia sẻ tại một thời điểm. Khi một luồng khác muốn truy cập tài nguyên đó trong khi mutex đang bị khóa, luồng sẽ bị chặn và đưa vào hàng đợi chờ đợi cho đến khi mutex được giải phóng.

    + Ưu điểm: Mutex giúp giảm tiêu tốn tài nguyên CPU do không sử dụng vòng quay, đồng thời cho phép xử lý ưu tiên giữa các luồng.

    + Nhược điểm: Mutex có thể gây ra hiện tượng "đói" (starvation) nếu một luồng liên tục giữ mutex mà không giải phóng cho luồng khác sử dụng.

3. Semaphore:

Semaphore là một cơ chế đồng bộ hóa dựa trên biến đếm, được sử dụng để kiểm soát việc truy cập đồng thời vào một tài nguyên chia sẻ. Semaphore cho phép nhiều luồng (tối đa số lượng biến đếm) truy cập tài nguyên chia sẻ cùng lúc. Khi một luồng truy cập vào tài nguyên, nó giảm giá trị biến đếm; khi giải phóng tài nguyên, giá trị biến đếm tăng lên. Nếu giá trị biến đếm bằng 0, các luồng muốn truy cập tài nguyên sẽ bị chặn và chờ đợi.

    + Ưu điểm: Semaphore linh hoạt hơn mutex và spinlock, vì nó cho phép kiểm soát số lượng luồng có thể truy cập tài nguyên chia sẻ đồng thời.

    + Nhược điểm: Semaphore phức tạp hơn mutex và spinlock, và việc sử dụng không chính xác có thể dẫn đến các vấn đề đồng bộ hóa, chẳng hạn như hiện tượng "bế tắc" (deadlock).

So sánh 3 kỹ thuật trên:

->  Spinlock thích hợp cho các tình huống có thời gian chờ đợi ngắn và yêu cầu độ trễ thấp nhất có thể, nhưng không hiệu quả khi thời gian chờ đợi dài do tiêu tốn tài nguyên CPU.

->  Mutex phù hợp cho các tình huống chỉ yêu cầu một luồng truy cập tài nguyên chia sẻ tại một thời điểm, và thời gian chờ đợi có thể dài hơn. Mutex giúp giảm tiêu tốn tài nguyên CPU so với spinlock.

->  Semaphore phù hợp cho các tình huống cần kiểm soát số lượng luồng truy cập đồng thời vào tài nguyên chia sẻ. Nó linh hoạt hơn nhưng phức tạp hơn so với mutex và spinlock.

> 4. Interrupt
Interrupt (ngắt) là một cơ chế cho phép việc tạm ngừng một quá trình đang thực thi để chuyển sang xử lý một sự kiện hoặc tác vụ khác quan trọng hơn. 
Interrupt thường được sử dụng để thông báo cho CPU về các sự kiện ngoài mong đợi, như nhập dữ liệu từ bàn phím, giao tiếp với các thiết bị ngoại vi,... 
Mục đích của interrupt là giúp tăng hiệu suất và đáp ứng nhanh hơn đối với các sự kiện quan trọng.

Hệ điều hành xử lý hàm ngắt như sau:

    + Lưu trữ trạng thái hiện tại: Khi một interrupt xảy ra, hệ điều hành sẽ lưu trữ trạng thái hiện tại của CPU, bao gồm các thanh ghi, con trỏ lệnh và các thông số khác. Điều này giúp đảm bảo rằng sau khi xử lý interrupt, hệ điều hành có thể tiếp tục thực thi chương trình đang chạy trước đó mà không gây ra sai sót.

    + Xác định ngắt: Hệ điều hành sẽ xác định loại interrupt và nguồn gây ra interrupt. Dựa vào thông tin này, hệ điều hành sẽ tìm đến hàm xử lý interrupt (interrupt handler) tương ứng để thực thi.

    + Thực thi hàm xử lý interrupt: Hàm xử lý interrupt sẽ được gọi để xử lý sự kiện gây ra interrupt. Công việc của hàm xử lý interrupt phụ thuộc vào loại interrupt và nguồn gây ra. Ví dụ, nếu interrupt do nhập dữ liệu từ bàn phím, hàm xử lý interrupt sẽ đọc dữ liệu từ bàn phím và lưu vào bộ nhớ.

    + Phục hồi trạng thái: Sau khi hàm xử lý interrupt hoàn thành công việc, hệ điều hành sẽ phục hồi trạng thái của CPU đã được lưu trữ ở bước 1. Điều này cho phép hệ điều hành tiếp tục thực thi chương trình đang chạy trước đó từ điểm bị gián đoạn.

    + Tiếp tục thực thi chương trình: Cuối cùng, hệ điều hành sẽ tiếp tục thực thi chương trình đang chạy trước đó từ điểm bị gián đoạn, như không có sự cố gắng nào xảy ra. Nhờ cơ chế interrupt, hệ điều hành có thể đảm bảo rằng các sự kiện quan trọng được xử lý kịp thời mà không làm ảnh hưởng đến việc thực thi các chương trình khác.

Cơ chế interrupt là một phần quan trọng của hệ điều hành và đóng vai trò chính trong việc điều phối và quản lý tài nguyên của hệ thống. Các thiết bị ngoại vi, chương trình người dùng và hệ thống có thể tương tác với nhau thông qua interrupt để đảm bảo rằng hệ thống hoạt động mượt mà, hiệu quả và đáp ứng nhanh chóng đối với các yêu cầu và sự kiện.

> 5. Khái niệm bottom haft, top haft và cách sử dụng.

Trong hệ thống máy tính, việc xử lý các interrupt đôi khi được chia thành hai phần: top half và bottom half. Cách chia này giúp giảm thiểu thời gian mà CPU bị ngắt, tăng hiệu suất và ổn định hệ thống. Dưới đây là khái niệm về top half, bottom half và cách sử dụng chúng:

1. Top half:
    Top half là phần xử lý đầu tiên của interrupt, được thực hiện ngay khi interrupt xảy ra. Nhiệm vụ của top half là xác định nguồn gây ra interrupt và đưa thông tin liên quan vào hàng đợi. Sau đó, top half sẽ kết thúc và cho phép CPU tiếp tục thực thi chương trình đang chạy trước đó. 

2. Bottom half:
    Bottom half là phần xử lý thứ hai của interrupt, được thực hiện sau khi top half hoàn thành. Nhiệm vụ của bottom half là hoàn thành các công việc còn lại liên quan đến interrupt, thường là các tác vụ phức tạp hơn và không cấp bách. Bottom half thường được thực thi trong quá trình hệ điều hành lên kế hoạch cho các tác vụ và chương trình khác.

Cách sử dụng top half và bottom half:
    + Khi một interrupt xảy ra, hệ điều hành sẽ thực hiện top half ngay lập tức để xử lý các công việc cấp bách và tối thiểu. Sau đó, hệ điều hành sẽ lên kế hoạch thực hiện bottom half ở thời điểm sau, dựa trên độ ưu tiên của các tác vụ và tình trạng hệ thống.

    + Cách chia này giúp giảm thiểu thời gian mà CPU bị ngắt do xử lý interrupt, đồng thời đảm bảo rằng các tác vụ không cấp bách được thực hiện khi hệ thống có sẵn tài nguyên và thời gian. Điều này đặc biệt quan trọng trong các hệ thống thời gian thực (real-time systems), nơi mà việc đảm bảo độ trễ thấp và ổn định hệ thống là rất quan trọng.

> 6. Trong ngắt có được trực tiếp hoặc gián tiếp gọi hàm delay hay không?
Trong quá trình xử lý ngắt (interrupt handler), việc gọi trực tiếp hoặc gián tiếp hàm delay không được khuyến khích và nên tránh. Lý do chính là các hàm delay thường ngăn chặn quá trình thực thi của CPU trong một khoảng thời gian nhất định. Khi sử dụng hàm delay trong ngắt, điều này sẽ dẫn đến việc:

    + Tăng thời gian ngắt: Khi sử dụng hàm delay trong ngắt, thời gian ngắt của CPU sẽ tăng lên, ảnh hưởng đến hiệu suất và độ ổn định của hệ thống.

    + Bỏ lỡ các ngắt khác: Nếu CPU đang bị ngắt để xử lý một ngắt cụ thể và sử dụng hàm delay, điều này có thể dẫn đến việc bỏ lỡ các ngắt khác xảy ra trong thời gian ngắt đó. Điều này có thể gây ra mất dữ liệu hoặc không đáp ứng kịp thời các sự kiện quan trọng.

    + Tiềm ẩn race condition: Sử dụng hàm delay trong ngắt có thể dẫn đến race condition, khi các tác vụ khác cũng muốn truy cập vào tài nguyên chung trong thời gian ngắt.

Thay vì sử dụng hàm delay trong ngắt, một giải pháp tốt hơn là sử dụng cơ chế top half và bottom half như đã đề cập ở trên. Bằng cách chia công việc ra thành hai phần, bạn có thể đảm bảo rằng các công việc cấp bách được xử lý ngay lập tức, trong khi các công việc không cấp bách và phức tạp hơn có thể được lên kế hoạch thực hiện sau, khi hệ thống có sẵn tài nguyên và thời gian.

Trong hàm ngắt thì chỉ được sử dụng các yếu tố với cơ chế busy wating (kiểm tra liên tục). KHÔNG được sử dụng các yếu tố với cơ chế sleep wating (chế độ ngủ)

> 7. Workqueue là gì? Softirq là gì? Tasklet là gì?  So sánh ba kĩ thuật trên, lấy ví dụ trường hợp sử dụng.

Workqueue, softirq và tasklet xử lý các công việc liên quan đến ngắt ở mức độ bottom half. Cả ba kỹ thuật đều giúp xử lý các công việc  sau khi top half đã hoàn thành.

1. Workqueue:
    Workqueue là một cấu trúc dữ liệu hàng đợi dùng để lên kế hoạch thực hiện các công việc theo thứ tự chúng được đưa vào hàng đợi. Mỗi công việc trong workqueue được thực hiện bởi một kernel thread, và do đó có thể ngủ (sleep) và chờ đợi các sự kiện. Workqueue thích hợp cho các tác vụ mà việc ngủ và chờ đợi là chấp nhận được.

    Ví dụ: Xử lý dữ liệu từ thiết bị ngoại vi như chuột, bàn phím, mạng, sau khi top half đã đưa dữ liệu vào hàng đợi.

2. Softirq:
    Softirq là một kỹ thuật xử lý ngắt mềm. Softirq hoạt động ở mức độ cao hơn so với workqueue và không cho phép ngủ hay chờ đợi sự kiện. Softirq thích hợp cho các tác vụ yêu cầu độ trễ thấp và không cần đợi.

    Ví dụ: Xử lý gói tin trong hệ thống mạng, sau khi top half đã nhận gói tin và đưa vào hàng đợi.

3. Tasklet:
    Tasklet là một kỹ thuật xử lý ngắt dựa trên softirq. Tasklet hoạt động tương tự như softirq, nhưng không cho phép song song hóa (concurrency) giữa các tasklet cùng loại. Tasklet thích hợp cho các tác vụ yêu cầu độ trễ thấp như softirq và không cần đến sự song song hóa.

    Ví dụ: Xử lý tín hiệu từ thiết bị ngoại vi như cảm biến, sau khi top half đã nhận tín hiệu và đưa vào hàng đợi.


