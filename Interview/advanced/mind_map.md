# C cơ bản
> 1. Biến static
Biến static có các đặc điểm sau:
    + Phạm vi: Cục bộ đối với hàm hoặc khối mã mà nó được khai báo
    + Tuổi thọ: Khởi tạo một lần duy nhất khi chương trình bắt đầu thực thi và giữ giá trị của nó cho đến khi chương trình kết thúc. 
VD:
    + Biến static trong hàm: Biến này sẽ được khởi tạo một lần và giữ giá trị của nó giữa các lần gọi hàm. 
    + Biến static cấp độ tập tin: Được khai báo bên ngoài hàm, giống với biến global nhưng có thêm từ khóa static. Biến này có thể được truy cập bởi các hàm trong tập tin khai báo nhưng không thể truy cập từ tập tin bên ngoài

> 2. So sánh hàm với hàm inline với macro
- Hàm:
    + Hàm là một khối mã thực hiện một nhiệm vụ nhất định
    + Khi gọi hàm, chương trình sẽ thực hiện việc truyền tham số, lưu trữ thông tin về ngăn xếp (stack), và nhảy đến địa chỉ của hàm.
    + Hàm có thể gây ra độ trễ nhỏ do việc gọi hàm (truy cập vào giá trị địa chỉ hàm) và trả về giá trị.
    + Hàm cho phép kiểm soát tốt hơn về phạm vi và truy cập biến.

- Hàm inline:
    + Được chỉ định bằng từ khoá inline.
    + Giảm độ trễ khi gọi hàm bằng cách chèn trực tiếp mã của hàm vào nơi gọi hàm
    + Nó có thể làm tăng kích thước mã nhị phân của chương trình (do việc chèn mã hàm vào nhiều nơi).
    + Hàm inline vẫn kiểm soát tốt phạm vi và truy cập biến như hàm thông thường.

- Macro:
    + Macro là một đoạn mã được xử lý bởi trình tiền xử lý.
    + Trình tiền xử lý sẽ thay thế lệnh gọi macro bằng mã của macro trước khi biên dịch.
    + Macro không gây ra độ trễ do gọi hàm, nhưng làm tăng kích thước mã nhị phân của chương trình.
    + Macro không kiểm soát tốt phạm vi và truy cập biến như hàm và hàm inline.

-> Các thông số cần khi lựa chọn sử dụng hàm:
    + Kích thước binary
    + Tốc độ xử lý
    + Kiểm soát phạm vi và truy cập biến

> 3. Structure
Structure là kiểu dữ liệu do người dùng tự định nghĩa. Các trường dữ liệu là kiểu dữ liệu đã tồn tại, bao gồm cả các kiểu dữ liệu cơ bản và kiểu dữ liệu tự định nghĩa.
VD:
    struct Person 
    {
        char name[50];
        int age;
        float height;
    };
    Struct mô tả thông tin của một người với tên được lưu trữ kiểu char, tuổi kiểu int và chiều cao kiểu float

Các trường dữ liệu của struct có thể được truy cập thông qua toán tử "." hoặc toán từ "->" nếu sử dụng con trỏ

> 4. Enum
Kiểu dữ liệu liệt kê cho phép bạn định nghĩa một tập hợp các hằng số nguyên có tên gọi. Enum giúp làm cho mã nguồn của bạn dễ đọc, hiểu và bảo trì hơn so với việc sử dụng các hằng số nguyên trực tiếp.
Để định nghĩa một enum, bạn sử dụng từ khoá enum, sau đó khai báo tên của kiểu liệt kê và các giá trị bên trong nó. Các giá trị liệt kê sẽ được gán các giá trị nguyên tăng dần, bắt đầu từ 0, trừ khi bạn chỉ định giá trị cụ thể cho chúng.
VD:
    Để quản lý trạng thái của LED mình có thể sử dụng enum như sau:
    enum STATE_LED{
        STATE_RED,
        STATE_BLUE,
        STATE_GREEN
    };

> 5. Từ khóa Typedef
Từ khóa Typedef dùng để định nghĩa lại một kiểu dữ liệu đã tồn tại. Typedef giúp đơn giản hóa mã nguồn của bạn, làm cho nó dễ đọc và hiểu hơn, sử dụng chung cùng với struct và enum.
VD:
    Thay vì khai báo 2 struct như sau:
        struct Person Person_1;
        struct Person Person_2;
    Thì chúng ta có thể sử dụng typedef như sau:
        typedef struct Person Person;
        Person Person_1;
        Person Person_2;

