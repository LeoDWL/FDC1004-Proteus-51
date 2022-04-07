#include <reg52.h>
#include "LCD12864.h"
#include "code.h"
#include "delay.h"

void nop(void)
{
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
}

//状态检查
void CheckState(void)
{
    uchar dat;
    dat = 0x00;
    di=0;
    rw=1;
    /*	do
    	{
    		P1=0xff;
    		en=1;
    		dat=P1;
    		en=0;
    		dat&=0x80; //仅当第7位为0时才可操作
    	}while(dat);*/
}

//写显示数据
//dat:显示数据
void WriteByte(uchar dat)
{
    CheckState();
    di=1;
    rw=0;
    LCD12864DataPort=dat;
    en=1;
    en=0;
}


void lcd_cmd_wr(uchar command)
{
    cs1=1;
    cs2=1;
    CheckState();
    rw=0;
    di=0;
    LCD12864DataPort=command;
    en=1;
    en=0;
}
/*-----------------------------------------------------------------------------------------------------*/
//向LCD发送命令
//command :命令
void SendCommandToLCD(uchar command)
{
    CheckState();
    rw=0;
    di=0;
    LCD12864DataPort=command;
    en=1;
    en=0;
}
/*----------------------------------------------------------------------------------------------------*/
//设定行地址(页)--X 0-7
void SetLine(uchar line)
{
    line &= 0x07; // 0<=line<=7
    line |= 0xb8; //1011 1xxx
    SendCommandToLCD(line);
}
//设定列地址--Y 0-63
void SetColumn(uchar column)
{
    column &= 0x3f; // 0=<column<=63
    column |= 0x40; //01xx xxxx
    SendCommandToLCD(column);
}
//设定显示开始行--XX
void SetStartLine(uchar startline) //0--63
{
    //startline &= 0x07;
    startline |= 0xc0; //1100 0000
    SendCommandToLCD(startline);
}
//开关显示
void SetOnOff(uchar onoff)
{
    onoff|=0x3e; //0011 111x
    SendCommandToLCD(onoff);
}
/*---------------------------------------------------------------------------------------------------*/
//选择屏幕
//screen: 0-全屏,1-左屏,2-右屏
void SelectScreen(uchar screen)
{   //北京显示器:负有效 cs1: 0--右; cs2: 0--左
    switch(screen)
    {
    case 0:
        cs1=0;//全屏
        nop();
        cs2=0;
        nop();
        break;
    case 1:
        cs1=1;//左屏
        nop();
        cs2=0;
        nop();
        break;
    case 2:
        cs1=0;//右屏
        nop();
        cs2=1;
        nop();
        break;
    default:
        break;
    }
}
/*---------------------------------------------------------------------------------------------------*/
//清屏
//screen: 0-全屏,1-左屏,2-右
void ClearScreen(uchar screen)
{
    uchar i,j;
    SelectScreen(screen);
    for(i=0; i<8; i++)
    {
        SetLine(i);
        for(j=0; j<64; j++)
        {
            WriteByte(0x00);
        }
    }
}
/*--------------------------------------------------------------------------------------------------*/
//显示8*8点阵
//lin:行(0-7), column: 列(0-127)
//address : 字模区首地址
void Show8x8(uchar lin,uchar column,uchar *address)
{
    uchar i;
//	if(column>128) {return;}
    if(column<64)
    {
        SelectScreen(1); //如果列数<64则从第一屏上开始写
        SetLine(lin);
        SetColumn(column);
        for(i=0; i<8; i++)
        {
            if(column+i<64)
            {
                WriteByte(*(address+i));
            }
            else
            {
                SelectScreen(2);
                SetLine(lin);
                SetColumn(column-64+i);
                WriteByte(*(address+i));
            }
        }
    }
    else
    {
        SelectScreen(2); //否则从第二屏上开始写
        column-=64; //防止越界
        SetLine(lin);
        SetColumn(column);
        for(i=0; i<8; i++)
        {
            if(column+i<64)
            {
                WriteByte(*(address+i));
            }
            else
            {
                SelectScreen(1);
                SetLine(lin);
                SetColumn(column-64+i);
                WriteByte(*(address+i));
            }
        }
    }
}

//显示数字8*16
void ShowNumber(uchar lin,uchar column,uchar num)
{
    uchar *address;
    address=&Numcode[num];
    Show8x8(lin,column,address);
    Show8x8(lin+1,column,address+8);
}



