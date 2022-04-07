#include <reg52.h>
#include "LCD12864.h"
#include "delay.h"
#include "i2c.h"
#include "FDC1004.h"

//��������
unsigned long Cap1 = 0;
unsigned long Cap2 = 0;
unsigned long Cap3 = 0;
unsigned long Cap4 = 0;
unsigned int Cap = 0;
int Screen_Flag=0;//������־λ
float Moisture_Accuracy_Result = 0;//����õ��ľ���
float Set_Moisture_Accuracy = 0.02;//�趨�ĺ�ˮ�ʾ���
float Set_Moisture_Result = 16.0;//�趨�ĺ�ˮ��%

sbit K1 = P3^3;//��������ʱΪ0����ʾ���Ⱥ��Ƿ�ϸ�
sbit buzzer=P3^7;//����������

void beep()//����������
{
  unsigned char a,b;
    for(b=19;b>0;b--)
        for(a=130;a>0;a--);
    buzzer=~buzzer;
    for(b=19;b>0;b--)
        for(a=130;a>0;a--);
    buzzer=1;
}

void main(void)
{
    InitLCD();    //��ʼ��LCD12864
    digplaystart();//��������
    Screen_Flag = 1;
    Config_FDC1004_Single(0x1C00,0x08);//����FDC1004����ͨ��1���˲���
    Config_FDC1004_Single(0x3C00,0x09);//����FDC1004����ͨ��2���˲���
    Config_FDC1004_Single(0x5C00,0x0A);//����FDC1004����ͨ��3���˲���
    Config_FDC1004_Single(0x7C00,0x0B);//����FDC1004����ͨ��4���˲���
    Triggering_FDC1004(0x0C);//��������
    WaitFor_FDC1004(0x0C);
    Cap1 = ReadData_FDC1004(0x00);//��ȡ����ֵ
    Cap2 = ReadData_FDC1004(0x02);//��ȡ����ֵ
    Cap3 = ReadData_FDC1004(0x04);//��ȡ����ֵ
    Cap4 = ReadData_FDC1004(0x06);//��ȡ����ֵ
    while(1)
    {
        WaitFor_FDC1004(0x0C);
        Cap1 = ReadData_FDC1004(0x00);//��ȡ����ͨ��1����ֵ
        Cap2 = ReadData_FDC1004(0x02);//��ȡ����ͨ��1����ֵ
        Cap3 = ReadData_FDC1004(0x04);//��ȡ����ͨ��1����ֵ
        Cap4 = ReadData_FDC1004(0x06);//��ȡ����ͨ��1����ֵ
        //Cap = (Cap1+Cap2+Cap3+Cap4)/4;//�����ڲ����ĸ�ͨ����ƽ��ֵ
        
        //����Ƿ���K1
        if(K1==0)//��ʾ�����Լ��Ƿ�ϸ�
        {
            if(Screen_Flag != 2)//���Screen_Flag��Ϊ2����˵���������1����Ҫ����
            {
                ClearScreenall();//����
                digplayscreen2();//��ʾ��2
            }
            Screen_Flag = 2;//˵���Ѿ�������2
            
            Cap_accuracy_Diplay(0,72,Cap2,Set_Moisture_Result);//���ݾ�����ʾ
            Moisture_Accuracy_Result = Moisture_accuracy_Diplay(3,88,Cap2,Set_Moisture_Result);//��ˮ�ʾ�����ʾ�������غϸ���
            if(Set_Moisture_Accuracy>=Moisture_Accuracy_Result)
            {
                digplayOK();//��ʾ�ϸ�
            }
            else
            {
                digplayNOTOK();//��ʾ���ϸ�
                beep();//����
            }
        }
        else
        {
            if(Screen_Flag != 0)//���Screen_Flag��Ϊ0����˵���������2���߸տ�������Ҫ����
            {
                ClearScreenall();//����
                digplayscreen1();//��ʾ��1
            }
            Screen_Flag = 0;
            Cap_Cac_Diplay(3,48,Cap2);//������ֵ���㲢��ʾ
            Moisture_Cac_Diplay(6,56,Cap2);//��ˮ�ʼ��㲢��ʾ�����ݵ��ݴ�С����ϵõ���ˮ�ʵ���ֵ����ʾ
        }
    }
}                                                                                                    
