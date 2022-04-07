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
unsigned long int CC = 0;
unsigned int Gain1 = 0;
unsigned int M_ID = 12;
unsigned int D_ID = 0;

void main(void)
{
    InitLCD();    //��ʼ��LCD12864
    digplaystart();
    delay5ms();//�����ȶ�
    ClearScreen(1);
    ClearScreen(2);

    ShowChina(0,8,4);//ԭ
    ShowChina(0,24,5);//��
    ShowChina(0,40,6);//��
    ShowChina(0,56,7);//ˮ
    ShowChina(0,72,8);//��
    ShowChina(0,88,9);//��
    ShowChina(0,104,10);//��
    ShowChina(3,0,11);//��
    ShowChina(3,16,12);//��
    //ShowChina(3,32,13);//��
    //ShowChina(3,48,14);//ֵ
    ShowChina(3,32,15);//:
    ShowChina(6,0,6);//��
    ShowChina(6,16,7);//ˮ
    ShowChina(6,32,8);//��
    ShowChina(6,48,15);//:
    M_ID = ReadID_FDC1004(0xFE);
    D_ID = ReadID_FDC1004(0xFF);
    M_ID = 12;
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
        Cap = (Cap1+Cap2+Cap3+Cap4)/4;//�����ڲ����ĸ�ͨ����ƽ��ֵ
        Cap_Cac_Diplay(3,48,Cap2);//������ֵ���㲢��ʾ
        Moisture_Cac_Diplay(6,56,Cap2);//��ˮ�ʼ��㲢��ʾ�����ݵ��ݴ�С����ϵõ���ˮ�ʵ���ֵ����ʾ
    }
}                                                                                                    

//void main(void)
//{

//    Cap = 0;
//    CC = 0;
//    delay_ms(200);
//    M_ID = ReadID_FDC1004(0xFE);
//    D_ID = ReadID_FDC1004(0xFF);
//    Config_FDC1004(4,0x08);//����FDC1004
//    Triggering_FDC1004(0x0C);//��������
//    WaitFor_FDC1004(0x0C);
//    Cap1 = ReadData_FDC1004(0x00);//��ȡ����ֵ
//    Cap2 = ReadData_FDC1004(0x02);//��ȡ����ֵ
//    Cap3 = ReadData_FDC1004(0x04);//��ȡ����ֵ
//    Cap4 = ReadData_FDC1004(0x06);//��ȡ����ֵ

//    while(1)
//    {
//        WaitFor_FDC1004(0x0C);
//        Cap1 = ReadData_FDC1004(0x00);//��ȡ����ֵ
//        Cap2 = ReadData_FDC1004(0x02);//��ȡ����ֵ
//        Cap3 = ReadData_FDC1004(0x04);//��ȡ����ֵ
//        Cap4 = ReadData_FDC1004(0x06);//��ȡ����ֵ
//        Cap = (Cap1+Cap2+Cap3+Cap4)/4;

//        if(Test_Flag==1)
//        {
//            Test_Flag=0;
//            WaitFor_FDC1004(0x0C);
//            Cap1 = ReadData_FDC1004(0x00);//��ȡ����ֵ
//            Cap2 = ReadData_FDC1004(0x02);//��ȡ����ֵ
//            Cap3 = ReadData_FDC1004(0x04);//��ȡ����ֵ
//            Cap4 = ReadData_FDC1004(0x06);//��ȡ����ֵ
//            Cap = (Cap1+Cap2+Cap3+Cap4)/4;
//            CC += Cap;

//            Time_Cnt++;
//            if(Time_Cnt>=5)
//            {
//                Time_Cnt=0;
//                CC = CC/5;
//                CC=0;
//            }
//        }
//    }
//}
