# Bài tập FreeRTOS: Giao tiếp giữa 2 Task sử dụng Queue (Producer-Consumer)

Đây là một dự án ví dụ trên vi điều khiển STM32F103, sử dụng hệ điều hành thời gian thực **FreeRTOS** để minh họa một trong những mẫu thiết kế phổ biến nhất: **Producer-Consumer** (Nhà sản xuất - Người tiêu dùng) thông qua cơ chế **Queue**.

---

## 🎯 Mục tiêu bài học

-   **Nắm vững khái niệm Queue:** Hiểu rõ cách Queue hoạt động như một cơ chế giao tiếp liên tác vụ (Inter-Task Communication - ITC) an toàn, giúp truyền dữ liệu mà không gây ra xung đột (race condition).
-   **Thực hành mô hình Producer-Consumer:** Triển khai một tác vụ chuyên tạo ra dữ liệu (`Producer`) và một tác vụ khác chuyên xử lý dữ liệu đó (`Consumer`), giúp tách biệt logic và tăng tính module hóa của hệ thống.
-   **Lập trình đa nhiệm (Multitasking):** Củng cố kỹ năng tạo, quản lý và cho các tác vụ chạy song song trên một lõi vi xử lý duy nhất.
-   **Sử dụng API của FreeRTOS:** Làm quen và vận dụng các hàm API cốt lõi liên quan đến Task và Queue như: `xTaskCreate()`, `xQueueCreate()`, `xQueueSend()`, `xQueueReceive()`, `vTaskDelay()`, và `vTaskStartScheduler()`.

---

## 📝 Mô tả chi tiết hoạt động

Chương trình triển khai hai tác vụ riêng biệt để điều khiển đèn LED nối với chân **PC13**. Hai tác vụ này không giao tiếp trực tiếp mà thông qua một "hộp thư" trung gian là `xLedConfigQueue`.

### 1. Task Producer (`vDataGenerateTask`) - Người tạo dữ liệu

Tác vụ này đóng vai trò là "Nhà sản xuất". Nhiệm vụ duy nhất của nó là định kỳ tạo ra dữ liệu cấu hình mới cho đèn LED và gửi đi.

-   **Chu kỳ hoạt động:** Tác vụ sẽ tạm dừng (`block`) trong **2 giây** bằng hàm `vTaskDelay(pdMS_TO_TICKS(2000))`. Việc này giúp tiết kiệm tài nguyên CPU thay vì dùng vòng lặp chờ bận (busy-waiting).
-   **Tạo dữ liệu:** Sau mỗi 2 giây, nó sẽ tạo ra hai giá trị ngẫu nhiên:
    -   `frequency` (tần số nháy): Một số nguyên ngẫu nhiên từ 1 đến 10 (Hz).
    -   `dutyCycle` (chu kỳ hoạt động): Một số nguyên ngẫu nhiên từ 10 đến 90 (%).
-   **Gửi dữ liệu:** Các giá trị trên được đóng gói vào một struct `LedConfig_t`. Sau đó, nó sử dụng hàm `xQueueSend(xLedConfigQueue, &newConfig, 0)` để gửi con trỏ của struct này vào Queue. Tham số cuối cùng là `0` (ticks to wait) có nghĩa là nếu Queue đầy, tác vụ sẽ không chờ mà bỏ qua việc gửi và tiếp tục chu kỳ tiếp theo.

### 2. Task Consumer (`vLedBlinkTask`) - Người xử lý dữ liệu

Tác vụ này đóng vai trò là "Người tiêu dùng". Nó liên tục chờ nhận dữ liệu từ Queue và hành động dựa trên dữ liệu đó.

