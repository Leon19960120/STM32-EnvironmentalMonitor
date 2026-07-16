#include "stm32f10x.h"                  // Device header
#include "USART.h"
#include "Delay.h"
//c库
#include <string.h>
#include <stdio.h>

#define REV_OK 0
#define REV_WAIT 1
//#define ESP8266_WIFI_INFO "AT+CWJAP=\"iPhone\",\"huangyuelai\"\r\n"
// 用 16 进制转义彻底解决 Keil 中文编译乱码问题
//#define ESP8266_WIFI_INFO "AT+CWJAP=\"\xE4\xBB\x93\xE5\x89\x8D\xE8\xA1\x97\xE9\x81\x93\xE4\xBA\xBA\xE5\xA4\xA7\xE8\x81\x94\xE7\xBB\x9C\xE7\xAB\x99\",\"cqjd123456\"\r\n"

#define ESP8266_WIFI_INFO "AT+CWJAP=\"HMWL\",\"hmwl123456qq\"\r\n"

unsigned  char esp8266_buf[512];
unsigned short esp8266_cnt =0 ;
unsigned short esp8266_cntPre =0 ;


//ESP8266_Clear
void ESP8266_Clear(void)
{
	memset(esp8266_buf,0,sizeof(esp8266_buf));
    esp8266_cnt =0;
	  esp8266_cntPre = 0;   // 增加这一行
}

//等待接收完成，rev_ok 接收完成， rev_wait 接收超时未完成
//_Bool ESP8266_WaitRev(void)
//{
//    if(esp8266_cnt ==0)
//    {
//        return REV_WAIT;
//    }
//    if(esp8266_cnt == esp8266_cntPre)
//    {
//        esp8266_cnt=0;
//        return REV_OK;//返回接收完成标志
//    
//    }
//    esp8266_cntPre = esp8266_cnt;
//		
//    return REV_WAIT;
//}
//  正确的WaitRev函数（绝对不要在里面清零cnt！）
_Bool ESP8266_WaitRev(void)
{
    static unsigned char stable_count = 0;
    
    if(esp8266_cnt == 0)
    {
        stable_count = 0;
        return REV_WAIT;
    }
    
    // 连续3次检测到数据长度不变，才认为接收完成
    if(esp8266_cnt == esp8266_cntPre)
    {
        stable_count++;
        if(stable_count >= 1)
        {
            stable_count = 0;
            return REV_OK;
        }
    }
    else
    {
        stable_count = 0;
        esp8266_cntPre = esp8266_cnt;
    }
    
    return REV_WAIT;
}


	_Bool ESP8266_SendCmd(const char *cmd, char* res)
{
    unsigned char timeout = 200;
    Usart_SendString(USART2, (unsigned char*)cmd, strlen((const char*)cmd));
    
    while (timeout--) {
        if (ESP8266_WaitRev() == REV_OK) {
            if (strstr((const char*)esp8266_buf, res) != NULL) {
                ESP8266_Clear();
                return 0;  // 成功
            }
        }
        Delay_ms(10);  // 延时在循环内
    }
    return 1;  // 失败
}



/**
 * @brief  ESP8266 发送数据函数
 * @param  data: 要发送的数据缓冲区指针
 * @param  len:  数据长度
 * @return _Bool: 1-发送成功, 0-发送失败
 */
_Bool ESP8266_SendData(unsigned char *data, unsigned short len)
{
    char cmdbuf[128];
    sprintf(cmdbuf, "AT+CIPSEND=%d\r\n", len);
      
    if (!ESP8266_SendCmd(cmdbuf, ">")){
    //{
        Usart_SendString(USART2, data, len);//发送设备连接请求数据
////        Delay_ms(50);
////        if (ESP8266_SendCmd("", "SEND OK") == 0)
////        {
////            UsartPrintf(USART1, "TCP send OK\r\n");
////            return 0; // ?? 成功
////        }
////    //}
////     //?? 只要上面任何一步漏了，统一判为失败
////    return 1; 
}
}


