ioremap là một hàm trong nhân Linux cho phép ánh xạ địa chỉ vật lý của một thiết bị I/O thành địa chỉ ảo của nhân. Nhân tạo ra một bảng trang, tức là ánh xạ địa chỉ ảo với địa chỉ vật lý được yêu cầu. Khi chúng ta thực hiện iounmap, ánh xạ này sẽ bị hủy

/* GPIO_OE_OFFSET / 4 */
Con trỏ base_addr là kiểu uint32 ~ 4bytes
Toán tử *(base_addr + GPIO_OE_OFFSET / 4) là toán tử tăng giảm con trỏ. Vậy để dịch đi off set là 134 thì cần tăng lên 134/4 đơn vị.

/* lsmod */
Kiểm tra các module đang có trên hệ thống 