-   **Chờ nhận dữ liệu:** Tác vụ gọi hàm `xQueueReceive(xLedConfigQueue, &currentConfig, pdMS_TO_TICKS(10))`. Lệnh này sẽ:
    -   Kiểm tra xem có dữ liệu nào trong Queue không.
    -   Nếu có, nó sao chép dữ liệu vào biến `currentConfig` và thực thi ngay lập tức.
    -   Nếu không, tác vụ sẽ bị `block` (tạm dừng) trong tối đa **10 mili giây** để chờ dữ liệu mới. Việc block này giúp CPU được giải phóng để làm việc khác. Sau 10ms nếu vẫn không có gì, nó sẽ chạy tiếp với cấu hình cũ.
-   **Xử lý dữ liệu:** Khi nhận được cấu hình mới (hoặc dùng lại cấu hình cũ), tác vụ sẽ:
    -   Tính toán `period_ms` (chu kỳ) từ `frequency`.
    -   Tính toán `ton_ms` (thời gian LED sáng) và `toff_ms` (thời gian LED tắt) dựa trên `dutyCycle`.
    -   Thực hiện bật LED, `vTaskDelay` một khoảng `ton_ms`, sau đó tắt LED và `vTaskDelay` một khoảng `toff_ms`. Vòng lặp này tạo ra hiệu ứng nhấp nháy.

### 3. Queue (`xLedConfigQueue`) - Kênh giao tiếp

-   **Khởi tạo:** Queue được tạo ra trong `main()` bằng `xQueueCreate(5, sizeof(LedConfig_t))`, cho phép nó chứa tối đa **5** cấu trúc `LedConfig_t`.
-   **Vai trò:** Nó là một bộ đệm FIFO (First-In, First-Out) đảm bảo an toàn luồng (thread-safe). Nhờ có Queue, `vDataGenerateTask` và `vLedBlinkTask` có thể hoạt động ở hai tốc độ khác nhau mà không làm mất dữ liệu và không cần các cơ chế khóa (mutex) phức tạp.

---

## 🔧 Yêu cầu & Cài đặt

### Phần cứng
* Board phát triển **STM32F103C8T6** (Blue Pill).
* Mạch nạp **ST-Link V2**.
* Một đèn LED và một điện trở (khoảng 220-330 Ohm).

### Phần mềm
* Môi trường lập trình: **Keil C for ARM**.
* Thư viện **STM32F1 Standard Peripherals Library (SPL)**.
* Thư viện **FreeRTOS**.

---

## 📊 Sơ đồ hoạt động

```mermaid
graph TD
    subgraph "Hàm main()"
        A[Bắt đầu] --> B[GPIO_Init_SPL()]
        B --> C[xQueueCreate()]
        C --> D[xTaskCreate(vLedBlinkTask)]
        C --> E[xTaskCreate(vDataGenerateTask)]
        E --> F[vTaskStartScheduler()]
    end

    subgraph "Task 1: vLedBlinkTask (Consumer)"
        style F2 fill:#f9f,stroke:#333,stroke-width:2px
        D -- Chạy song song --> F1
        F1(Vòng lặp vô tận) --> F2["xQueueReceive(Queue, &cfg, 10ms)"]
        F2 -- "Có dữ liệu mới" --> F3[Tính toán ton/toff từ cfg]
        F2 -- "Hết thời gian chờ" --> F4[Giữ nguyên ton/toff cũ]
        F3 --> F5[LED_ON()]
        F4 --> F5
        F5 --> F6["vTaskDelay(ton_ms)"]
        F6 --> F7[LED_OFF()]
        F7 --> F8["vTaskDelay(toff_ms)"]
        F8 --> F1
    end

    subgraph "Task 2: vDataGenerateTask (Producer)"
        style E3 fill:#ccf,stroke:#333,stroke-width:2px
        E -- Chạy song song --> E1
        E1(Vòng lặp vô tận) --> E2["vTaskDelay(2000ms)"]
        E2 --> E3[Tạo freq & duty ngẫu nhiên]
        E3 --> E4["xQueueSend(Queue, &data, 0)"]
        E4 --> E1
    end

    E4 -- "Gửi dữ liệu" --> C
    C -- "Lấy dữ liệu" --> F2
