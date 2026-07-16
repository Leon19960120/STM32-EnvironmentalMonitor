#include "stm32f10x.h"                  // Device header
//#include "LightSensor.h"
//光传感器初始化
void LightSensor_Init(void)
{
	//使能APB2总线上的GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	//定义一个GPIO初始化结构体，通常用于配置GPIO的各种参数
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;//设置GPIO为推挽输出模式
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_13;//设置要输出的GPIO引脚
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;//输出速度为50Mhz
	GPIO_Init(GPIOB,&GPIO_InitStruct);

}
uint8_t LightSensor_Get(void){
	return GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13);
}
