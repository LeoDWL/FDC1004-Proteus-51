#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "sys.h"
#include "delay.h"

//����OLED��ʾ��
	//OLED ģ����ص��ǵ͹���,��������,�����򵥵��ص�,�������� SSD1306��
	//�ӿ�ģʽ����BS1,BS2����,��ʹ��spiģʽ,�����ӵش���.

//���߳�ȥVC,GND���������
	//CS		 - PD6��	OLED Ƭѡ�ź�,��оƬʹ���ź�
	//RST(RES)   - PG15��	Ӳ��λ OLED��
	//DC		 - PD3��	����/���ݱ�־��0����д���1����д���ݣ���
	//SCLK		 - PC0��	����ʱ����,D0 
	//SDIN 		 - PC1��	����������,D1 

//�������˼·:
	//1,���� STM32 �� OLED  ģ�������ӵ� IO
	//2,��ʼ�� OLED,������ʾ,����Դ� 
	//3,ͨ���������ַ������ָ���OLED_GRAM����
	//4,��OLED_GRAM����ˢ�� OLED �Դ���,��Ļ�Զ�һֱ���Դ�,������Ļ

//������ʾ����
	//�ȴ���һ����ʾ������ĺ���,֮���"��ʾ�ַ�"�Ի��㺯��Ϊ��ʯ,��ʵ�ֹ���
	//�ٺ����"��ʾ����""��ʾ�ַ���"�ȶ�����"��ʾ�ַ�"Ϊ����������ʾ

//�����Դ�
	//OLED�Դ�һ֡����128*64���Դ�,�в���,ÿ���н��Ϊһ��8λ������,
	//OLED���Դ水��ĳһ˳����128*8���ֽ�,����Ϊʹ��spiģʽֻ��д��
	//���ܶ�ȡ,�����޷�ʵ�־�ȷ����Ĺ���,���Խ���취�Ǵ���һ��[128][8]
	//����,ÿ�β������Ƕ��������,Ȼ��������ˢ���Դ�����,�������ַ�����̫
	//����С�洢����Ƭ����51��Ƭ��

//��������
//���������ֽھ���һ������,��һ���ֽ��൱�ڵ�ַ,�ڶ����ֽڱ�ʾҪִ�е�����
	//0X81,���д���һ��8λ���ݾ�������,ֵԽ����ĻԽ����
	//0X8D,���д���0x14/0x10������ɱõĿ���/�رա�ģ���ʼ����ʱ�򣬱���ѡ����
//����һ���ֽھ�������,ǰ��λ��ʾ��ַ,����λ��ʾ����
	//0XAE/0XAF,0XAE Ϊ�ر���ʾ���0XAF Ϊ������ʾ����
	//0XB0~B7����������ҳ��ַ���ɱ�ʾһ��8ҳ,��һ��64������,ÿ8λ���غ���һ��Ϊһҳ
	//0X00~0X0F����ָ������������ʾʱ����ʼ�е�ַ����λ��
	//0X10~0X1F����ָ������������ʾʱ����ʼ�е�ַ����λ���������ʹ�ÿɱ�ʾһ��128��

u8 OLED_GRAM[128][8];	//128��,8��,һ�����ݰ���8������
//д���ʱ��һ��һ�е�д���Դ�
	//		0	1	2	3	4	5	6	7 
	//0		F	A	F	A	F	A	F	A
	//1		A	F	A	F	A	F	A	F
	//2		F	A	F	A	F	A	F	A
	//3		A	F	A	F	A	F	A	F
	//4		A	F	A	F	A	F	A	F
	//5		A	F	A	F	A	F	A	F
	//6		A	F	A	F	A	F	A	F
	//7		A	F	A	F	A	F	A	F
	//,,, ,,	
	//126 A	F	A	F	A	F	A	F
	//127 F	A	F	A	F	A	F	A

//���������������ˢ�µ��Դ�����,��Ϊ��Ļֻ�ܶ�ȡ�Դ����������
void OLED_Refresh_Gram(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		
		OLED_WR_Byte (0x00,OLED_CMD);
		OLED_WR_Byte (0x10,OLED_CMD);     
//���������ݸ�ȡ����λ,���һ��8λ����,��ʾ0~127,��������д�����ݵ���ʼ��ַ
		
		for(n=0;n<128;n++)
			OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA);
	}   
}

//��SSD1306д�����ݻ�ִ������
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
//Ϊ����ǿ�ɶ���,�Ѿ�����ͷ���崦��
//#define OLED_CMD  0	�������õ�ʱ����"����"
//#define OLED_DATA 1	 ���Դ�д�����ݵ�ʱ����"����"
void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	u8 i;			  
	OLED_RS=cmd; 
	OLED_CS=0;		  
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK=0;
		if(dat&0x80)OLED_SDIN=1;
		else OLED_SDIN=0;
		OLED_SCLK=1;
		dat<<=1;   
	}				 
	OLED_CS=1;		  
	OLED_RS=1;   	  
} 
	  	  
//����OLED��ʾ    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //�������õ�ɱ�
	OLED_WR_Byte(0X14,OLED_CMD);  //����ɱô�(��ͨ��Դ)
	OLED_WR_Byte(0XAF,OLED_CMD);  //������ʾ
}
//�ر�OLED��ʾ     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //�������õ�ɱ�
	OLED_WR_Byte(0X10,OLED_CMD);  //����ɱùر�(�رյ�Դ)
	OLED_WR_Byte(0XAE,OLED_CMD);  //�ر���ʾ
}		   			

