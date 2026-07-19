#include "stm32f10x.h"
#include "OLED.h"
#include "Delay.h"




void DisplayTask(void){
	 // 第1步：显示“项目正在启动...”
    OLED_Clear();
    OLED_ShowString(1, 1, "Project Starting");
    OLED_ShowString(2, 1, "Please wait...");
    Delay_ms(1000);                     // 停留1秒
    // 第2步：模拟初始化过程（可以换成实际硬件初始化检测）
    // 此处假设初始化成功，直接显示成功信息
    OLED_Clear();
    OLED_ShowString(1, 1, "Initialization");
    OLED_ShowString(2, 1, "Successful!");
    OLED_ShowString(3, 1, "System Ready");
    Delay_ms(800);                      // 显示成功信息，保持0.8秒
    // 第3步：清屏，返回主循环调用（主循环会继续显示温湿度等）
    OLED_Clear();
 }