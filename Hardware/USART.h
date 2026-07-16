#ifndef __USART_H__
#define __USART_H__

#include "stm32f10x.h"                  // Device header

#define USART_DEBUG   USART1

void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);\
void Usart_SendString(USART_TypeDef * USARTx, unsigned char* str, unsigned short len);
void Serial_SendString(char *String);
uint32_t Serial_Pow(uint32_t X, uint32_t Y);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
void UsartPrintf(USART_TypeDef * USARTx,const char* fmt,...);

 #endif
