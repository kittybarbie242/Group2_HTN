# Cấu hình LED nhấp nháy nâng cao (FreeRTOS)

## Mục đích
- Sử dụng FreeRTOS để nhấp nháy 3 LED với tần số khác nhau: 3Hz, 10Hz, 25Hz.  
- Thực hiện nâng cao: dùng **1 hàm tác vụ duy nhất**, truyền tham số để thay đổi chân LED và tần số.

## Ý tưởng
- Tạo 1 hàm tác vụ `led_blink_task(void *pvParameters)` nhận tham số cấu hình LED.  
- Từ tham số, xác định chân GPIO và tần số nhấp nháy.  
- Tạo 3 tác vụ cùng hàm này với các tham số khác nhau để nhấp nháy 3 LED độc lập.

## Cấu hình phần cứng
- STM32 hoặc board tương thích FreeRTOS.  
- 3 LED nối với các chân GPIO (GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2).  
- Nguồn: 3.3V hoặc 5V tùy board.

## Mô tả cơ bản
- Tác vụ `led_blink_task`:
  - Nhận tham số `LedConfig` (GPIO, frequency).  
  - Dùng `vTaskDelayUntil()` để nhấp nháy LED ổn định theo tần số.
- Trong `main()`:
  - Tạo 3 task với các tham số khác nhau cho 3 LED.
- Kết quả:  
  - LED1 nhấp nháy 3Hz  
  - LED2 nhấp nháy 10Hz  
  - LED3 nhấp nháy 25Hz  
- Các LED nhấp nháy độc lập, không ảnh hưởng nhau nhờ FreeRTOS quản lý tác vụ song song.

**Source code:** [Bài 11.2](11.2.c)  