# Process
> 1. Phân biệt Program, Process
- Program (chương trình):
    + Là một tập hợp các dòng lệnh được viết bằng ngôn ngữ lập trình để thực hiện một nhiệm vụ cụ thể.
    + Được lưu trữ trên ổ cứng dưới dạng binary hoặc source code.
    + Không thực thi bởi chính nó mà cần một tiến trình (process) để thực thi nó.
 Ví dụ: Khi bạn viết một ứng dụng bằng C, Java, Python, v.v., bạn tạo ra một chương trình.

- Process (tiến trình):
    + Tiến trình là một phiên bản thực thi của một chương trình đang được thực hiện trên CPU của máy tính.
    + Khi một chương trình được chạy, hệ điều hành tạo ra một tiến trình và cấp phát cho nó các tài nguyên (như bộ nhớ, thiết bị ngoại vi, ...)
    + Mỗi tiến trình có một không gian địa chỉ riêng biệt và không thể trực tiếp truy cập vào không gian địa chỉ của tiến trình khác, trừ khi thông qua các phương thức liên tiến trình IPC.
Ví dụ: Khi bạn mở trình duyệt web, một tiến trình của trình duyệt sẽ được hệ điều hành tạo ra và chạy chương trình trình duyệt.

> 2. Process layout bao gồm những segment nào?
Layout của một process trong bộ nhớ chia thành các segments khác nhau, mỗi segments có vai trò và chức năng riêng.
    + Text segment: Chứa mã máy của chương trình, thường chỉ đọc để tránh việc các lỗi khi chương trình running vô tình ghi đè lên vùng này.
    + Data segment: Chứa các biến toàn cục và biến tĩnh được khởi tạo. Segment này có thể đọc và ghi
    + BSS segment: Chứa các biến toàn cục và biến tĩnh chưa được khởi tạo hoặc được khởi tạo với giá trị 0. Khi tiến trình bắt đầu, hệ điều hành đặt giá trị 0 cho tất cả các biến trong phân đoạn BSS.
    + Heap segment: Chứa bộ nhớ động được cấp phát bởi chương trình thông qua các hàm như malloc(), calloc(), realloc(). Quản lý bộ nhớ heap là trách nhiệm của chương trình, bao gồm việc giải phóng bộ nhớ đã cấp phát khi không còn sử dụng.
    + Stack segment: Chứa ngăn xếp (stack) gọi hàm của chương trình, bao gồm các biến cục bộ, thông tin trả về và các địa chỉ trả về của các hàm.

> 3. Context Switching
Context switching là quá trình lưu trữ trạng thái của một tiến trình đang chạy để sau đó tiếp tục thực thi tiến trình khác. Đây là một hoạt động quan trọng của hệ điều hành trong việc quản lý đa nhiệm (multitasking) và phân chia tài nguyên của hệ thống cho nhiều tiến trình một cách công bằng.

Quá trình context switching diễn ra như sau:
    + Tiến trình hiện tại bị ngắt: Vd: Hết thời gian chạy được cấp phát, chờ tín hiệu từ thiết bị ngoại vi hoặc đợi một sự kiện nào đó.
    + Lưu trạng thái tiến trình hiện tại: Hệ điều hành lưu trạng thái của tiến trình hiện tại vào bộ nhớ, bao gồm các giá trị của thanh ghi, con trỏ ngăn xếp, con trỏ chỉ thị, ... 
    + Chọn tiến trình tiếp theo: Hệ điều hành sử dụng một thuật toán lập lịch để chọn tiến trình tiếp theo sẽ được chạy. Vd: Round Robin, Priority Scheduling
    + Nạp trạng thái tiến trình mới: Hệ điều hành nạp trạng thái của tiến trình mới vào bộ nhớ và thiết lập các giá trị của thanh ghi, con trỏ ngăn xếp, con trỏ chỉ thị và các thông tin liên quan khác.
    + Tiếp tục thực thi tiến trình mới: Hệ điều hành tiếp tục thực thi tiến trình mới, bắt đầu từ điểm dừng cuối cùng của tiến trình này.
Quá trình context switching có chi phí về thời gian và tài nguyên, do đó việc tối ưu hóa quá trình chuyển đổi ngữ cảnh là rất quan trọng trong việc cải thiện hiệu suất hệ thống.

