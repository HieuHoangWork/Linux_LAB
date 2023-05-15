> Step by step
Chuyển đổi file MP3 thành dữ liệu âm thanh không nén:
    Đầu tiên, bạn cần chuyển đổi file MP3 của bạn thành một định dạng âm thanh không nén, chẳng hạn như WAV. Bạn có thể sử dụng libmpg123 để giải mã file MP3 như đã thảo luận ở trên.

Đọc dữ liệu âm thanh không nén:
    Sau khi có file WAV, bạn cần đọc dữ liệu từ file này. File WAV chứa dữ liệu âm thanh dưới dạng mẫu số 16-bit. Mỗi mẫu là một số nguyên biểu diễn áp suất âm thanh tại một thời điểm nhất định.

Chuyển đổi mẫu âm thanh thành chu kỳ công tác PWM:
    Mỗi mẫu âm thanh cần được chuyển đổi thành một giá trị chu kỳ công tác PWM. Bạn có thể chuyển đổi trực tiếp giá trị mẫu 16-bit thành giá trị chu kỳ công tác, nhưng cần lưu ý rằng độ phân giải của hệ thống PWM của bạn có thể không đủ cao để tái tạo tất cả các mức âm thanh.

Gửi chu kỳ công tác đến PWM:
    Sau khi chuyển đổi mẫu âm thanh thành chu kỳ công tác, bạn cần gửi nó đến hệ thống PWM của bạn. Bạn có thể sử dụng hàm set_PWM_duty trong chương trình của bạn để làm điều này.

Lặp lại với mỗi mẫu âm thanh:
    Bạn cần lặp lại quy trình này với mỗi mẫu âm thanh trong file WAV. Điều này sẽ tạo ra một dãy chu kỳ công tác PWM phản ánh dữ liệu âm thanh gốc.

> Tạo ra file .wav từ file .mp3 trên x86
1. Cài đặt libmpg123:
sudo apt-get update
sudo apt-get install libmpg123-dev

2. Cài đặt libsndfile:
sudo apt-get update
sudo apt-get install libsndfile1-dev

3. Chuyển file định dạng mp3 sang wav
cd encode_mp3_to_wav
make all
./encode music.mp3 /* Tạo ra file .wav */

> Tải thư viện libsndfile và build thư viện này dùng cho ARM
1. Link: 
    http://www.mega-nerd.com/libsndfile/#Download
2. Giải file nén: 
    libsndfile-1.0.28.tar.gz
3. Giải nén: 
    tar -xzf libsndfile-1.0.28.tar.gz
4. Truy cập thư mục: 
    cd libsndfile-1.0.28/
    export CC=/home/hieuhoangwork/work/BBB/gcc-linaro-6.5.0-2018.12-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc 
    export CXX=/home/hieuhoangwork/work/BBB/gcc-linaro-6.5.0-2018.12-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++
5. Cài libsndfile cho trình biên dịch ARM:
    ./configure --host=arm-linux-gnueabihf --prefix=/home/hieuhoangwork/work/BBB/libsndfile-install
    make
    make install

> Copy tập tin .wav sang thư mục pwm_play_music
encode_mp3_to_wav/output.wav -> pwm_play_music

> Copy file thư mục lib tại ~/work/BBB/libsndfile-install/lib sang BBB. Đây là thư mục chứa file shared lib dành cho thư viện libsndfile
Nén thư mục:
    cd /home/hieuhoangwork/work/BBB/libsndfile-install
    tar -czvf lib.tar.gz ./lib/
Copy sang thẻ nhớ bằng cp -rf hoặc copy trực tiếp trên MobaXterm

> Copy file thực thi "main" trong thư mục pwm_play_music và file "output.wav" trong thư mục này sang BBB
Copy sang thẻ nhớ bằng cp -rf hoặc copy trực tiếp trên MobaXterm

> Trên BBB
Giải nén file thư viện lib.tar.gz:
    tar -xzvf lib.tar.gz
Tạo biến môi trường để sử dụng thư viện (trong đó /root/lib là đường dẫn trực tiếp đến thư viện lib vừa đc copy vào BBB):
    export LD_LIBRARY_PATH=/root/lib:$LD_LIBRARY_PATH 
Chạy chương trình pwm_play_music
    ./main

Chương trình chỉ chạy chính xác khi tại đường dẫn của nó có đủ các file:
    lib
    lib.tar.gz
    main
    output.wav