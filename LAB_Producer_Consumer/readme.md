Producer - Consumer

1. Quantity:
Producer:2
Consumer: 3
Buffer: 2 (mặn và chay)

2. Details:
- Producer:
+ Gồm 2 đầu bếp: 
    Donatello quản lý buffer mặn, sản xuất 2 món mặn: Fettuccine Chicken Alfredo & Garlic Sirloin Steak
    Portecelli quản lý buffer chay, sản xuất 2 món chay: Pistachio Pesto Pasta & Avocado Fruit Salad
+ Mỗi đầu bếp chọn ngẫu nhiên một trong 2 món thêm vào buffer của họ với tốc độ (1->5s)

- Consumer:
+ Người ăn mặn
+ Người ăn chay
+ Ăn đc cả 2 loại

Khách hàng cần đợi nếu không có bất kỳ món ăn nào. 
Các đầu bếp cần đợi nếu buffer tương ứng của họ đầy.
Sau khi một khách lấy một mặt hàng, các khách khác tương ứng sẽ đợi 10-15s để lấy món tiếp theo.

3. Required:
- Tạo tiến trình con bằng fork()
- Chia sẻ dữ liệu bằng mmap()
- Sử dụng semaphores POSIX để đồng bộ hóa quy trình
- 2 bộ đệm giới hạn dùng chung giữa các processs, MAX_BUFFER_SIZE = 10
    + Bộ đệm đầu tiên chứa món mặn với 2 loại ID là 11, 12
    + Bộ đệm thứ hai chứa món chay với 2 loại ID là 21, 22