# Cấu hình một kênh ADC để đọc giá trị từ một biến trở. Chuyển đổi giá trị này thành điện áp và hiển thị lên terminal.
## Mục đích
Bài thực hành giúp chúng ta làm quen với việc đọc giá trị ADC và truyền dữ liệu qua UART từ STM32F1 đến máy tính. Thông qua ví dụ, chúng ta sẽ:

- Hiểu cách cấu hình ADC để đọc điện áp từ cảm biến hoặc biến trở.

- Nắm được cơ chế chuyển đổi giá trị ADC sang điện áp thực tế.

- Truyền dữ liệu ADC và điện áp qua UART để hiển thị trên máy tính.

- Thực hành với phần mềm terminal (Hercules/PuTTY) để theo dõi kết quả.

## Ý tưởng
- Sử dụng ADC1 trên STM32F103 với chân PA1 (Channel 1).

- Sử dụng USART1 với PA9 (TX) và PA10 (RX) để giao tiếp với máy tính.

- ADC đọc giá trị điện áp từ 0-3.3V và chuyển đổi sang giá trị số 12-bit (0-4095).

- Chương trình đọc giá trị ADC mỗi giây và gửi kết quả qua UART.

- Hiển thị cả giá trị ADC thô và điện áp tính toán (mV) trên terminal.

## Cấu hình
- **Bật clock** cho GPIOA, ADC1, và USART1.

- **Cấu hình chân ADC**:

   - PA1 (ADC1_IN1) ở chế độ Analog Input.

- **Cấu hình UART**:

  - PA9 (TX) ở chế độ Alternate Function Push-Pull, tốc độ 50MHz.

  - PA10 (RX) ở chế độ Input Floating.

- **Thông số ADC**:

  - Độ phân giải: 12-bit (0-4095)

  - Tham chiếu điện áp: 3.3V (3300 mV)

  - Thời gian lấy mẫu: 55.5 cycles

  - Chế độ: Single Conversion (không dùng ngắt)

- **Thông số UART**:

  - Baudrate: 9600 bps

  - Word length: 8 bits

  - Stop bit: 1

  - Parity: None

- **Phương thức đọc ADC**: Polling (không dùng ngắt)
## Phần cứng kết nối
- **STM32F103C8T6** (Blue Pill)
- **CP2102 (USB–UART)** hoặc module UART tương đương
- **LDR (Light Dependent Resistor)** 
- **Kết nối UART**:
  - PA9 (TX) → RX module UART
  - PA10 (RX) → TX module UART  
  - GND STM32 ↔ GND UART
- **Kết nối LDR**:
  - VCC → 3.3V trên STM32 
  - GND → GND của STM32.
  - A0 → PA1 (chân ADC1 kênh 1 trên STM32).




## Ứng dụng thực tế
- Đo cường độ ánh sáng môi trường
- Hệ thống tự động bật/tắt đèn
- Cảm biến ánh sáng cho nhà thông minh
- Điều khiển rèm cửa tự động


![alt text](ex8.jpg)  
---

**Source code:** [Bài 8](EX8_lib.c)  

**Video Demo:** [Demo EX8](https://drive.google.com/file/d/1fHd_4chSRnwTCXccJLD6f4aPwpCooSU5/view?usp=drive_link) 