> 4. Phân biệt Zombie Process/Orphane Process
Zombie process (tiến trình zombie) và orphan process (tiến trình mồ côi) là hai khái niệm liên quan đến tiến trình (process) trong hệ điều hành.

- Zombie process (tiến trình zombie):
    + Một tiến trình được gọi là zombie khi nó đã kết thúc nhưng vẫn còn tồn tại trong bảng tiến trình của hệ điều hành.
    + Điều này xảy ra khi parent process không đọc thông tin trạng thái kết thúc của child process thông qua hàm wait() hoặc các biến thể khác của nó.
    + Tiến trình zombie không sử dụng tài nguyên hệ thống, nhưng vẫn chiếm một vị trí trong bảng tiến trình.
    -> Nếu có quá nhiều tiến trình zombie thì gây ra đầy bảng tiến trình và hệ thống không thể tạo tiến trình mới. 

- Orphan process (tiến trình mồ côi):
    + Một tiến trình được coi là mồ côi khi parent process của nó kết thúc trước khi child process kết thúc.
    + Khi tiến trình cha kết thúc, hệ điều hành gán tiến trình mồ côi cho tiến trình "init" (process ID 1). Điều này giúp đảm bảo quy trình cleanup khi tiến trình con này kết thúc.
    + Tiến trình mồ côi vẫn tiếp tục thực thi bình thường và sử dụng tài nguyên hệ thống, nhưng không còn được quản lý bởi tiến trình cha ban đầu.

> 5. Process image
Process image là một cấu trúc dữ liệu trong hệ điều hành mô tả toàn bộ thông tin về một tiến trình khi nó đang chạy. Hình ảnh tiến trình bao gồm cả trạng thái và không gian bộ nhớ của tiến trình, giúp hệ điều hành quản lý và điều khiển tiến trình đó.

Một số thành phần chính của hình ảnh tiến trình bao gồm:
    + Code segment: Chứa mã máy của chương trình.
    + Data segment: Chứa các biến toàn cục và biến tĩnh được khởi tạo.
    + BSS segment: Chứa các biến toàn cục và biến tĩnh chưa được khởi tạo hoặc được khởi tạo với giá trị 0.
    + Heap segment: Chứa bộ nhớ động được cấp phát bởi chương trình thông qua các hàm như malloc(), calloc(), realloc() =
    + Stack segment: Chứa ngăn xếp gọi hàm của chương trình, bao gồm các biến cục bộ (local), thông tin trả về (return information) và các địa chỉ trả về (return addresses) của các hàm.

    + Process Control Block (PCB): Chứa thông tin về tiến trình, bao gồm ID tiến trình, trạng thái tiến trình (running, ready, waiting, v.v.), thông tin về đăng ký (registers), con trỏ ngăn xếp (stack pointer), con trỏ chỉ thị (instruction pointer), thông tin về ưu tiên, thông tin về tài nguyên và các thông tin khác cần thiết để quản lý tiến trình.    


> 6. Khi Child Process kết thúc vòng đời thực thi của mình, signal nào được gửi đến Parent Process
Khi một Child Process kết thúc vòng đời thực thi của mình, một tín hiệu SIGCHLD được gửi đến Parent Process để thông báo về sự kiện này.
SIGCHLD là một tín hiệu được gửi đến Parent Process khi một trong những sự kiện sau xảy ra:

    + Child Process kết thúc (terminated).
    + Child Process bị dừng (stopped), ví dụ như do một tín hiệu SIGSTOP hoặc SIGTSTP.
    + Child Process tiếp tục chạy sau khi bị dừng (resumed), ví dụ như do một tín hiệu SIGCONT.

Khi nhận được tín hiệu SIGCHLD, Parent Process có thể xử lý tình huống này theo nhiều cách, chẳng hạn như gọi hàm wait() hoặc waitpid() để đọc trạng thái kết thúc của Child Process, thực hiện một số hành động cụ thể hoặc bỏ qua tín hiệu nếu không cần xử lý.

# Thread
> 1. Phân biệt giữa Program, Process và Thread
- Program (chương trình):
    + Là một tập hợp các dòng lệnh được viết bằng ngôn ngữ lập trình để thực hiện một nhiệm vụ cụ thể.
    + Được lưu trữ trên ổ cứng dưới dạng binary hoặc source code.
    + Không thực thi bởi chính nó mà cần một tiến trình (process) để thực thi nó.

