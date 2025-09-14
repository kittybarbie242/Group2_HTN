
### Cấu hình ngắt ngoài cho 1 nút nhấn. Khi nhấn nút, trạng thái của led sẽ đảo ngược. Trong quá trình đó, 1 led khác vẫn nhấp náy với chu kỳ 1HZ  
**Mục đích:**  
Bài thực hành này giúp người học làm quen với việc điều khiển LED và xử lý tín hiệu nút nhấn bằng vi điều khiển.  
Thông qua ví dụ, sinh viên sẽ:  
- Hiểu được cách thiết lập ngõ ra (Output) để điều khiển LED.  
- Biết cách sử dụng ngõ vào (Input) để đọc trạng thái nút nhấn.  
- Làm quen với cơ chế **ngắt ngoài (EXTI)**, hiểu cách hệ thống phản ứng ngay khi có sự kiện xảy ra.  
- Nắm được vai trò của **NVIC** trong việc quản lý và ưu tiên ngắt.  

**Ý tưởng:**  
- Sử dụng hai LED: một LED được lập trình để nhấp nháy liên tục theo chu kỳ định sẵn, đóng vai trò như tín hiệu kiểm tra hoạt động.  
- LED còn lại thay đổi trạng thái mỗi khi người dùng nhấn nút. Việc thay đổi này không dựa vào kiểm tra liên tục (polling) mà dựa trên **ngắt ngoài**, giúp chương trình phản hồi tức thời và hiệu quả hơn.  
- Kết hợp LED nhấp nháy theo chu kỳ và LED điều khiển bằng nút nhấn nhằm minh họa sự phối hợp giữa vòng lặp chính và cơ chế ngắt.  

**Cấu hình:**  
- Bật clock cho GPIOA, GPIOB và AFIO.  
- Thiết lập các chân GPIO ở chế độ Output (cho LED) và Input Pull-up (cho nút nhấn).  
- Ánh xạ chân nút nhấn vào EXTI line, kích hoạt ngắt ngoài theo sườn xuống.  
- Cấu hình NVIC để bật kênh ngắt và thiết lập mức ưu tiên.  
- Phần cứng kết nối:  
  - LED1 → PA5  
  - LED2 → PA6  
  - Button → PB0  
 

![alt text](bai3/img.jpg)  

**Source code:** [Bài 3](main.c)  

**Video Demo:** [DEMO](https://drive.google.com/drive/u/0/folders/18WuSejkMz8G0gB_w4a7SlnFkfVLqn4w3)



