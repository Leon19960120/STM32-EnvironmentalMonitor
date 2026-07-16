#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "led.h"

LED_INFO led_info ={1};

//LED初始化
//void LED_Init(void){
//	//使能APB2总线上的GPIOA时钟
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
//	//定义一个GPIO初始化结构体，通常用于配置GPIO的各种参数
//	GPIO_InitTypeDef GPIO_InitStruct;
//	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;//设置GPIO为推挽输出模式
//	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_All;//设置要输出的GPIO引脚
//	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;//输出速度为50Mhz
//	GPIO_Init(GPIOA,&GPIO_InitStruct);
//	GPIO_SetBits(GPIOA,GPIO_Pin_All);//设置高电平灯灭，
//}
void Led_Init(void)
{

	GPIO_InitTypeDef gpio_initstruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//打开GPIOB的时钟
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);	//禁止JTAG功能
	
	gpio_initstruct.GPIO_Mode = GPIO_Mode_Out_PP;				//设置为输出
	gpio_initstruct.GPIO_Pin = GPIO_Pin_1;						//将初始化的Pin脚
	gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;				//可承载的最大频率
	
	GPIO_Init(GPIOA, &gpio_initstruct);							//初始化GPIO
	
	Led_Set(LED_OFF);											//初始化完成后，关闭蜂鸣器

}
////打开LED灯
//void LED_ON(void){
//		GPIO_ResetBits(GPIOA,GPIO_Pin_0);//A0的LED
//		GPIO_ResetBits(GPIOA,GPIO_Pin_1);//A1的LED
//		}


//	
////关闭LED灯
//void LED_OFF(){
//	GPIO_SetBits(GPIOA,GPIO_Pin_All);
//}


/**
 * @brief  控制所有LED的状态（单参数版本）
 * @param  status: LED_ON=点亮，LED_OFF=熄灭
 * @note   完全匹配你 Led_Set(LED_ON) / Led_Set(LED_OFF) 的调用方式
 */
void Led_Set(uint8_t status)
{
	// 核心一行：判断传入的状态，写入对应的高低电平
	GPIO_WriteBit(GPIOA,GPIO_Pin_1,status ==LED_ON ? Bit_RESET:Bit_SET);
	// 记录状态：把当前状态存入全局变量，方便其他函数读取
	led_info.Led_Status = status;
}

