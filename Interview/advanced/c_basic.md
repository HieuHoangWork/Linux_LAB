# C cơ bản
> 1. Biến static

Biến static có các đặc điểm sau:

    + Phạm vi (scope): Biến static có phạm vi cục bộ đối với hàm hoặc khối mã mà nó được khai báo, nhưng nó không thể truy cập từ bên ngoài hàm hoặc khối mã đó.

    + Tuổi thọ (lifespan): Biến static được khởi tạo một lần duy nhất khi chương trình bắt đầu thực thi và giữ giá trị của nó cho đến khi chương trình kết thúc. Điều này có nghĩa là giá trị của biến static không bị mất đi khi hàm hoặc khối mã chứa nó kết thúc.

Khi sử dụng biến static trong C, bạn thường gặp hai trường hợp sau:

    + Biến static trong hàm: Khi khai báo một biến static trong hàm, biến này sẽ chỉ được khởi tạo một lần và giữ giá trị của nó giữa các lần gọi hàm. Điều này hữu ích khi bạn muốn giữ lại giá trị của biến giữa các lần gọi hàm mà không cần sử dụng biến toàn cục.
    
    + Biến static cấp độ tập tin (file-level): Biến static cấp độ tập tin được khai báo ở cấp độ tập tin (ngoài các hàm), và chỉ có thể truy cập được từ các hàm trong cùng một tập tin mã nguồn. Điều này giúp giới hạn phạm vi của biến, ngăn ngừa truy cập trực tiếp từ mã nguồn khác và giúp bảo vệ dữ liệu.

Ví dụ về biến static trong hàm:

#include <stdio.h>

void increment() {
    static int count = 0;
    count++;
    printf("Count: %d\n", count);
}

int main() {
    increment(); // Count: 1
    increment(); // Count: 2
    increment(); // Count: 3
    return 0;
}

Ở ví dụ trên, biến count được khai báo là static, vì vậy nó sẽ không bị mất giá trị giữa các lần gọi hàm increment(). Kết quả là, biến count sẽ tăng lên mỗi khi hàm increment() được gọi.

> 2. So sánh hàm với hàm inline với macro

Hàm, hàm inline và macro là ba khái niệm khác nhau trong ngôn ngữ lập trình C, mỗi loại có các ưu và nhược điểm riêng. Dưới đây là một so sánh giữa chúng:

- Hàm (Function):

    + Hàm là một khối mã được định nghĩa riêng biệt, thực hiện một tác vụ cụ thể. Mỗi hàm đều có một địa chỉ cụ thể có thể truy cập được thông qua con trỏ hàm

    + Khi gọi hàm, chương trình sẽ thực hiện việc truyền tham số, lưu trữ thông tin về ngăn xếp (stack), và nhảy đến địa chỉ của hàm.

    + Hàm có thể gây ra độ trễ nhỏ do việc gọi hàm (truy cập vào giá trị địa chỉ hàm) và trả về giá trị.

    + Hàm cho phép kiểm soát tốt hơn về phạm vi và truy cập biến.

    + Các hàm định nghĩa trong C không được thực thi inline một cách mặc định.

- Hàm inline (Inline function):

    + Hàm inline là một loại hàm đặc biệt trong C, được chỉ định bằng từ khoá inline.

    + Mục đích của hàm inline là giảm độ trễ khi gọi hàm bằng cách chèn trực tiếp mã của hàm vào nơi gọi hàm (như macro), thay vì thực hiện gọi hàm thông thường.

    + Hàm inline giúp giảm độ trễ, tuy nhiên, nó có thể làm tăng kích thước mã nhị phân của chương trình (do việc chèn mã hàm vào nhiều nơi).

    + Hàm inline vẫn kiểm soát tốt phạm vi và truy cập biến như hàm thông thường.

    + Việc sử dụng hàm inline là một gợi ý cho trình biên dịch, nhưng không phải lúc nào trình biên dịch cũng tuân thủ gợi ý đó.

