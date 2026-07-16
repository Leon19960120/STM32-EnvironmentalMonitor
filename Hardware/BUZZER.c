#include "stm32f10x.h"                  // Device header
#include "Delay.h"
//蜂鸣器初始化
void Buzzer_Init(void)
	{
	//使能APB2总线上的GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	//定义一个GPIO初始化结构体，通常用于配置GPIO的各种参数
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;//设置GPIO为推挽输出模式
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_12;//设置要输出的GPIO引脚
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;//输出速度为50Mhz
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	GPIO_SetBits(GPIOB,GPIO_Pin_12);//设置上电默认电平状态，高电平灯灭，
}

//open Buzzer
void Buzzer_ON(void){
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);//
}

//close Buzzer
void Buzzer_OFF(void){
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
}

//获得输出数据寄存器的值进行反转
void Buzzer_Turn(void)
{
  if(GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_12)==0){
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
	return ;
  }
   GPIO_ResetBits(GPIOB,GPIO_Pin_12);
}




// 项目启动提示：两快一短
void Buzzer_StartBeep(void)
{
    for (int i = 0; i < 2; i++) {
        Buzzer_ON();
        Delay_ms(100);
        Buzzer_OFF();
        Delay_ms(100);
    }
    Buzzer_ON();
    Delay_ms(50);
    Buzzer_OFF();
    Delay_ms(100);
}

// 数据收发提示：短鸣一声
void Buzzer_DataBeep(void)
{
    Buzzer_ON();
    Delay_ms(50);
    Buzzer_OFF();
}

// 持续报警（开启/停止）
static uint8_t alarmEnabled = 0;

void Buzzer_AlarmStart(void)
{
    alarmEnabled = 1;
    Buzzer_ON();      // 一直响，直到调用 Stop
}

void Buzzer_AlarmStop(void)
{
    alarmEnabled = 0;
    Buzzer_OFF();
}

// 温湿度阈值检查并控制报警
// 假设温度范围 0~50℃，湿度 20~90%，超出则报警
#define TEMP_MIN    0
#define TEMP_MAX    50
#define HUMI_MIN    20
#define HUMI_MAX    90

void Buzzer_CheckThreshold(int16_t temp, int16_t humi)
{
    if (temp < TEMP_MIN || temp > TEMP_MAX || humi < HUMI_MIN || humi > HUMI_MAX) {
        if (!alarmEnabled) {
            Buzzer_AlarmStart();
        }
    } else {
        if (alarmEnabled) {
            Buzzer_AlarmStop();
        }
    }
}