- Process: 
    + Một tiến trình (process) là một phiên bản đang chạy của chương trình, được tải vào bộ nhớ và được thực thi bởi CPU.
    + Tiến trình có một không gian địa chỉ riêng biệt, bao gồm các phân đoạn mã, dữ liệu, ngăn xếp và heap.
    + Tiến trình sở hữu các tài nguyên hệ thống (ví dụ: file handles, sockets, v.v.) và có một trạng thái thực thi (ví dụ: running, waiting, terminated, v.v.).
    + Mỗi tiến trình có ít nhất một luồng thực thi (thread) và có thể có nhiều luồng.

- Thread:
    + Một luồng (thread) là một đơn vị nhỏ nhất của việc thực thi mã trong một tiến trình.
    + Các luồng chia sẻ không gian địa chỉ và tài nguyên của tiến trình mẹ (parent process) nhưng có bộ đăng ký (registers), con trỏ ngăn xếp (stack pointer) và ngăn xếp (stack) riêng.
    + Các luồng có thể thực thi đồng thời (concurrent) hoặc song song (parallel) trên nhiều nhân CPU hoặc lõi, cho phép chương trình thực hiện nhiều tác vụ cùng lúc.
    + Các luồng có trạng thái thực thi (ví dụ: running, waiting, terminated, v.v.) riêng biệt.    

> 2. So sánh giữa Process với Thread (memory, data sharing, process switching/thread switching ...)
- Bộ nhớ:
    + Process: Mỗi tiến trình có một không gian địa chỉ riêng biệt, bao gồm các phân đoạn mã, dữ liệu, ngăn xếp và heap. Do đó, các tiến trình không thể truy cập trực tiếp vào bộ nhớ của nhau.
    + Thread: Tất cả các luồng trong cùng một tiến trình chia sẻ không gian địa chỉ của tiến trình mẹ, bao gồm phân đoạn mã, dữ liệu, ngăn xếp và heap. Mỗi luồng có ngăn xếp riêng, nhưng chúng có thể truy cập vào phân đoạn dữ liệu và heap chung

- Chia sẻ dữ liệu::
    + Process: Các tiến trình không chia sẻ dữ liệu trực tiếp với nhau. Chúng cần sử dụng các phương tiện giao tiếp giữa các tiến trình IPC như pipes, sockets, shared memory, v.v., để chia sẻ dữ liệu.
    + Thread: Các luồng trong cùng một tiến trình có thể chia sẻ dữ liệu trực tiếp với nhau thông qua các biến toàn cục và heap, giúp việc truyền dữ liệu nhanh hơn và dễ dàng hơn.

- Chuyển đổi tiến trình/luồng (Process switching/Thread switching):
    + Process: Chuyển đổi giữa các tiến trình (process switching) đòi hỏi nhiều công việc hơn so với chuyển đổi giữa các luồng. Khi chuyển đổi tiến trình, hệ điều hành phải lưu trữ trạng thái của tiến trình hiện tại, tải trạng thái của tiến trình mới và cập nhật các cấu trúc dữ liệu liên quan. 
    + Thread: Chuyển đổi giữa các luồng (thread switching), các luồng chia sẻ không gian địa chỉ và tài nguyên của tiến trình mẹ. Khi chuyển đổi luồng, hệ điều hành chỉ cần lưu trữ và tải trạng thái của luồng (như bộ đăng ký và con trỏ ngăn xếp) và cập nhật các cấu trúc dữ liệu liên quan. 
    -> Chuyển đổi tiến trình có chi phí cao hơn.

- Tài nguyên hệ thống:
    + Process: Các tiến trình sở hữu các tài nguyên hệ thống riêng biệt, như file handles, sockets, v.v. Khi một tiến trình kết thúc, các tài nguyên của nó sẽ được giải phóng.
    + Thread: Tất cả các luồng trong cùng một tiến trình chia sẻ các tài nguyên hệ thống của tiến trình mẹ. Khi một luồng kết thúc, các tài nguyên chung vẫn được giữ lại cho các luồng khác.

