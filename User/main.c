#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "DHT11.h"
#include "USART.h"
#include "ESP8266.h"
#include "onenet.h"
#include <string.h>
#include "BUZZER.h"
#include "LED.h"
#include "display.h"

#define ESP8266_ONENET_INFO "AT+CIPSTART=\"TCP\",\"mqtts.heclouds.com\",1883\r\n"

uint8_t Temp ;
uint8_t humidity ;

void Main_init(void){
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断控制分组
  OLED_Init();
	Buzzer_Init();
	DisplayTask();
	Buzzer_StartBeep();
	Led_Init();
  Serial_Init();
	ESP8266_Init();//初始化esp8266  
		UsartPrintf(USART1,"Connect MQTTs Server start\r\n");
		while (ESP8266_SendCmd(ESP8266_ONENET_INFO,"CONNECT"))
		{
				 Delay_ms(100);
		}
		UsartPrintf(USART1,"Connect MQTTs Server success\r\n");
		while(OneNet_DevLink() != 0)
		{
				UsartPrintf(USART1, "网络登陆失败\r\n");
				Delay_ms(100);
		}
    // 跳出上面循环，登录成功
		OLED_ShowString(2, 6, "OK");
    UsartPrintf(USART_DEBUG, "111111111111\r\n");
    Delay_ms(500);       //给底层硬件500ms的喘息和清空时间
    ESP8266_Clear();     //极其重要：强行把刚才接收过·+IPD·的串口接收缓存全部清零！

    OneNET_Subscribe();	
		while(DHT11_Init()){
			UsartPrintf(USART1,"DHT11 Error\r\n");
			Delay_ms(50);
     }
	 }

int main(void)
{
    // 初始化外设
	 Main_init(); 
	 unsigned short timeCount = 0;	//发送间隔变量
	 unsigned char *dataPtr = NULL;	
    while (1)
    {
			DHT11_Read_Data(&Temp ,&humidity);
      UsartPrintf(USART1,"Temp %d ,humidity %d \r\n",Temp,humidity);
			Delay_ms(50);
			if(timeCount++>=100){
				UsartPrintf(USART1,"OneNet_SendData\r\n");
				// 1. 先把灯点亮！此时 led_info.Led_Status 变成了 1 (true)
        Led_Set(LED_ON);
				Delay_ms(100);
				OneNet_SendData();//发送数据
				timeCount =0;
				ESP8266_Clear();
				// 3. 延时100毫秒后把灯灭掉，为下一次保持熄灭状态做准备
        Delay_ms(100);
        Led_Set(LED_OFF);
			}
			dataPtr =ESP8266_GetIPD(100);//接收数据
			if(dataPtr != NULL){
				OneNet_RevPro(dataPtr);		
			}
			 Buzzer_DataBeep();
			 Delay_ms(50);
			//显示温湿度
			// 调用DHT11读取数据（正确写法：传变量地址）
			OLED_ShowCHinese(1, 1, 1);  // 温
			OLED_ShowCHinese(1, 3, 2);  // 度
			OLED_ShowCHinese(2, 1, 4);  // 湿
			OLED_ShowCHinese(2, 3, 5);  // 度
			OLED_ShowNum(1, 6, Temp, 2);    // 显示温度整数
			OLED_ShowString(1, 8, "C");
			OLED_ShowNum(2, 6, humidity, 2);    // 显示湿度整数
			OLED_ShowString(2, 8, "%");
		}
	}


//int main(void)
//{
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//    Serial_Init();  // 现在这个函数会同时初始化USART1和USART2
//    
//    
//    UsartPrintf(USART1, "ESP8266 Test Start\r\n");
//    
//    while(1)
//    {
//        Usart_SendString(USART2, (unsigned char*)"AT\r\n", 4);
//        UsartPrintf(USART1, "Sent: AT\r\n");
//        
//        Delay_ms(1000);
//        
//        if(esp8266_cnt > 0)
//        {
//            UsartPrintf(USART1, "Received: %s\r\n", esp8266_buf);
//            esp8266_cnt = 0;
//            memset(esp8266_buf, 0, sizeof(esp8266_buf));
//        }
//        
//        UsartPrintf(USART1, "------------------------\r\n");
//    }
//}
