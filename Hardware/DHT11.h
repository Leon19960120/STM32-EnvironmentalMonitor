#include "stm32f10x.h"                  // Device header

#ifndef __DHT11_H
#define __DHT11_H


extern uint8_t Temp,humidity;

uint8_t DHT11_Init(void);
uint8_t DHT11_Read_Data (uint8_t* Temp,uint8_t* humidity);

//读取一个字节
uint8_t DHT11_Read_Byte(void);
//读出一个位
uint8_t DHT11_Read_Bit(void);
//检测是否存在DTH11
uint8_t DHT11_Check(void);

void DHT11_Rst(void);

#endif /* __DHT11_H */