- Khởi tạo và kết thúc:
    + Process: Việc khởi tạo và kết thúc một tiến trình có chi phí cao hơn so với luồng, bởi vì hệ điều hành phải cấp phát không gian địa chỉ mới và khởi tạo/cập nhật/cleanup các cấu trúc dữ liệu liên quan.
    + Thread: Việc khởi tạo và kết thúc một luồng có chi phí thấp hơn, bởi vì các luồng chia sẻ không gian địa chỉ và tài nguyên của tiến trình mẹ, giảm bớt công việc của hệ điều hành.

- Độc lập:
    + Process: Các tiến trình hoạt động độc lập với nhau. Sự cố của một tiến trình thường không ảnh hưởng đến các tiến trình khác.
    + Thread: Các luồng trong cùng một tiến trình không hoàn toàn độc lập với nhau. Một sự cố của một luồng có thể ảnh hưởng đến các luồng khác trong cùng tiến trình, ví dụ như việc truy cập bộ nhớ không hợp lệ.

> 3. Synchronous/Asynchronous là gì?
Synchronous (đồng bộ) và Asynchronous (bất đồng bộ) là hai thuật ngữ dùng để mô tả cách thức hoạt động của các hệ thống, giao tiếp, hay chức năng trong lập trình và mạng máy tính.
- Synchronous (đồng bộ):
    + Trong một hoạt động đồng bộ, các tác vụ được thực hiện tuần tự, nghĩa là một tác vụ phải hoàn thành trước khi tác vụ tiếp theo được bắt đầu.
    + Trong lập trình, một hàm đồng bộ sẽ chặn (block) luồng thực thi cho đến khi công việc hoàn thành. 
Ví dụ: việc đọc dữ liệu từ tệp tin, nếu hàm đọc tệp đồng bộ được sử dụng, chương trình sẽ chờ đợi cho đến khi việc đọc tệp hoàn thành trước khi tiếp tục thực thi.

- Asynchronous (bất đồng bộ):
    + Trong một hoạt động bất đồng bộ, các tác vụ có thể được thực hiện đồng thời mà không cần chờ đợi hoàn thành của tác vụ trước đó.
    + Trong lập trình, một hàm bất đồng bộ cho phép luồng thực thi tiếp tục công việc khác trong khi chờ đợi kết quả từ hàm đó. 
Ví dụ: việc đọc dữ liệu từ tệp tin, nếu hàm đọc tệp bất đồng bộ được sử dụng, chương trình có thể tiếp tục thực thi và xử lý các tác vụ khác trong khi đợi việc đọc tệp hoàn thành.

- Ưu điểm của Synchronous (đồng bộ):
    + Dễ dàng hiểu và lập trình, vì các tác vụ được thực hiện tuần tự.
    + Kiểm soát chặt chẽ hơn về thứ tự thực hiện các tác vụ, điều này đôi khi là yêu cầu bắt buộc của một số ứng dụng.
- Nhược điểm của Synchronous (đồng bộ):
    + Hiệu năng thấp hơn, bởi vì các tác vụ phải chờ đợi hoàn thành lẫn nhau, dẫn đến việc tài nguyên không được sử dụng hiệu quả.
    + Có thể dẫn đến việc chương trình bị "treo" (block) nếu một tác vụ mất quá nhiều thời gian để hoàn thành.

- Ưu điểm của Asynchronous (bất đồng bộ):
    + Hiệu năng cao hơn, bởi vì các tác vụ có thể thực hiện đồng thời, tận dụng tối đa tài nguyên và giảm độ trễ.
    + Khả năng mở rộng tốt hơn, đặc biệt khi xử lý các tác vụ phức tạp và mất nhiều thời gian.
- Nhược điểm của Asynchronous (bất đồng bộ):
    + Phức tạp hơn trong việc lập trình và kiểm soát, vì các tác vụ thực hiện đồng thời có thể dẫn đến các vấn đề như race conditions, deadlocks, v.v.
    + Đôi khi khó xác định thứ tự hoàn thành của các tác vụ, điều này có thể gây ra những hệ quả không mong muốn trong một số ứng dụng

