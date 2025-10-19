# FreeRTOS – Giao tiếp Queue giữa hai Task điều khiển LED

### Yêu cầu

Viết chương trình sử dụng **FreeRTOS** gồm 2 tác vụ chạy song song:

* **Task 1 (LedBlinkTask):** Nhận dữ liệu từ hàng đợi và điều khiển LED nhấp nháy theo tần số và độ rộng xung tương ứng.
* **Task 2 (ControlTask):** Gửi các giá trị tần số và độ rộng xung thay đổi định kỳ sang hàng đợi để điều khiển LED.

**Kết quả:** LED trên chân PC13 nhấp nháy với tần số và độ rộng xung thay đổi liên tục theo thời gian.

---

### Ý tưởng

* Sử dụng **Queue** để truyền dữ liệu giữa hai tác vụ trong FreeRTOS.
* Dữ liệu truyền đi là một cấu trúc `LedConfig` gồm:

  ```c
  typedef struct {
      uint32_t frequency;  // Tần số nhấp nháy (Hz)
      uint8_t dutyCycle;   // Độ rộng xung (%)
  }LedConfig;
  ```
* **ControlTask** định kỳ thay đổi giá trị frequency và dutyCycle, sau đó gửi vào hàng đợi.
* **LedBlinkTask** đọc dữ liệu từ hàng đợi, tính toán chu kỳ bật/tắt LED dựa vào frequency và dutyCycle.
* Nếu không có dữ liệu mới, LED sẽ tiếp tục nhấp nháy theo thông số cũ.
* Sử dụng `vTaskDelay()` để đảm bảo chu kỳ LED chính xác theo thời gian thực.

---

### Cấu hình phần mềm

* **IDE:** Keil µVision5
* **Thư viện sử dụng:**

  * FreeRTOS Kernel
  * STM32F10x Standard Peripheral Library
* **File cấu hình:**

  * `FreeRTOSConfig.h` – định nghĩa các thông số hệ thống như:

    * `configCPU_CLOCK_HZ`
    * `configTICK_RATE_HZ`
    * `configMINIMAL_STACK_SIZE`
    * `configMAX_PRIORITIES`

---

### Cấu hình phần cứng

* **Vi điều khiển:** STM32F103C8T6 (Blue Pill)
* **LED:** Nối tại chân **PC13**, LED sáng ở mức logic thấp.
* **Clock hệ thống:** 72 MHz

---

### Mô tả chương trình

#### 1. Khởi tạo GPIO

* Bật clock cho cổng GPIOC.
* Cấu hình chân PC13 ở chế độ **Output Push-Pull**, tốc độ 2 MHz.

#### 2. Khởi tạo Queue

* Tạo hàng đợi `LedConfigQueue` có thể chứa tối đa **5 phần tử** kiểu `LedConfig`.
* Mỗi phần tử lưu tần số nhấp nháy và độ rộng xung của LED.

#### 3. Tạo các Task

* **Task 1 – LedBlinkTask:**

  * Dùng `xQueueReceive()` để nhận dữ liệu từ hàng đợi.
  * Tính toán thời gian bật/tắt LED theo công thức:

    * `period = 1000 / frequency` (ms)
    * `onTime = period * dutyCycle / 100`
    * `offTime = period - onTime`
  * Điều khiển LED bật/tắt tương ứng và trễ bằng `vTaskDelay()` để giữ nhịp chính xác.
  * Nếu không có dữ liệu mới, tiếp tục dùng giá trị cũ để duy trì nháy LED.

* **Task 2 – ControlTask:**

  * Chu kỳ gửi dữ liệu: 1 giây/lần.
  * Thay đổi lần lượt các giá trị frequency và dutyCycle (ví dụ: 1Hz–20%, 2Hz–50%, 5Hz–80%).
  * Gửi cấu trúc `LedConfig` vào hàng đợi bằng `xQueueSend()`.

#### 4. Khởi động Scheduler

* Gọi `vTaskStartScheduler()` để bắt đầu hoạt động đa nhiệm của hệ thống.
* FreeRTOS tự động quản lý việc chia thời gian CPU giữa các Task.

---

### Ứng dụng thực tế

* Mô phỏng quá trình **giao tiếp dữ liệu giữa các tiến trình** trong hệ thống nhúng.
* Là ví dụ cơ bản giúp hiểu **cơ chế đồng bộ** bằng Queue trong FreeRTOS.
* Có thể mở rộng thành hệ thống điều khiển nhiều LED, động cơ, hoặc truyền dữ liệu giữa cảm biến – UART – hiển thị LCD.
* Cơ sở cho các bài học nâng cao về **multitasking**, **inter-task communication**, và **real-time control** trong hệ thống nhúng.

---
**Source code:** [Bài 12](main.c)  

**Video Demo:** [Demo](https://drive.google.com/file/d/1xVIidrJJnMDRVxv5WBiCA5UfqnloDLT2/view?usp=sharing)

