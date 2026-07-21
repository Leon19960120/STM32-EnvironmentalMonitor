#include "stm32f10x.h"                  // Device header
#include "USART.h"
#include "Delay.h"
//c��
#include <string.h>
#include <stdio.h>

#define REV_OK 0
#define REV_WAIT 1


//�� 16 ����ת�峹�׽�� Keil ���ı�����������
//����wifi����ת��
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
	  esp8266_cntPre = 0;   // ������һ��
}

//�ȴ�������ɣ�rev_ok ������ɣ� rev_wait ���ճ�ʱδ���
//_Bool ESP8266_WaitRev(void)
//{
//    if(esp8266_cnt ==0)
//    {
//        return REV_WAIT;
//    }
//    if(esp8266_cnt == esp8266_cntPre)
//    {
//        esp8266_cnt=0;
//        return REV_OK;//���ؽ�����ɱ�־
//    
//    }
//    esp8266_cntPre = esp8266_cnt;
//		
//    return REV_WAIT;
//}
//  ��ȷ��WaitRev���������Բ�Ҫ����������cnt����
_Bool ESP8266_WaitRev(void)
{
    static unsigned char stable_count = 0;
    
    if(esp8266_cnt == 0)
    {
        stable_count = 0;
        return REV_WAIT;
    }
    
    // ����3�μ�⵽���ݳ��Ȳ��䣬����Ϊ�������
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
                return 0;  // �ɹ�
            }
        }
        Delay_ms(10);  // ��ʱ��ѭ����
    }
    return 1;  // ʧ��
}



/**
 * @brief  ESP8266 �������ݺ���
 * @param  data: Ҫ���͵����ݻ�����ָ��
 * @param  len:  ���ݳ���
 * @return _Bool: 1-���ͳɹ�, 0-����ʧ��
 */
_Bool ESP8266_SendData(unsigned char *data, unsigned short len)
{
    char cmdbuf[128];
    sprintf(cmdbuf, "AT+CIPSEND=%d\r\n", len);
      
    if (!ESP8266_SendCmd(cmdbuf, ">")){
    //{
        Usart_SendString(USART2, data, len);//�����豸������������
////        Delay_ms(50);
////        if (ESP8266_SendCmd("", "SEND OK") == 0)
////        {
////            UsartPrintf(USART1, "TCP send OK\r\n");
////            return 0; // ?? �ɹ�
////        }
////    //}
////     //?? ֻҪ�����κ�һ��©�ˣ�ͳһ��Ϊʧ��
////    return 1; 
}
}


//==========================================================
//	�������ƣ�	ESP8266_GetIPD
//
//	�������ܣ�	��ȡƽ̨���ص�����
//
//	��ڲ�����	�ȴ���ʱ��(����10ms)
//
//	���ز�����	ƽ̨���ص�ԭʼ����
//
//	˵����		��ͬ�����豸���صĸ�ʽ��ͬ����Ҫȥ����
//				��ESP8266�ķ��ظ�ʽΪ	"+IPD,x:yyy"	x�������ݳ��ȣ�yyy����������
//==========================================================
//��ȡƽ̨���ص����ݣ�ԭʼ���ݣ���timeout �ȴ���ʱ�䣨x10ms��
// ��ȡƽ̨��������
unsigned char*  ESP8266_GetIPD(unsigned short timeout){
	// ��������ӡ�����յ���ԭʼ����
//    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
//    {
//        uint8_t c = USART_ReceiveData(USART2);
//        UsartPrintf(USART1, "�յ��ֽ�: 0x%02X\r\n", c);
//    }
    char* ptrIPD = NULL;
    do {
        if (ESP8266_WaitRev() == REV_OK){
            ptrIPD = strstr((const char*)esp8266_buf, "IPD,");
            if (ptrIPD == NULL)
            {
                UsartPrintf(USART_DEBUG, "No IPD found in buffer\r\n");
							  ESP8266_Clear(); //  ??�������޸����Ҳ���IPD������������ջ���������ֹ�´ν�����ѭ�����У�
            }
            else{
                ptrIPD = strchr(ptrIPD, ':');
                if (ptrIPD != NULL){
                    ptrIPD ++;
                    return (unsigned char*)ptrIPD;
                }else
								ESP8266_Clear(); // ?? ��ʽ����Ҳ���
                return NULL;  
            }
        }
        Delay_ms(5);
    } while (timeout--);
    return NULL;//��ʱδ�ҵ�IPD
    }

		
		
		
void ESP8266_Init(void)
{
 
    ESP8266_Clear();
	  // �ؼ����ϵ��ȴ�2�룬��ESP8266��ȫ����
    UsartPrintf(USART1, "1.AT\r\n");
    while(ESP8266_SendCmd("AT\r\n", "OK"))
			{
    Delay_ms(500);
    }
		// 3. �ָ��������ã���������ã�
    //    ESP8266_SendCmd("AT+RESTORE\r\n", "OK");
    //    Delay_ms(1000);  // �ָ���������Ҫ�����ȴ�
		
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
//    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//�����ж�
//    {
//			if(esp8266_cnt >= sizeof (esp8266_buf)) {//��ֹ���ڱ�ˢ��
//				esp8266_cnt =0 ;
//			}
//      esp8266_buf[esp8266_cnt++] = USART2 ->DR;			//USART_ReceiveData(USART2)
//			//uint8_t data = USART_ReceiveData(USART2);
//      // ���� USART2 �յ�������
//        USART_ClearFlag(USART2, USART_IT_RXNE);
//    }
//}


void USART2_IRQHandler(void)
{
    // ?? ���ķ�������Ⲣ���������� (ORE) / ֡���� (FE) ��
    // �����ڴ��� RXNE ֮ǰ���أ���ֹ��Ƭ���������ж��
    if (USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET)
    {
        (void)USART2->SR; // ���ݹٷ��ֲ᣺�ȶ� SR
        (void)USART2->DR; // �ٶ� DR��Ӳ�����Զ���� ORE ��־λ
        return;           // �����ֱ���˳�����ֹ�����ݸ���
    }

    // �����Ľ����ж�
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        // ��ȡ DR ���Զ���� RXNE ��־λ������Ҫ�ֶ� ClearFlag
        unsigned char res = USART2->DR; 
        
        // ��ȫ���ƣ���ֹ������Խ�磨����һ���ֽڸ� '\0'��
        if(esp8266_cnt < sizeof(esp8266_buf) - 1) 
        {
            esp8266_buf[esp8266_cnt++] = res;
        }
        // ������˾;�Ĭ���������ֽڣ����Բ�Ҫ���ж���� cnt äĿ���㣬������ƻ���غ�����
    }
}



