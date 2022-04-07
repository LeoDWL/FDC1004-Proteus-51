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
int Screen_Flag=0;//清屏标志位
float Moisture_Accuracy_Result = 0;//计算得到的精度
float Set_Moisture_Accuracy = 0.02;//设定的含水率精度
float Set_Moisture_Result = 16.0;//设定的含水率%

sbit K1 = P3^3;//案件按下时为0，显示精度和是否合格
sbit buzzer=P3^7;//蜂鸣器引脚

void beep()//蜂鸣器报警
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
    InitLCD();    //初始化LCD12864
    digplaystart();//开机动画
    Screen_Flag = 1;
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
        //Cap = (Cap1+Cap2+Cap3+Cap4)/4;//可用于测量四个通道的平均值
        
        //检测是否按下K1
        if(K1==0)//显示精度以及是否合格
        {
            if(Screen_Flag != 2)//如果Screen_Flag不为2，则说明进入过屏1，需要清屏
            {
                ClearScreenall();//清屏
                digplayscreen2();//显示屏2
            }
            Screen_Flag = 2;//说明已经进入屏2
            
            Cap_accuracy_Diplay(0,72,Cap2,Set_Moisture_Result);//电容精度显示
            Moisture_Accuracy_Result = Moisture_accuracy_Diplay(3,88,Cap2,Set_Moisture_Result);//含水率精度显示，并返回合格率
            if(Set_Moisture_Accuracy>=Moisture_Accuracy_Result)
            {
                digplayOK();//显示合格
            }
            else
            {
                digplayNOTOK();//显示不合格
                beep();//报警
            }
        }
        else
        {
            if(Screen_Flag != 0)//如果Screen_Flag不为0，则说明进入过屏2或者刚开机，需要清屏
            {
                ClearScreenall();//清屏
                digplayscreen1();//显示屏1
            }
            Screen_Flag = 0;
            Cap_Cac_Diplay(3,48,Cap2);//电容容值计算并显示
            Moisture_Cac_Diplay(6,56,Cap2);//含水率计算并显示，根据电容大小，拟合得到含水率的数值并显示
        }
    }
}                                                                                                    