> 4. Khái niệm về Mutex, Conditional Variable?
- Mutex (Mutual Exclusion):
    + Mutex là một công cụ đồng bộ hóa được sử dụng trong lập trình đa luồng để đảm bảo rằng chỉ có một luồng cùng một thời điểm được truy cập vào một tài nguyên hay một phần mã nguồn chung (critical section). Mutex giúp ngăn chặn các vấn đề xung đột khi truy cập vào tài nguyên chung, như race conditions.
    + Khi một luồng muốn truy cập vào critical section, nó sẽ cố gắng khóa (lock) mutex. Nếu mutex chưa bị khóa bởi luồng nào khác, luồng đó sẽ thành công và có quyền truy cập vào critical section. Nếu mutex đã bị khóa, luồng đó sẽ chờ đợi cho đến khi mutex được giải phóng (unlock) bởi luồng hiện đang giữ khóa. Sau khi hoàn thành công việc trong critical section, luồng sẽ giải phóng mutex, cho phép các luồng khác có cơ hội truy cập vào critical section.

- Conditional Variable (Biến Điều Kiện):
    + Conditional Variable là một công cụ đồng bộ hóa khác được sử dụng trong lập trình đa luồng, giúp các luồng chờ đợi cho đến khi một điều kiện nào đó được thỏa mãn. Biến điều kiện thường được kết hợp với mutex để đảm bảo rằng việc kiểm tra và chờ đợi điều kiện diễn ra một cách an toàn trong critical section.
    + Khi một luồng muốn chờ đợi điều kiện, nó sẽ khóa mutex và kiểm tra điều kiện. Nếu điều kiện chưa thỏa mãn, luồng đó sẽ chuyển sang trạng thái chờ đợi trên biến điều kiện và tự động giải phóng mutex. Khi điều kiện được thỏa mãn (thông thường do một luồng khác thay đổi trạng thái của tài nguyên chung), luồng đó sẽ thông báo cho một hoặc tất cả các luồng đang chờ đợi trên biến điều kiện. Các luồng chờ đợi sẽ tỉnh lại, khóa lại mutex và kiểm tra lại điều kiện trước khi tiếp tục thực thi.

> 5. Giải thích các khái niệm: atomic/non-atomic, shared resource, critical section.
- Atomic (Độc quyền): Khi chỉ có một luồng truy cập vào vùng shared resource hoặc critical section
- Non-atomic: Khi có nhiều luồng muốn truy cập và sử dụng shared resource hoặc critical section

- Shared Resource:
    + Shared Resource là một tài nguyên (biến, cấu trúc dữ liệu, tệp, v.v.) được truy cập bởi nhiều luồng trong một quá trình hoặc giữa các quá trình khác nhau. Khi nhiều luồng truy cập đồng thời vào một tài nguyên chia sẻ, có thể xảy ra các vấn đề xung đột nếu không có cơ chế đồng bộ hóa phù hợp, như mutex hoặc semaphore.

- Critical Section:
    + Critical Section là một phần mã nguồn mà trong đó một luồng truy cập thay đổi tài nguyên chia sẻ. Các công cụ đồng bộ hóa như mutex, semaphore, hoặc atomic operations được sử dụng để bảo vệ critical section, ngăn chặn các vấn đề liên quan đến đa luồng như race conditions hay deadlocks.

Tóm lại, atomic và non-atomic mô tả cách thức hoạt động của các tác vụ trong lập trình đa luồng; shared resource là tài nguyên được truy cập bởi nhiều luồng; và critical section là phần mã nguồn mà các luồng truy cập vào tài nguyên chia sẻ. Để đảm bảo tính ổn định

> 6. Nêu một vài trường hợp gây ra hiện tượng deadlock
Deadlock là tình huống mà các tiến trình hoặc luồng chờ đợi lẫn nhau để giải phóng tài nguyên mà chúng đang giữ, dẫn đến việc không có tiến trình nào có thể tiếp tục thực thi. 

Vd:
    + Tiến trình A giữ tài nguyên X và đang chờ để giành quyền truy cập vào tài nguyên Y. Đồng thời, tiến trình B giữ tài nguyên Y và đang chờ để giành quyền truy cập vào tài nguyên X. Cả hai tiến trình đều không giải phóng tài nguyên của mình trước khi yêu cầu tài nguyên khác, dẫn đến deadlock.
   
    + Deadlock có thể xảy ra khi có nhiều luồng chờ đợi một sự kiện mà không bao giờ xảy ra hoặc không được thông báo. Điều này thường xảy ra khi sử dụng các cơ chế đồng bộ hóa như biến điều kiện (conditional variables) hoặc semaphore.
   
    + Deadlock xảy ra khi có nhiều tiến trình hoặc luồng chờ đợi để giành quyền truy cập vào một tài nguyên giới hạn, nhưng không có cơ chế phân bổ tài nguyên hiệu quả.
