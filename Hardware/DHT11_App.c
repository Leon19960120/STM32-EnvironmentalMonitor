//#include "DHT11_App.h"
////#include "DTH11.h"
//#include "OLED.h"
//#include "Delay.h"

//void DHT11_DisplayTask(void)
//{
//    uint8_t buffer[5];  // 数据缓冲区

//    while (1)
//    {
//        if (DHT_Get_Temp_Humi_Data(buffer) == DHT_OK)
//        {
//            OLED_ShowCHinese(1, 1, 1); // 温
//					  OLED_ShowCHinese(1, 3, 2); // 度
//					  OLED_ShowCHinese(2, 1, 4); // 
//					  OLED_ShowCHinese(2, 3, 5); // 度
//					  OLED_ShowCHinese(3, 1, 6); // 
//					  OLED_ShowCHinese(3, 3, 7); // 
//            
//            
//            OLED_ShowNum(1, 6, buffer[2], 2);   // 温度
//					  OLED_ShowString(1, 8, "C");
//            OLED_ShowNum(2, 6, buffer[0], 2);   // 湿度
//            OLED_ShowString(2, 8, "% ");        //%
//					  OLED_ShowString(3, 6, "yes");        //%
//        }
//        else
//        {
//            OLED_ShowString(1, 6, "ERR");
//            OLED_ShowString(2, 6, "ERR");
//					  OLED_Clear();
//        }

//        Delay_ms(2000);
//    }
//}