- Macro (Macro):

    + Macro là một đoạn mã được xử lý bởi trình tiền xử lý (preprocessor) trước khi biên dịch chương trình.

    + Khi macro được gọi, trình tiền xử lý sẽ thay thế lệnh gọi macro bằng mã của macro trước khi biên dịch.

    + Macro không gây ra độ trễ do gọi hàm, nhưng có thể làm tăng kích thước mã nhị phân của chương trình.

    + Macro không kiểm soát tốt phạm vi và truy cập biến như hàm và hàm inline.

    + Macro có thể dẫn đến các vấn đề liên quan đến thứ tự đánh giá, vì chúng thực hiện thay thế văn bản mà không quan tâm đến ngữ cảnh.

Ví dụ: 

// Hàm thông thường
int add(int a, int b) {
    return a + b;
}

// Hàm inline
inline int add_inline(int a, int b) {
    return a + b;
}

// Macro
#define ADD_MACRO(a, b) ((a) + (b))

Khi lựa chọn giữa hàm, hàm inline và macro, bạn nên xem xét các yếu tố sau:

    + Nếu bạn cần kiểm soát tốt phạm vi và truy cập biến, hãy sử dụng hàm hoặc hàm inline.

    + Nếu bạn muốn giảm độ trễ do gọi hàm và không quá lo lắng về kích thước mã nhị phân, hãy sử dụng hàm inline hoặc macro.

    + Nếu bạn cần một đoạn mã linh hoạt hơn và không cần kiểm soát phạm vi và truy cập biến, hãy sử dụng macro.

    + Trong nhiều trường hợp, hàm inline là một giải pháp tốt để kết hợp lợi ích của cả hàm thông thường và macro.
    
Tuy nhiên, cần lưu ý rằng các trình biên dịch hiện đại rất thông minh và có thể tự động tối ưu hóa mã nguồn của bạn. Do đó, đôi khi không cần thiết phải quá lo lắng về việc chọn giữa hàm, hàm inline và macro. Thay vào đó, hãy tập trung vào việc viết mã nguồn rõ ràng, dễ đọc và dễ bảo trì.

> 3. Structure

Trong ngôn ngữ lập trình C, "structure" (cấu trúc) là một kiểu dữ liệu tổng hợp (composite data type) cho phép bạn kết hợp nhiều biến với các kiểu dữ liệu khác nhau thành một đơn vị lưu trữ thông tin. Cấu trúc giúp tổ chức dữ liệu theo cách phù hợp hơn, làm cho mã nguồn dễ hiểu và bảo trì hơn.

Để định nghĩa một cấu trúc, bạn sử dụng từ khoá struct, sau đó khai báo tên của cấu trúc và các trường dữ liệu bên trong cấu trúc. Các trường dữ liệu có thể có các kiểu dữ liệu khác nhau, bao gồm cả các kiểu dữ liệu cơ bản và kiểu dữ liệu tự định nghĩa.

Ví dụ về cấu trúc trong C:

#include <stdio.h>

// Định nghĩa cấu trúc Person
struct Person {
    char name[50];
    int age;
    float height;
};

int main() {
    // Khai báo và khởi tạo một biến kiểu struct Person
    struct Person person1 = {"Alice", 30, 165.5};

    // Truy cập và thay đổi giá trị các trường của cấu trúc
    printf("Name: %s\n", person1.name);
    printf("Age: %d\n", person1.age);
    printf("Height: %.1f\n", person1.height);

    person1.age = 31;
    printf("New age: %d\n", person1.age);

    return 0;
}

Trong ví dụ trên, chúng ta định nghĩa một cấu trúc Person với các trường name, age và height. Sau đó, chúng ta khai báo và khởi tạo một biến person1 kiểu struct Person, truy cập và thay đổi giá trị các trường của cấu trúc thông qua toán tử . (dấu chấm).

Cấu trúc thường được sử dụng để đại diện cho các đối tượng phức tạp hơn, như điểm trong không gian hai chiều hoặc ba chiều, ngày tháng, thông tin cá nhân, v.v. Chúng giúp làm cho mã nguồn của bạn dễ hiểu và bảo trì hơn.

