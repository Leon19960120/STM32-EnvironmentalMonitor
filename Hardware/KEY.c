#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
 uint16_t KeyNum =0 ;
//LED初始化
void KEY_Init(void){
	//使能APB2总线上的GPIOA时钟和AFIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	

	//定义一个GPIO初始化结构体，通常用于配置GPIO的各种参数
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;   //设置GPIO为上拉输入模式
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6;//设置要输出的GPIO引脚
	//GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;//输出速度为50Mhz
	GPIO_Init(GPIOA,&GPIO_InitStruct);

//外部中断函数
/*AFIO选择中断引脚*/
 GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource6);
//配置EXIT 下降沿触发
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line=EXTI_Line6;//指定配置的中断线
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;//指定中断的新状态
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;//指定模式为外部中断模式
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;//出发方式为下降沿触发。
	EXTI_Init(&EXTI_InitStruct);
	
//设置中断分组
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//NVIC配置
NVIC_InitTypeDef NVIC_InitStruct;
NVIC_InitStruct.NVIC_IRQChannel=EXTI9_5_IRQn;//中断通道 
NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
NVIC_Init(&NVIC_InitStruct);


}
/**
  * 函 数：外部中断服务函数
  * 功 能：按键按下 -> 翻转流水灯标志位
  */

//中断服务函数
void EXTI9_5_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line6) == SET)
	{
	//按下按键就是低电平
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 0)
		{
			KeyNum ++;					//计数值自增一次
		}
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
}
uint16_t Get_KeyNum(void){
	return KeyNum;
}
	