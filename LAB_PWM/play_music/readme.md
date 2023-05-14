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

> Xử lý tập tin WAV điều khiển sung PWM
Tải thư viện libsndfile lưu vào gcc biên dịch cross_compile
Link: http://www.mega-nerd.com/libsndfile/#Download

Giải file nén: libsndfile-1.0.28.tar.gz
Giải nén: tar -xzf libsndfile-1.0.28.tar.gz
Truy cập thư mục: cd libsndfile-1.0.28/
    export CC=/home/hieuhoangwork/work/BBB/gcc-linaro-6.5.0-2018.12-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc 
    export CXX=/home/hieuhoangwork/work/BBB/gcc-linaro-6.5.0-2018.12-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++
Cài libsndfile cho trình biên dịch ARM:
    ./configure --host=arm-linux-gnueabihf --prefix=/home/hieuhoangwork/work/BBB/libsndfile-install
    make
    make install

> Copy tập tin .wav sang thư mục pwm_play_music
encode_mp3_to_wav/output.wav -> pwm_play_music

> Copy file libsndfile.so.1 sang BBB
Tìm file:
    find /home/hieuhoangwork/work/BBB/libsndfile-install -name libsndfile.so.1
Kết quả:
    /home/hieuhoangwork/work/BBB/libsndfile-install/lib/libsndfile.so.1
Copy sang usb hoặc dùng lệnh cp -rf

> Trên BBB
Copy file libsndfile.so.1:
    cp -rf /media/rootfs/libsndfile.so.1 ./

Thêm thư viện libsndfile.so.1 vào hệ thống

Cách 1:
Tạo biến môi trường:
    export LD_LIBRARY_PATH=/root/LAB_PWM/pwm_play_music/lib:$LD_LIBRARY_PATH

Cách 2: 
    cd /usr/lib/
    mv ~/LAB_PWM/pwm_play_music/lib/libsndfile.so.1 ./
    chmod 755 /usr/lib/libsndfile.so.1