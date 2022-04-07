#include "sys.h"  

void FAST_INIT(u32 port,u8 pin,u8 mode)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(port, ENABLE);
	GPIO_InitStructure.GPIO_Pin = 1<<pin;
	switch(mode)
	{
		case PP:
		{
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
		}break;
		case IPU:
		{
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
		}break;
		case IPD:
		{
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//����
		}break;
		case NP:
		{
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		}break;
	}
	switch(port)
 {
	 case PA:GPIO_Init(GPIOA, &GPIO_InitStructure);break;
	 case PB:GPIO_Init(GPIOB, &GPIO_InitStructure);break;
	 case PC:GPIO_Init(GPIOC, &GPIO_InitStructure);break;
	 case PD:GPIO_Init(GPIOD, &GPIO_InitStructure);break;
	 case PE:GPIO_Init(GPIOE, &GPIO_InitStructure);break;
	 case PF:GPIO_Init(GPIOF, &GPIO_InitStructure);break;
	 case PG:GPIO_Init(GPIOG, &GPIO_InitStructure);break;
 }	
	
}

//THUMBָ�֧�ֻ������
//�������·���ʵ��ִ�л��ָ��WFI  
__asm void WFI_SET(void)
{
	WFI;		  
}
//�ر������ж�(���ǲ�����fault��NMI�ж�)
__asm void INTX_DISABLE(void)
{
	CPSID   I
	BX      LR	  
}
//���������ж�
__asm void INTX_ENABLE(void)
{
	CPSIE   I
	BX      LR  
}
//����ջ����ַ
//addr:ջ����ַ
__asm void MSR_MSP(u32 addr) 
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}
















