#include "tim8.h"
#include "led.h"


//通用定时器8中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
void TIM8_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStrecture;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);/*使能定时器8的时钟*/


	NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_TIM13_IRQn;/*定时器8的中断通道使能*/
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;/*定时器1的中断通道使能*/
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;/*抢占优先级*/
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;/*响应优先级*/
	NVIC_Init(&NVIC_InitStructure);/*配置中断分组，并使能中断*/
	
  	TIM_TimeBaseInitStrecture.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStrecture.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStrecture.TIM_ClockDivision = TIM_CKD_DIV1;/*时钟分频*/
	TIM_TimeBaseInitStrecture.TIM_CounterMode = TIM_CounterMode_Up;/*向上计数*/
	TIM_TimeBaseInitStrecture.TIM_RepetitionCounter = 0;/*重复计数寄存器*/
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseInitStrecture);/*初始化*/

	TIM_ClearFlag(TIM8,TIM_FLAG_Update);/*清更新标志位*/
	TIM_ITConfig(TIM8,TIM_IT_Update,ENABLE);/*使能中断*/
	TIM_Cmd(TIM8,ENABLE);/*使能计数*/
	
}

//定时器8中断服务函数
void TIM8_UP_TIM13_IRQHandler(void)
{

	LED1=!LED1;
	TIM_ClearFlag(TIM8,TIM_FLAG_Update);/*清中断标志*/
}
