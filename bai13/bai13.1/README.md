# **BÀI 1: SỬ DỤNG NGẮT NGOÀI VÀ TASK TRONG FREERTOS**

---

## **1. Yêu Cầu**

* Viết chương trình có **2 Task**:

  * **Task 1:** Nhấp nháy LED1.
  * **Task 2:** Bật LED2 khi có **ngắt ngoài (PA0)**.
* Khi chưa nhấn nút, **Task 2 ở trạng thái Block** (chờ semaphore).
* Khi có ngắt, Task 2 được **đánh thức ngay sau khi hàm xử lý ngắt (ISR)** thực thi.
* Sử dụng **FreeRTOS** để quản lý các task.

---

## **2. Ý Tưởng**

* Sử dụng **Semaphore nhị phân (Binary Semaphore)** để đồng bộ giữa **ISR** và **Task**.
* Task 1 chạy độc lập, chỉ điều khiển LED1 nhấp nháy định kỳ.
* Task 2 bị Block do `xSemaphoreTake()`. Khi ISR xảy ra, `xSemaphoreGiveFromISR()` đánh thức Task 2 để bật LED2 trong 3 giây.
* Cả hai Task hoạt động song song và được **FreeRTOS Scheduler** quản lý.

---

## **3. Cấu hình phần cứng và phần mềm**

| Thành phần | Chức năng                        | Chân kết nối |
| ---------- | -------------------------------- | ------------ |
| LED1       | Nhấp nháy liên tục               | PC13         |
| LED2       | Bật khi có ngắt                  | PA5         |
| Nút nhấn   | Tạo ngắt ngoài                   | PA0          |
| MCU        | STM32F103C8T6 (Blue Pill)        |              |
| FreeRTOS   | Quản lý đa nhiệm và đồng bộ Task |              |

**Công cụ:** Keil uVision 5 + Ngôn ngữ **C**.

---

## **4. Cấu hình chương trình**

### **4.1. Cấu hình GPIO và Ngắt ngoài**

* **PA0** được cấu hình là ngõ vào có điện trở kéo lên (Input Pull-up).
* Kích hoạt **ngắt ngoài EXTI0** với sườn **Falling Edge** (nhấn nút kéo chân xuống 0).
* Trong ISR `EXTI0_IRQHandler()`:

  * Gọi `xSemaphoreGiveFromISR()` để gửi tín hiệu cho Task 2.
  * Xóa cờ ngắt bằng `EXTI_ClearITPendingBit()`.

### **4.2. Cấu hình Task**

* **TaskBlink:** Nháy LED1 mỗi 500ms.
* **TaskWarning:** Bật LED2 khi có semaphore từ ngắt ngoài, giữ trong 3 giây rồi tắt.
---

## **5. Kết Quả**

* Khi **chưa nhấn nút**, LED1 (PC13) nhấp nháy đều đặn mỗi 0.5 giây, LED2 tắt.
* Khi **nhấn nút (PA0)**, LED2 bật sáng trong 3 giây rồi tắt.
* TaskWarning được **đánh thức ngay sau ISR**, chứng minh sự phối hợp giữa **ngắt – semaphore – task** hoạt động chính xác.

---

## **6. Nhận xét**

* Việc dùng **Binary Semaphore** giúp đồng bộ hiệu quả giữa **ngắt và task**.
* TaskWarning không chiếm CPU khi chờ, vì ở trạng thái **Block**.
* Cấu hình ưu tiên phù hợp giúp chương trình phản ứng nhanh, không bị trễ.
* Chương trình thể hiện rõ khả năng **đa nhiệm và đồng bộ ISR – Task trong FreeRTOS**.
* Là nền tảng để mở rộng sang các ứng dụng phức tạp hơn như: **báo động, xử lý sự kiện ngoại vi, hoặc quản lý nhiều cảm biến.**
