#include "key.h"
#include "adc.h"
#include "delay.h" 

//������ʼ������
void KEY_Init(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOA,GPIOEʱ��
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4; //KEY0 KEY1 KEY2��Ӧ����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//WK_UP��Ӧ����PA0
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA0
 
} 
//����������
//mode:0,��֧��������;1,֧��������;

u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY1==0||KEY2==0||KEY3==0||WK_UP==1))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY1==0)return 1;
		else if(KEY2==0)return 2;
		else if(KEY3==0)return 3;
		else if(WK_UP==1)return 4;
	}else if(KEY1==1&&KEY2==1&&KEY3==1&&WK_UP==0)key_up=1; 	    
 	return 0;// �ް�������
}

extern u16 ADC_BUF[60]; 	
void RolKeyScan1(RolKey_typedef * RolKey,u8 mode)
{
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������	
	RolKey->dx=1;RolKey->dy=1;	
	if(ADC_BUF[0]>51||ADC_BUF[0]<11||ADC_BUF[1]>51||ADC_BUF[1]<11)	RolKey->sta=1;
	else 	RolKey->sta=0;		
	if(key_up&&RolKey->sta==1)
	{
		key_up=0;
		if(ADC_BUF[0]>51)	RolKey->dx=2;
		else if(ADC_BUF[0]<11)	RolKey->dx=0;
		else RolKey->dx=1;	
		if(ADC_BUF[1]>51)	RolKey->dy=0;
		else if(ADC_BUF[1]<11)	RolKey->dy=2;
		else 	RolKey->dy=1;		
	}
	else if(RolKey->sta==0)	key_up=1; 
}


void RolKeyScan2(RolKey_typedef * RolKey,u8 mode)
{
	static u8 key_up=0;//�������ɿ���־
	if(mode)key_up=1;  //֧������	
	RolKey->dx=1;RolKey->dy=1;	
	if(ADC_BUF[2]>51||ADC_BUF[2]<11||ADC_BUF[3]>51||ADC_BUF[3]<11)	RolKey->sta=1;
	else 	RolKey->sta=0;		
	if(key_up&&RolKey->sta==1)
	{
		key_up=0;
		if(ADC_BUF[2]>51)	RolKey->dx=2;
		else if(ADC_BUF[2]<11)	RolKey->dx=0;
		else RolKey->dx=1;	
		if(ADC_BUF[3]>51)	RolKey->dy=0;
		else if(ADC_BUF[3]<11)	RolKey->dy=2;
		else 	RolKey->dy=1;		
	}
	else if(RolKey->sta==0)	key_up=1; 
}