//��������,�����������ȫ��д��0,��Ļȫ��Ϩ��
void OLED_Clear(void)  
{  
	u8 i,n;  
	for(i=0;i<8;i++)for(n=0;n<128;n++)OLED_GRAM[n][i]=0X00;  
	OLED_Refresh_Gram();//������ʾ
}


//һ��ͨ�õ��ڵ㣨x��y���� 1 ���ʽΪ��OLED_GRAM[x][7-y/8]|=1<<(7-y%8)��
//���� 
//x:0~127
//y:0~63
//t:1 ���� ,0 Ϩ��õ�				   
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;//������Χ��.
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	    
}

//x1,y1,x2,y2 �������ĶԽ�����
//dot:0,���;1,���	  
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  
{  
	u8 x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}													    
	OLED_Refresh_Gram();//����������������ʾ
}

//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 12/16/24

//���ѡ���12������,��ô������12*6������,��12���ֽ�,
	//���β���ʱ,��һ���ֽڶ�������,�ڶ����ֽ�ֻ�и�4λ
	//������,����λ�޹ؾͶ�����,Ȼ���л����ڶ���,,,һ����6��
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���
	chr=chr-' ';//�õ�ƫ�ƺ��ֵ		 
    for(t=0;t<csize;t++)
    {   
		if(size==12)temp=asc2_1206_[chr][t]; 	 	//����1206����
		else if(size==16)temp=asc2_1608_[chr][t];	//����1608����
		else if(size==24)temp=asc2_2412_[chr][t];	//����2412����
		else return;							
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
//��temp�����λ���д�����,����֮��,���θ�λ�������Ƶ����λ,ѭ������ֱ��8�������
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)//����ʹ��Ч���ص�"0"�����Ե�
			{
				y=y0;
				x++;
				break;//ִ������֮������ѭ��,����һ���ֽڽ��д��
			}
		}  	 
    }
}

//����10^n����
u32 mypow(u8 n)
{
	u32 result=1;	 
	while(n--)result*=10;    
	return result;
}				  

//��ʾ����
//x,y :�������	 
//len : ��λ��
//size: ��С
//num:��ֵ(0~4294967295);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(len-t-1))%10;//�õ���ǰҪ��ʾ��ĳһλ,�Ӹ�λ�����½�
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size,1);//���λ�����٣������Ҷ���
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1); //����ǰλ���ַ�����ʽ������
	}
	OLED_Refresh_Gram();
} 
//��ʾ�ַ���
//x,y:�������  
//size:�����С 
//*p:�ַ�����ʼ��ַ 
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size)
{	
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�
    {       
        if(x>(128-(size/2)))
					{x=0;y+=size;}
        if(y>(64-size))
					{y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,size,1);	 
        x+=size/2;
        p++;
    }  
	OLED_Refresh_Gram();
}	   

//��ʼ��SSD1306					    
void OLED_Init(void)
{ 	
    FAST_INIT(PF,0,PP);
	FAST_INIT(PF,2,PP);
	FAST_INIT(PF,4,PP);
	FAST_INIT(PF,6,PP);
	FAST_INIT(PF,8,PP);

	OLED_CS=1;
	OLED_RS=1;	 
	
	OLED_RST=0;
	delay_ms(100);
	OLED_RST=1; 
	
//�����ǳ����ṩ��Ĭ�ϳ�ʼ������,����ҪŪ���ϸ��	
//***********************************************************
	OLED_WR_Byte(0xAE,OLED_CMD); //�ر���ʾ
	OLED_WR_Byte(0xD5,OLED_CMD); //����ʱ�ӷ�Ƶ����,��Ƶ��
	OLED_WR_Byte(80,OLED_CMD);   //[3:0],��Ƶ����;[7:4],��Ƶ��
	OLED_WR_Byte(0xA8,OLED_CMD); //��������·��
	OLED_WR_Byte(0X3F,OLED_CMD); //Ĭ��0X3F(1/64) 
	OLED_WR_Byte(0xD3,OLED_CMD); //������ʾƫ��
	OLED_WR_Byte(0X00,OLED_CMD); //Ĭ��Ϊ0

	OLED_WR_Byte(0x40,OLED_CMD); //������ʾ��ʼ�� [5:0],����.
													    
	OLED_WR_Byte(0x8D,OLED_CMD); //��ɱ�����
	OLED_WR_Byte(0x14,OLED_CMD); //bit2������/�ر�
	OLED_WR_Byte(0x20,OLED_CMD); //�����ڴ��ַģʽ
	OLED_WR_Byte(0x02,OLED_CMD); //[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;
	OLED_WR_Byte(0xA1,OLED_CMD); //���ض�������,bit0:0,0->0;1,0->127;
	OLED_WR_Byte(0xC0,OLED_CMD); //����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·��
	OLED_WR_Byte(0xDA,OLED_CMD); //����COMӲ����������
	OLED_WR_Byte(0x12,OLED_CMD); //[5:4]����
		 
	OLED_WR_Byte(0x81,OLED_CMD); //�Աȶ�����
	OLED_WR_Byte(0xEF,OLED_CMD); //1~255;Ĭ��0X7F (��������,Խ��Խ��)
	OLED_WR_Byte(0xD9,OLED_CMD); //����Ԥ�������
	OLED_WR_Byte(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
	OLED_WR_Byte(0xDB,OLED_CMD); //����VCOMH ��ѹ����
	OLED_WR_Byte(0x30,OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	OLED_WR_Byte(0xA4,OLED_CMD); //ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
	OLED_WR_Byte(0xA6,OLED_CMD); //������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ	    						   
	OLED_WR_Byte(0xAF,OLED_CMD); //������ʾ	 
//************************************************************

	OLED_Clear();
//���Դ�ȫ��д����,��Ϩ����Ļ
}  





























