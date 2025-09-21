# Thí nghiệm: Giao tiếp SPI với ADXL345 và hiển thị dữ liệu qua UART

## 1. Mục đích
- Làm quen với giao tiếp **SPI** trên vi điều khiển STM32F103.  
- Kết nối và đọc dữ liệu từ cảm biến **ADXL345** qua giao thức SPI.  
- Kiểm tra hoạt động bằng cách đọc thanh ghi **DEVID (0x00)**, giá trị trả về mặc định là `0xE5`.  
- Gửi dữ liệu đọc được lên PC thông qua **UART1** để quan sát bằng terminal.  

---

## 2. Lý thuyết cơ bản

### 2.1. SPI (Serial Peripheral Interface)
- SPI là giao thức truyền thông nối tiếp đồng bộ, tốc độ cao.  
- Các thành phần chính:
  - **Master**: thiết bị điều khiển xung clock (ở đây là STM32F103).  
  - **Slave**: thiết bị ngoại vi (ở đây là ADXL345).  
- 4 tín hiệu quan trọng:
  - **SCK**: Serial Clock.  
  - **MOSI**: Master Out – Slave In.  
  - **MISO**: Master In – Slave Out.  
  - **CS (Chip Select)**: chọn slave cần giao tiếp.  
- Có 4 chế độ SPI (Mode 0 → 3) tùy thuộc vào CPOL và CPHA.  
- ADXL345 hoạt động ở **SPI mode 3 (CPOL=1, CPHA=1)**.  

### 2.2. UART (Universal Asynchronous Receiver/Transmitter)
- UART là chuẩn truyền thông nối tiếp **không đồng bộ**.  
- Thường được dùng để debug, giao tiếp với máy tính thông qua USB-UART.  
- Thông số cấu hình phổ biến:
  - Baudrate: 9600 bps.  
  - Data bits: 8.  
  - Stop bit: 1.  
  - No parity.  

### 2.3. Thanh ghi DEVID – ADXL345
- Địa chỉ: `0x00`.  
- Chỉ đọc (read-only).  
- Luôn chứa giá trị **0xE5**.  
- Mục đích: giúp người lập trình kiểm tra nhanh xem cảm biến đã được kết nối đúng chưa.  

---

## 3. Sơ đồ kết nối phần cứng

### 3.1. STM32F103C8T6 ↔ ADXL345 (SPI)
| STM32F103 | ADXL345 |
|-----------|---------|
| PA5 (SPI1_SCK)  | SCL/SCLK |
| PA6 (SPI1_MISO) | SDO      |
| PA7 (SPI1_MOSI) | SDA/SDI  |
| PA4 (GPIO – CS) | CS       |
| 3.3V             | VCC      |
| GND              | GND      |

### 3.2. UART Debug
- PA9 (TX) → USB-UART (TXD → RX của PC).  
- PA10 (RX) (không sử dụng trong demo).  
- Baudrate 9600, cấu hình terminal **8N1**.  

---

## 4. Phân tích chương trình

### 4.1. Cấu hình GPIO
- PA5, PA7: Output Alternate Function Push-Pull → SCK, MOSI.  
- PA6: Input Floating → MISO.  
- PA4: Output Push-Pull → CS (chip select).  

### 4.2. Cấu hình SPI1
- Master mode.  
- Data size = 8 bit.  
- CPOL = 1, CPHA = 1 (Mode 3).  
- Baudrate Prescaler = 16 (tốc độ ~4.5 MHz).  
- First bit = MSB.  

### 4.3. Cấu hình UART1
- Baudrate = 9600.  
- Data = 8 bit, stop = 1, parity = none.  
- Dùng để in dữ liệu ra màn hình terminal.  

### 4.4. Chu trình đọc DEVID
1. Kéo chân CS xuống LOW.  
2. Gửi byte lệnh `0x80 | 0x00 = 0x80`.  
   - Bit 7 = 1 (Read).  
   - Bit 6 = 0 (Single byte).  
   - Bit 5-0 = địa chỉ thanh ghi (0x00).  
3. Gửi thêm 1 byte `0x00` để tạo clock.  
   - Trong lúc này, ADXL345 sẽ trả về giá trị trong thanh ghi `0x00`.  
4. STM32 đọc dữ liệu trả về từ SPI (kết quả là `0xE5`).  
5. Nhả CS lên HIGH.  
6. In dữ liệu qua UART.  

---

## 5. Kết quả mong đợi
- Khi chạy chương trình, terminal sẽ hiển thị liên tục (mỗi 500 ms):  

