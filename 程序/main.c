#include <reg52.h>
#include "LCD12864.h"
#include "delay.h"
#include "i2c.h"
#include "FDC1004.h"

//参数定义
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
    InitLCD();    //初始化LCD12864
    digplaystart();
    delay5ms();//保持稳定
    ClearScreen(1);
    ClearScreen(2);

    ShowChina(0,8,4);//原
    ShowChina(0,24,5);//油
    ShowChina(0,40,6);//含
    ShowChina(0,56,7);//水
    ShowChina(0,72,8);//率
    ShowChina(0,88,9);//检
    ShowChina(0,104,10);//测
    ShowChina(3,0,11);//电
    ShowChina(3,16,12);//容
    //ShowChina(3,32,13);//容
    //ShowChina(3,48,14);//值
    ShowChina(3,32,15);//:
    ShowChina(6,0,6);//含
    ShowChina(6,16,7);//水
    ShowChina(6,32,8);//率
    ShowChina(6,48,15);//:
    M_ID = ReadID_FDC1004(0xFE);
    D_ID = ReadID_FDC1004(0xFF);
    M_ID = 12;
    Config_FDC1004_Single(0x1C00,0x08);//配置FDC1004电容通道1单端测量
    Config_FDC1004_Single(0x3C00,0x09);//配置FDC1004电容通道2单端测量
    Config_FDC1004_Single(0x5C00,0x0A);//配置FDC1004电容通道3单端测量
    Config_FDC1004_Single(0x7C00,0x0B);//配置FDC1004电容通道4单端测量
    Triggering_FDC1004(0x0C);//触发测量
    WaitFor_FDC1004(0x0C);
    Cap1 = ReadData_FDC1004(0x00);//读取电容值
    Cap2 = ReadData_FDC1004(0x02);//读取电容值
    Cap3 = ReadData_FDC1004(0x04);//读取电容值
    Cap4 = ReadData_FDC1004(0x06);//读取电容值
    while(1)
    {
        WaitFor_FDC1004(0x0C);
        Cap1 = ReadData_FDC1004(0x00);//读取电容通道1电容值
        Cap2 = ReadData_FDC1004(0x02);//读取电容通道1电容值
        Cap3 = ReadData_FDC1004(0x04);//读取电容通道1电容值
        Cap4 = ReadData_FDC1004(0x06);//读取电容通道1电容值
        Cap = (Cap1+Cap2+Cap3+Cap4)/4;//可用于测量四个通道的平均值
        Cap_Cac_Diplay(3,48,Cap2);//电容容值计算并显示
        Moisture_Cac_Diplay(6,56,Cap2);//含水率计算并显示，根据电容大小，拟合得到含水率的数值并显示
    }
}                                                                                                    

//void main(void)
//{

//    Cap = 0;
//    CC = 0;
//    delay_ms(200);
//    M_ID = ReadID_FDC1004(0xFE);
//    D_ID = ReadID_FDC1004(0xFF);
//    Config_FDC1004(4,0x08);//配置FDC1004
//    Triggering_FDC1004(0x0C);//触发测量
//    WaitFor_FDC1004(0x0C);
//    Cap1 = ReadData_FDC1004(0x00);//读取电容值
//    Cap2 = ReadData_FDC1004(0x02);//读取电容值
//    Cap3 = ReadData_FDC1004(0x04);//读取电容值
//    Cap4 = ReadData_FDC1004(0x06);//读取电容值

//    while(1)
//    {
//        WaitFor_FDC1004(0x0C);
//        Cap1 = ReadData_FDC1004(0x00);//读取电容值
//        Cap2 = ReadData_FDC1004(0x02);//读取电容值
//        Cap3 = ReadData_FDC1004(0x04);//读取电容值
//        Cap4 = ReadData_FDC1004(0x06);//读取电容值
//        Cap = (Cap1+Cap2+Cap3+Cap4)/4;

//        if(Test_Flag==1)
//        {
//            Test_Flag=0;
//            WaitFor_FDC1004(0x0C);
//            Cap1 = ReadData_FDC1004(0x00);//读取电容值
//            Cap2 = ReadData_FDC1004(0x02);//读取电容值
//            Cap3 = ReadData_FDC1004(0x04);//读取电容值
//            Cap4 = ReadData_FDC1004(0x06);//读取电容值
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
