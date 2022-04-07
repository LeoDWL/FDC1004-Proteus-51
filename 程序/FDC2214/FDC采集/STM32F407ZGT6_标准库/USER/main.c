//系统文件
#include "sys.h"
#include "delay.h"
#include "usart.h"

//内部驱动           
#include "tim1.h" 
#include "tim8.h" 
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "exti.h"
#include "dac.h"
#include "myiic.h"
#include "spi.h"

//外围电路           
#include "led.h"  //LED0-PB5,LED1-PE5  
#include "key.h"  //KEY0-PE4,KEY1-PE3,WKUP-PA0  
//#include "24l01.h"
#include "lcd_4_3.h"//PB0-2,PD:0,1,4,5,8-10,14,15,PE7-15,PF9-11,PG:0,12
#include "oled.h"//D0-PF0,D1-PF2,RES-PF4,DC-PF6,CS-PF8
#include "LCD_5110.h"
#include "FDC2214.h"
//用户算法
#include "handsome.h"

//全局变量
float value=0;
u8 number=100;
u8 key=0;
char str[20];
u8 temp[50] = {0}; 
u16 AdcValue1[10];
u16 AdcValue2[10];
extern u16 ADC_BUF[60];
RolKey_typedef RolKey1,RolKey2;
int main(void)
{ 

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);	//初始化串口波特率为115200	
	LED_Init();					//初始化LED 
 	LCD_Init();					//LCD初始化  
	LCD_Clear(WHITE);
	POINT_COLOR=RED;
	
	FDC2214_Init();
	

	delay_s(1); 
   	while(1)
	{
		POINT_COLOR=RED;
		value=FDC_Result(CH0);
		sprintf(str,"CH0=%.4f",value);
		LCD_ShowString(10,100,100,16,16,(u8 *)str);
		value=FDC_Result(CH1);
		sprintf(str,"CH1=%.4f",value);
		LCD_ShowString(10,150,100,16,16,(u8 *)str);
		value=FDC_Result(CH2);
		sprintf(str,"CH2=%.4f",value);
		LCD_ShowString(10,200,100,16,16,(u8 *)str);
		value=FDC_Result(CH3);
		sprintf(str,"CH3=%.4f",value);
		LCD_ShowString(10,250,100,16,16,(u8 *)str);


		delay_ms(100);
//		RolKeyScan1(&RolKey1,0);
//		if(RolKey1.dx==0)LED0=!LED0;
//		if(RolKey1.dy==0)LED0=!LED0;
//		if(RolKey1.dx==2)LED1=!LED1;
//		if(RolKey1.dy==2)LED1=!LED1;
//		
//		RolKeyScan2(&RolKey2,1);
//		if(RolKey2.dx==0)LED1=!LED1;
//		if(RolKey2.dy==0)LED1=!LED1;
//		if(RolKey2.dx==2)LED2=!LED2;
//		if(RolKey2.dy==2)LED2=!LED2;
		
		/*delay_ms(50);	
		//MyColorNum(120,160,123,16,RED,WHITE);
		FAST_NUM(120,160,123);
		MyColorStr(120,200,"helloworld",16,RED,BLUE);*/
	}
		
}

