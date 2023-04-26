Producer - Consumer

1. Quantity:
Producer:2
Consumer: 3
Buffer: 2 (A and B)

2. Details:
- Producer:
+ Gồm 2 đầu bếp: 
    Donatello quản lý buffer A, sản xuất 2 món mặn: Fettuccine Chicken Alfredo & Garlic Sirloin Steak
    Portecelli quản lý buffer B, sản xuất 2 món chay: Pistachio Pesto Pasta & Avocado Fruit Salad
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
- 2 bộ đệm giới hạn dùng chung, có kiểu int, MAX_BUFFER_SIZE = 10
    0 Khay trống
    1 Khay có món chay
    2 Khay có món mặn