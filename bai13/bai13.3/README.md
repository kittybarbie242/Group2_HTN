# **Bài 3: Hai Task Gửi Dữ Liệu UART Có Đồng Bộ Mutex Trong FREERTOS**

---

## **1. Yêu Cầu**

* Viết chương trình có 2 Task gửi dữ liệu ra UART.
* Mỗi Task gửi một chuỗi khác nhau, ví dụ:
  * Task1 gửi: `"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"`
  * Task2 gửi: `"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBB"`
* Sử dụng **Mutex** để tránh xung đột khi hai Task cùng truy cập UART.
* Dữ liệu được hiển thị trên **Terminal** qua UART.

---

## **2. Ý Tưởng**

* Tạo **2 Task** chạy song song trong FreeRTOS.
* Cả hai Task cùng muốn truy cập tài nguyên **UART** để gửi chuỗi.
* Nếu không có cơ chế bảo vệ, dữ liệu có thể bị xen kẽ, hiển thị sai.
* Giải pháp: sử dụng **xSemaphoreCreateMutex()** để tạo **mutex**,
  và dùng **xSemaphoreTake() / xSemaphoreGive()** để đảm bảo chỉ một Task được phép sử dụng UART tại một thời điểm.

---

## **3. Cấu Hình Phần Cứng**

| Thành phần | Chân STM32F103 | Ghi chú                     |
| ---------- | -------------- | --------------------------- |
| UART1 TX   | PA9            | Truyền dữ liệu ra máy tính  |
| UART1 RX   | PA10           | Nhận dữ liệu (nếu cần)      |

---

## **4. Cấu Hình Phần Mềm**

* **IDE**: Keil uVision
* **FreeRTOS**: Được tích hợp sẵn trong project
* **Thư viện**: `stm32f10x.h`, `FreeRTOS.h`, `task.h`, `semphr.h`, `stm32f10x_usart.h`

---

## **5. Mô Tả Chương Trình**

### **Hàm `main()`**

* Cấu hình UART1 để truyền dữ liệu.
* Tạo một **mutex** bằng `xSemaphoreCreateMutex()`.
* Tạo 2 task (`Task1`, `Task2`) có cùng độ ưu tiên.
* Bắt đầu đặt lịch bằng `vTaskStartScheduler()`.

### **Task1 và Task2**

* Mỗi Task liên tục:
  * Chờ quyền truy cập UART (`xSemaphoreTake`).
  * Gửi chuỗi dữ liệu tương ứng (`USART_SendString()`).
  * Trả lại quyền truy cập (`xSemaphoreGive`).
  * Delay 100ms.

### **Cơ chế Mutex**

* Đảm bảo **chỉ một Task được phép gửi UART tại một thời điểm**.
* Khi một Task đang giữ Mutex, Task kia phải chờ đến khi được nhả Mutex mới được gửi.
* Tránh khỏi việc chồng dữ liệu giữa hai Task.

---

## **6. Kết Quả**

* Dữ liệu UART trên terminal hiển thị tuần tự, không bị xen kẽ:

  ```
  AAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
  BBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
  AAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
  BBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
  ...
  ```
* Chứng minh cơ chế Mutex hoạt động đúng, giúp bảo vệ tài nguyên chia sẻ.

---

## **7. Kết Luận**

* Minh họa được rõ **cơ chế đồng bộ Mutex trong FreeRTOS**.
* Khi hai Task cùng truy cập một tài nguyên, việc dùng Mutex giúp đảm bảo dữ liệu chính xác và tránh xung đột.
* Là nguyên tắc cơ bản trong lập trình **đa nhiệm và đa luồng**.

---

**Hình ảnh hiển thị:**

![alt text](image2.jpg)

**Video Demo:** [Link](https://drive.google.com/drive/folders/19U7jRkqjUVH6kjL8KimZb8kITfMR30vp?usp=sharing)