> 4. Enum
liệt kê cho phép bạn định nghĩa một tập hợp các hằng số nguyên (integer constants) có tên gọi. enum giúp làm cho mã nguồn của bạn dễ đọc, hiểu và bảo trì hơn so với việc sử dụng các hằng số nguyên trực tiếp.

Để định nghĩa một enum, bạn sử dụng từ khoá enum, sau đó khai báo tên của kiểu liệt kê và các giá trị bên trong nó. Các giá trị liệt kê sẽ được gán các giá trị nguyên tăng dần, bắt đầu từ 0, trừ khi bạn chỉ định giá trị cụ thể cho chúng.

Ví dụ về enum trong C:

#include <stdio.h>

// Định nghĩa enum DayOfWeek
enum DayOfWeek {
    Sunday,
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday
};

int main() {
    // Khai báo và khởi tạo một biến kiểu enum DayOfWeek
    enum DayOfWeek today = Wednesday;

    // Sử dụng biến enum trong câu lệnh điều kiện
    if (today == Sunday || today == Saturday) {
        printf("Today is a weekend day.\n");
    } else {
        printf("Today is a weekday.\n");
    }

    return 0;
}

Trong ví dụ trên, chúng ta định nghĩa một enum DayOfWeek với các giá trị từ Sunday đến Saturday. Mặc định, Sunday sẽ được gán giá trị 0, Monday được gán giá trị 1, và cứ tiếp tục cho đến Saturday được gán giá trị 6. Sau đó, chúng ta khai báo và khởi tạo một biến today kiểu enum DayOfWeek và sử dụng biến này trong một câu lệnh điều kiện.

Sử dụng enum giúp làm rõ nghĩa của các giá trị nguyên trong mã nguồn của bạn, giúp tránh các lỗi liên quan đến việc sử dụng sai giá trị nguyên.

> 5. Từ khóa Typedef

Trong ngôn ngữ lập trình C, từ khóa typedef được sử dụng để định nghĩa một bí danh (alias) cho một kiểu dữ liệu đã tồn tại. typedef giúp đơn giản hóa mã nguồn của bạn, làm cho nó dễ đọc và hiểu hơn, đặc biệt khi làm việc với các kiểu dữ liệu phức tạp như con trỏ hàm, cấu trúc (struct) và liệt kê (enum).

Khi sử dụng typedef, bạn đặt từ khóa typedef trước kiểu dữ liệu mà bạn muốn tạo bí danh, sau đó đặt tên bí danh. Sau khi định nghĩa xong, bạn có thể sử dụng bí danh này thay vì kiểu dữ liệu gốc.

Ví dụ về việc sử dụng typedef trong C:

#include <stdio.h>

// Định nghĩa một cấu trúc Point
struct Point {
    int x;
    int y;
};

// Sử dụng typedef để tạo bí danh cho struct Point và int
typedef struct Point Point;
typedef int Integer;

int main() {
    // Sử dụng bí danh Point và Integer thay vì struct Point và int
    Point p1 = {3, 4};
    Integer a = 5;

    printf("Point: (%d, %d)\n", p1.x, p1.y);
    printf("Integer: %d\n", a);

    return 0;
}

Trong ví dụ trên, chúng ta sử dụng typedef để tạo bí danh Point cho struct Point và bí danh Integer cho int. Sau đó, chúng ta sử dụng các bí danh này để khai báo và khởi tạo các biến p1 và a. Việc sử dụng typedef giúp mã nguồn của chúng ta dễ đọc và hiểu hơn.

typedef đặc biệt hữu ích khi làm việc với các kiểu dữ liệu phức tạp, ví dụ như con trỏ hàm, cấu trúc lồng nhau, v.v. Nó giúp đơn giản hóa mã nguồn và giảm thiểu lỗi khi khai báo các biến với kiểu dữ liệu phức tạp.