//显示汉字16*16
void ShowChina(uchar lin,uchar column,uchar num)
{
    uchar *address;
//	if(lin>7 || column>127){return;}
    address = &word[num];
    Show8x8(lin,column,address);
    Show8x8(lin,column+8,address+8);
    Show8x8(lin+1,column,address+16);
    Show8x8(lin+1,column+8,address+24);
}

void InitLCD(void) //初始化LCD
{
    uchar i=2000; //延时
    while(i--);
    SetOnOff(1); //开显示
    ClearScreen(1);//清屏
    ClearScreen(2);
    SetStartLine(0); //开始行:0
}

void ClearScreenall() { //清全屏
    ClearScreen(1);
    ClearScreen(2);
}

void digplaystart() {//开机显示
    uchar i=0;
    ShowChina(2,64-16*1.5,0);
    ShowChina(2,64-16*0.5,1);
    ShowChina(2,64+16*0.5,2);
    ShowChina(2,64+16*1.5,3);
    for (i=0; i<10; i++) {
        ShowNumber(4,32+8*i,19);
        delay5ms();
        delay5ms();
    }
}

void digplayscreen1() {//显示屏1
    ShowChina(0,8,4);//原
    ShowChina(0,24,5);//油
    ShowChina(0,40,6);//含
    ShowChina(0,56,7);//水
    ShowChina(0,72,8);//率
    ShowChina(0,88,9);//检
    ShowChina(0,104,10);//测
    ShowChina(3,0,11);//电
    ShowChina(3,16,12);//容
    ShowChina(3,32,15);//:
    ShowChina(6,0,6);//含
    ShowChina(6,16,7);//水
    ShowChina(6,32,8);//率
    ShowChina(6,48,15);//:
}

void digplayscreen2() {//显示屏2
    ShowChina(0,0,11);//电
    ShowChina(0,16,12);//容
    ShowChina(0,32,16);//精
    ShowChina(0,48,17);//度
    ShowChina(0,64,15);//:
    ShowChina(3,0,6);//含
    ShowChina(3,16,7);//水
    ShowChina(3,32,8);//率
    ShowChina(3,48,16);//精
    ShowChina(3,64,17);//度
    ShowChina(3,80,15);//:
    ShowChina(6,0,18);//是
    ShowChina(6,16,19);//否
    ShowChina(6,32,20);//合
    ShowChina(6,48,21);//格
    ShowChina(6,64,15);//:
}

void digplayOK()
{
    ShowChina(6,80,20);//合
    ShowChina(6,96,21);//格
}
void digplayNOTOK()
{
    ShowChina(6,80,22);//不
    ShowChina(6,96,20);//合
    ShowChina(6,112,21);//格
}

//用于实时显示具体坐标的数字的函数，其中x对应的是显示器的行，y对应的是显示器的列，m对应的是要显示的数字
void DisP_parameters(int x,int y,int m) {
    switch(m) {
    case(1):
        ShowNumber(x,y,1);
        break;
    case(2):
        ShowNumber(x,y,2);
        break;
    case(3):
        ShowNumber(x,y,3);
        break;
    case(4):
        ShowNumber(x,y,4);
        break;
    case(5):
        ShowNumber(x,y,5);
        break;
    case(6):
        ShowNumber(x,y,6);
        break;
    case(7):
        ShowNumber(x,y,7);
        break;
    case(8):
        ShowNumber(x,y,8);
        break;
    case(9):
        ShowNumber(x,y,9);
        break;
    case(0):
        ShowNumber(x,y,0);
        break;
    case(10):
        ShowNumber(x,y,13);
        break;
    case(11):
        ShowNumber(x,y,14);
        break;
    case(12):
        ShowNumber(x,y,15);
        break;
    case(13):
        ShowNumber(x,y,16);
        break;
    }
}

////显示电容的数值
//void Disp_Cap(unsigned int c)
//{
//  unsigned char i;
//  unsigned char Disp1[4];
//    i = c/10000;
//	Disp1[0] = digit[i];
//	i = c/1000%10;
//	Disp1[1] = digit[i];
//	i = c/100%10;
//	Disp1[2] = digit[i];
//	i = c/10%10;
//	Disp1[3] = digit[i];
//  for(i=0;i<4;i++)
//  {LCDMEM[i+1] = Disp1[i];}
//}