#include "stm32f10x.h"                  // Device header
#include "DHT11.h"
#include "Delay.h"


//#define DTH11_IO_IN() {GPIOA->CRL&0XFFFFFFF0;G

void DHT11_IO_IN(void){
GPIOA -> CRL &= 0XFFFFFFF0;
	GPIOA-> CRL |= 8 ;
	
}
void DHT11_IO_OUT(void){
  GPIOA-> CRL &= 0XFFFFFFF0;
	GPIOA-> CRL |= 3 ;
}	

//设置引脚电平
void DHT11_DQ_OUT (uint16_t Bit)
	{
GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit);	
  }	

//读取引脚电平
uint8_t DHT11_DQ_IN()
	{
     return GPIO_ReadInputDataBit(GPIOA ,GPIO_Pin_0);
}
	
void DHT11_Rst(){
	
	DHT11_IO_OUT();//设置引脚为输出方向
	DHT11_DQ_OUT(0);//拉低为低电平
	Delay_ms(20);
	DHT11_DQ_OUT(1);//拉高为高电平
	Delay_us(30);//主机至少拉高20-40us
	
}

//从DTH11读取一个位 ，返回值1/0
uint8_t DHT11_Read_Bit(void){
    uint8_t  retry =0 ;
	while (DHT11_DQ_IN()&& retry < 100 ) {
		retry ++ ;
		Delay_us(1); 
    }
	 retry = 0 ;
		while (!DHT11_DQ_IN()&& retry < 100){//等待变高电平
			retry ++;
			Delay_us(1);
}
		Delay_us(40);
if (DHT11_DQ_IN()){
	return 1 ;
}
  return 0 ;
}
//从DHT11读取一个字节，返回值，读取的数据
uint8_t DHT11_Read_Byte(void){
	uint8_t i ,data ;
	data =0 ;
	for(i =0 ;i<8;i++){
		data <<=1 ;
		data |= DHT11_Read_Bit();
	}
	return data ;
}



uint8_t DHT11_Check(void){
   uint8_t retry =0 ;
	 DHT11_IO_IN();//
	while(DHT11_DQ_IN()&& retry <100){
		retry ++ ;
		Delay_us(1);
		
 }
if(retry >= 100)
{
   return 1 ;
}	else {
	retry = 0 ;
   }
while (!DHT11_DQ_IN() &&  retry<100){
	retry ++ ;
	Delay_us(1);	
}
if (retry >= 100){
	return 1 ;
}
  return 0;
}

//读取温湿度
uint8_t DHT11_Read_Data(uint8_t* Temp,uint8_t* humidity){
	uint8_t buf[5];
	uint8_t i ;
	DHT11_Rst();
	 if (DHT11_Check()==0){   //检测回应
		 for(i=0 ;i<5;i++){   //读取40位数据
		buf[i]=DHT11_Read_Byte();	 
		 }

		 //判断和校验
   if ((buf[0]+	buf[1]+	buf[2]+	buf[3])==buf[4])
		 {
		 *humidity = buf[0];
		 *Temp =buf[2];
	 }
 }		 
   else{
	   return 1 ;
  }
     return 0;
	   }

	 
uint8_t DHT11_Init(void)//初始化DTH11
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure ;
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz ;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
  GPIO_SetBits(GPIOA,GPIO_Pin_0);
	
	DHT11_Rst();
	
  return DHT11_Check();//等待DHT11的回应
}