//==========================================================
//	函数名称：	ESP8266_GetIPD
//
//	函数功能：	获取平台返回的数据
//
//	入口参数：	等待的时间(乘以10ms)
//
//	返回参数：	平台返回的原始数据
//
//	说明：		不同网络设备返回的格式不同，需要去调试
//				如ESP8266的返回格式为	"+IPD,x:yyy"	x代表数据长度，yyy是数据内容
//==========================================================
//获取平台返回的数据（原始数据），timeout 等待的时间（x10ms）
// 获取平台返回数据
unsigned char*  ESP8266_GetIPD(unsigned short timeout){
	// 新增：打印所有收到的原始数据
//    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
//    {
//        uint8_t c = USART_ReceiveData(USART2);
//        UsartPrintf(USART1, "收到字节: 0x%02X\r\n", c);
//    }
    char* ptrIPD = NULL;
    do {
        if (ESP8266_WaitRev() == REV_OK){
            ptrIPD = strstr((const char*)esp8266_buf, "IPD,");
            if (ptrIPD == NULL)
            {
                UsartPrintf(USART_DEBUG, "No IPD found in buffer\r\n");
							  ESP8266_Clear(); //  ??【核心修复】找不到IPD，必须立刻清空缓冲区，防止下次进来死循环误判！
            }
            else{
                ptrIPD = strchr(ptrIPD, ':');
                if (ptrIPD != NULL){
                    ptrIPD ++;
                    return (unsigned char*)ptrIPD;
                }else
								ESP8266_Clear(); // ?? 格式不对也清空
                return NULL;  
            }
        }
        Delay_ms(5);
    } while (timeout--);
    return NULL;//超时未找到IPD
    }

		
		
		
void ESP8266_Init(void)
{
 
    ESP8266_Clear();
	  // 关键：上电后等待2秒，让ESP8266完全启动
    UsartPrintf(USART1, "1.AT\r\n");
    while(ESP8266_SendCmd("AT\r\n", "OK"))
			{
    Delay_ms(500);
    }
		// 3. 恢复出厂设置（清除旧配置）
    //    ESP8266_SendCmd("AT+RESTORE\r\n", "OK");
    //    Delay_ms(1000);  // 恢复出厂后需要重启等待
		
    UsartPrintf(USART1, "2.CWMODE\r\n");
    while(ESP8266_SendCmd("AT+CWMODE=1\r\n", "OK") )
			{
    Delay_ms(500);
    }
		
    UsartPrintf(USART1, "3.AT+CWDHCP\r\n");
    while(ESP8266_SendCmd("AT+CWDHCP=1,1\r\n", "OK") )
			{
    Delay_ms(500);
    }
    UsartPrintf(USART1, "4.AT+CWJAP\r\n");
    while(ESP8266_SendCmd(ESP8266_WIFI_INFO, "GOT IP") ){
    Delay_ms(2000);
    }
    UsartPrintf(USART1, "5.ESP8266_Init OK\r\n");
    
}
//void USART2_IRQHandler(void)
//{
//    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//接收中断
//    {
//			if(esp8266_cnt >= sizeof (esp8266_buf)) {//防止串口被刷爆
//				esp8266_cnt =0 ;
//			}
//      esp8266_buf[esp8266_cnt++] = USART2 ->DR;			//USART_ReceiveData(USART2)
//			//uint8_t data = USART_ReceiveData(USART2);
//      // 处理 USART2 收到的数据
//        USART_ClearFlag(USART2, USART_IT_RXNE);
//    }
//}


void USART2_IRQHandler(void)
{
    // ?? 核心防御：检测并清除溢出错误 (ORE) / 帧错误 (FE) 等
    // 必须在处理 RXNE 之前拦截，防止单片机死卡在中断里！
    if (USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET)
    {
        (void)USART2->SR; // 依据官方手册：先读 SR
        (void)USART2->DR; // 再读 DR，硬件会自动清除 ORE 标志位
        return;           // 清除后直接退出，防止脏数据干扰
    }

    // 正常的接收中断
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        // 读取 DR 会自动清除 RXNE 标志位，不需要手动 ClearFlag
        unsigned char res = USART2->DR; 
        
        // 安全控制：防止缓冲区越界（保留一个字节给 '\0'）
        if(esp8266_cnt < sizeof(esp8266_buf) - 1) 
        {
            esp8266_buf[esp8266_cnt++] = res;
        }
        // 如果满了就静默丢弃后续字节，绝对不要在中断里把 cnt 盲目清零，否则会破坏半截好数据
    }
}



