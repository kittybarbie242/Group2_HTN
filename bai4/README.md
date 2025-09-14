### Sử dụng Timer để tạo Delay và điều khiển LED bằng ngắt Timer  

#### Yêu cầu 1: Cấu hình Timer để tạo Delay có độ chính xác cao  

**Mục đích:**  
- Làm quen với việc cấu hình Timer ở chế độ cơ bản.  
- Hiểu được cách dùng Timer như một bộ đếm để tạo độ trễ chính xác thay vì dùng vòng lặp bận.  

**Ý tưởng:**  
- Cấu hình một Timer chạy ở tần số 1 MHz (mỗi tick = 1 µs).  
- Dựa vào giá trị bộ đếm (`Counter`), xây dựng hàm `Delay_us()` để tạo trễ micro giây, và từ đó xây dựng `Delay_ms()`.  
- Qua đó, delay tạo ra sẽ ổn định và chính xác hơn so với cách dùng vòng lặp for thông thường.  

**Cấu hình:**  
- **Clock**: bật xung cho TIM2.  
- **Timer2**:  
  - Prescaler đặt để Timer đếm với tần số 1 MHz.  
  - Period đặt giá trị tối đa (`0xFFFF`) để có đủ khoảng đếm.  
- **Hàm Delay**:  
  - `Delay_us()` → reset bộ đếm về 0, chờ đến khi Counter đạt giá trị mong muốn.  
  - `Delay_ms()` → gọi lặp lại `Delay_us(1000)` theo số mili giây cần chờ.
  -  
**Source code:** [4.1](bai4.1/main.c)

#### Yêu cầu 2: Cấu hình Timer để tạo ngắt định kỳ và điều khiển LED  

**Mục đích:**  
- Làm quen với việc sử dụng Timer để sinh ngắt định kỳ.  
- Hiểu được cơ chế xử lý trong hàm ngắt (ISR).  
- Biết cách kết hợp Timer và GPIO để tạo LED nhấp nháy tự động.  

**Ý tưởng:**  
- Cấu hình Timer để tạo ra ngắt mỗi 500ms.  
- Trong ISR của Timer, đảo trạng thái LED.  
- Vì LED đổi trạng thái sau mỗi 500ms → kết quả LED sẽ nhấp nháy với chu kỳ 1000ms (tần số 1Hz).  

**Cấu hình:**  
- **Clock**: bật xung cho GPIOA và TIM3.  
- **GPIO**: PA5 cấu hình ở chế độ Output Push-Pull để điều khiển LED.  
- **Timer3**:  
  - Prescaler = 7199, Period = 4999 → tạo ra ngắt mỗi 500ms (với thạch anh 72 MHz).  
  - Kích hoạt ngắt `TIM_IT_Update`.  
- **NVIC**: bật kênh `TIM3_IRQn`, ưu tiên tiền xử lý và xử lý con đều bằng 0.  
- **ISR (TIM3_IRQHandler)**:  
  - Xóa cờ ngắt.  
  - Đảo trạng thái LED.  
- **Kết quả phần cứng:**  
  - LED (LD2) → PA5  
  - LED nhấp nháy với chu kỳ 1000ms (1Hz).  

![alt text](Ex4.jpg)  

**Source code:** [4.2](bai4.2/main.c)  

**Video Demo:** [DEMO](https://drive.google.com/drive/u/0/folders/18WuSejkMz8G0gB_w4a7SlnFkfVLqn4w3)


