# Bài tập 3: Chế độ STANDBY và đánh thức hệ thống bằng RTC Alarm

## 1. Yêu cầu
Xây dựng chương trình sử dụng chế độ **STANDBY** của vi điều khiển **STM32F103C8T6**, kết hợp với **bộ RTC** để hệ thống tự động **đánh thức sau một khoảng thời gian định sẵn**.  
Mục tiêu của bài là giúp hiểu cách vi điều khiển **ngủ sâu hoàn toàn**, chỉ được đánh thức bởi **nguồn ngắt RTC (Alarm)**.

## 2. Ý tưởng thực hiện
- Khi hệ thống khởi động, **UART** được cấu hình để gửi dữ liệu kiểm tra.  
- **RTC** được khởi tạo dùng nguồn xung **LSI (Low-Speed Internal)**, sau đó đặt **Alarm** sẽ kích hoạt sau **5 giây**.  
- Vi điều khiển được đưa vào **chế độ STANDBY** – đây là chế độ tiết kiệm năng lượng cao nhất, toàn bộ phần lớn ngoại vi bị tắt.  
- Khi đến thời điểm báo thức của RTC, tín hiệu ngắt từ **RTC Alarm (EXTI Line 17)** sẽ đánh thức vi điều khiển.  
- Sau khi thức dậy, hệ thống khởi tạo lại các ngoại vi cần thiết (USART, RTC, EXTI...) và tiếp tục thực thi chương trình.

## 3. Cấu hình chương trình
- **Vi điều khiển:** STM32F103C8T6 (Blue Pill)  
- **IDE:** Keil µVision 5  
- **Clock hệ thống:** 72 MHz  
- **Nguồn RTC:** LSI (40 kHz)  
- **Ngoại vi sử dụng:**  
  - USART1: Giao tiếp UART hiển thị chuỗi ký tự “123456” trước khi vào standby.  
  - RTC: Đặt báo thức sau 5 giây để đánh thức hệ thống.  
  - EXTI Line 17: Dẫn tín hiệu đánh thức từ RTC Alarm.  
- **Các thư viện:** `stm32f10x_usart.h`, `stm32f10x_rtc.h`, `stm32f10x_pwr.h`, `stm32f10x_exti.h`, `stm32f10x_bkp.h`.

## 4. Mô tả hoạt động chi tiết
1. **Khởi tạo hệ thống:**  
   - Cấu hình **UART1** để truyền dữ liệu ở tốc độ 9600 bps.  
   - Thiết lập **EXTI Line 17** để nhận tín hiệu ngắt từ RTC Alarm.  
   - Cấu hình **RTC** với nguồn LSI và đặt Prescaler = 40000 - 1 để tạo xung 1 Hz.  
2. **Kiểm tra trạng thái sau khi khởi động:**  
   - Nếu hệ thống được khởi động lại sau chế độ Standby → Xóa cờ Standby, RTC và EXTI được đồng bộ lại.  
   - Nếu là lần khởi động đầu tiên → Thiết lập RTC và báo thức.  
3. **Vòng hoạt động chính:**  
   - Gửi chuỗi “123456” qua UART.  
   - Cài đặt **Alarm** sau 5 giây (`RTC_SetAlarm(RTC_GetCounter() + 5)`).  
   - Sau khi chờ ngắn (Delay), gọi `PWR_EnterSTANDBYMode()` → hệ thống ngủ sâu.  
4. **Đánh thức hệ thống:**  
   - Sau 5 giây, RTC Alarm kích hoạt → gửi tín hiệu qua EXTI → MCU tự động khởi động lại từ đầu.  
   - Chuỗi “123456” được gửi lại qua UART, chứng minh hệ thống đã **thức dậy thành công**.

## 5. Kết quả đạt được
- Khi chạy chương trình, UART sẽ gửi “123456” mỗi **5 giây** (tương ứng với mỗi lần MCU thức dậy).  
- Giữa các lần truyền, MCU **hoàn toàn ngủ trong Standby Mode**, tiết kiệm năng lượng tối đa.  
- RTC hoạt động độc lập với CPU, đảm bảo thời gian báo thức chính xác.  
- Hệ thống có thể dễ dàng mở rộng để dùng báo thức đánh thức cho các tác vụ định kỳ hoặc đo đạc cảm biến theo chu kỳ.

## 6. Nhận xét
- Bài tập giúp làm quen với **chế độ năng lượng thấp (Low-Power Modes)** trên STM32, đặc biệt là **Standby Mode** – nơi MCU gần như tắt hoàn toàn.  
- Việc kết hợp **RTC Alarm** và **EXTI Wakeup** là nền tảng quan trọng cho các ứng dụng **IoT chạy pin**, nơi yêu cầu tiêu thụ điện năng cực thấp.  
- Qua bài này, người học nắm vững quy trình: **Khởi tạo RTC → Cấu hình Alarm → Đưa MCU vào Standby → Đánh thức bằng ngắt RTC.**

---


