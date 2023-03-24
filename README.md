# DACN
Đồ án Hệ Thống Aquaponics ứng dụng Thinhspeak và Blynk Iot để giám sát, lưu trữ, cảnh báo và điều khiển từ xa.
Sử dụng Esp8266 và một số module khác liên quan như cảm biển đo pH, DHT11, DS18B20, Rellay 4 kênh, cảm biến siêu âm...
Nguyên lý hoạt động của toàn bộ hệ thống:
      Hệ thống sau khi cấm nguồn sẽ bắt đầu tiến hành chạy chương trình, vi điều khiển truy cập với 
      mạng WiFi đã được thiết lập sẵn trong phần lập trình. Tiếp theo, dữ liệu đọc được từ các cảm biến sẽ gửi tới ESP8266 để
      mã hóa và hiển thị lên app Blynk và trang mạng Thingspeak. Quá trình tiếp theo cảm biến siêu âm sẽ bắt đầu đo khoảng cách 
      mực nước nếu mực nước ở khay trồng rau đạt yêu cầu thì máy bơm sẽ tắt, nếu khoảng cách mực nước chưa đủ thì vi điều khiển sẽ kích 
      relay để thực hiện máy bơm nước vào khay trồng rau tới khi đến khoảng cách cài đặt thì dừng bơm và lập lại chu kỳ. 
      Ngoài ra, vi điều khiển có thể thực hiện bật tắt relay theo quyết định của người dùng.
