# Bài tập 2: Chế độ Delay và Sleep trong FreeRTOS

## 1. Yêu cầu
Viết chương trình điều khiển LED nhấp nháy bằng FreeRTOS trên vi điều khiển **STM32F103C8T6**.  
**Hai chương trình riêng biệt** nhằm so sánh sự khác nhau giữa chế độ **Delay thông thường** và **Sleep Mode** của hệ thống:

**Phần 1 – Delay:** LED nhấp nháy với chu kỳ 1 giây, sử dụng hàm `vTaskDelay()` để tạo độ trễ.  
**Phần 2 – Sleep Mode:** Hệ thống không có Task nào hoạt động, CPU tự động vào trạng thái **(Sleep)** nhờ cơ chế **Idle Hook** và lệnh `__WFI()`.

## 2. Ý tưởng thực hiện
- Tạo một **Task duy nhất** có nhiệm vụ điều khiển LED (Task_Blink).  
- Trong Task, LED được đảo trạng thái mỗi 1000 ms.  
- Khi Task tạm dừng bằng `vTaskDelay()`, hệ điều hành chuyển sang **Idle Task**.  
- Nếu bật **Idle Hook**, hệ thống sẽ gọi hàm `__WFI()` giúp CPU **tạm dừng hoạt động**, giảm tiêu thụ năng lượng cho đến khi có Task sẵn sàng chạy lại.  
- Hai phần chương trình dùng cùng một cấu trúc Task, chỉ khác cách xử lý ở chế độ Idle.

## 3. Cấu hình chương trình
- **Vi điều khiển:** STM32F103C8T6 (Blue Pill)  
- **IDE:** Keil µVision 5  
- **RTOS:** FreeRTOS   
- **Clock hệ thống:** 72 MHz  
- **Chân LED:** PC13 – LED   
- **Các file chính:**  
  - `main.c`: Cấu hình GPIO, tạo Task, cài đặt Idle Hook.  
  - `FreeRTOSConfig.h`: Kích hoạt Idle Hook bằng `#define configUSE_IDLE_HOOK 1`.  
  - `stm32f10x_gpio.c/h`, `stm32f10x_rcc.c/h`: Cấu hình phần cứng.  

## 4. Mô tả hoạt động
- **Khởi tạo hệ thống**: Bật xung clock cho GPIOC và cấu hình chân PC13 ở chế độ Output Push-Pull.  
- **Tạo Task_Blink**: LED nhấp nháy bằng cách đảo bit PC13, mỗi lần chờ 1000 ms bằng `vTaskDelay()`.  
- **Chế độ Delay:** CPU vẫn hoạt động bình thường trong suốt thời gian chờ của Task, gây hao năng lượng.  
- **Chế độ Sleep:** Khi Task_Blink đang delay, **Idle Task** được kích hoạt → gọi `__WFI()` → CPU tạm ngừng cho đến khi có Task cần chạy.  
- **Khi hết thời gian Delay**, CPU tự động thức dậy và LED đổi trạng thái, chu kỳ lặp lại liên tục.

## 5. Kết quả đạt được
- LED trên chân **PC13** nhấp nháy ổn định với chu kỳ 1 giây.  
- Ở **Delay Mode**, hệ thống hoạt động liên tục, dòng tiêu thụ điện cao hơn.  
- Ở **Sleep Mode**, CPU thường xuyên nghỉ giữa các chu kỳ, giúp tiết kiệm năng lượng đáng kể.  
- Chương trình chạy ổn định, không xảy ra lỗi treo hoặc mất đồng bộ giữa các Task.  
- Có thể dễ dàng mở rộng để điều khiển nhiều Task hoặc kích hoạt Sleep thông minh hơn.

## 6. Nhận xét
Bài tập giúp hiểu sâu hơn về **cơ chế lập lịch và quản lý năng lượng** trong FreeRTOS.  
Việc sử dụng **Idle Hook kết hợp lệnh `__WFI()`** là giải pháp hiệu quả cho các ứng dụng **nhúng tiết kiệm năng lượng**.  
Thấy rõ được sự khác biệt giữa **Delay (CPU vẫn chạy)** và **Sleep (CPU thực sự nghỉ)** — một yếu tố rất quan trọng khi thiết kế các hệ thống nhúng dùng pin.

---

**Video Demo:** [Link](https://drive.google.com/drive/folders/18AB3U_aaBF28kigNuzHOAKb5ejsTVPNG?usp=sharing)

