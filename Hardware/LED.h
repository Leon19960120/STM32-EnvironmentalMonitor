#ifndef _LED_H
#define _LED_H

#include "stm32f10x.h"                  // Device header



typedef struct{
  _Bool Led_Status;
}LED_INFO;


#define LED_ON		1

#define LED_OFF	0

extern LED_INFO led_info;

void Led_Init(void);

void Led_Set(uint8_t status);  

#endif
