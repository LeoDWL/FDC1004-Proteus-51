#include "tim1.h"
#include "led.h"


//ͨ�ö�ʱ��1�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
void TIM1_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStrecture;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);/*ʹ�ܶ�ʱ��1��ʱ��*/


	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;/*��ʱ��1���ж�ͨ��ʹ��*/
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;/*��ʱ��1���ж�ͨ��ʹ��*/
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;/*��ռ���ȼ�*/
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;/*��Ӧ���ȼ�*/
	NVIC_Init(&NVIC_InitStructure);/*�����жϷ��飬��ʹ���ж�*/
	
  	TIM_TimeBaseInitStrecture.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStrecture.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStrecture.TIM_ClockDivision = TIM_CKD_DIV1;/*ʱ�ӷ�Ƶ*/
	TIM_TimeBaseInitStrecture.TIM_CounterMode = TIM_CounterMode_Up;/*���ϼ���*/
	TIM_TimeBaseInitStrecture.TIM_RepetitionCounter = 0;/*�ظ������Ĵ���*/
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStrecture);/*��ʼ��*/

	TIM_ClearFlag(TIM1,TIM_FLAG_Update);/*����±�־λ*/
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);/*ʹ���ж�*/
	TIM_Cmd(TIM1,ENABLE);/*ʹ�ܼ���*/
	
}

//��ʱ��1�жϷ�����
void TIM1_UP_TIM10_IRQHandler(void)
{

	LED0=!LED0;
	TIM_ClearFlag(TIM1,TIM_FLAG_Update);/*���жϱ�־*/
}
