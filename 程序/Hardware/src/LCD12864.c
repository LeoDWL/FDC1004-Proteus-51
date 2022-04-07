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

//״̬���
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
    		dat&=0x80; //������7λΪ0ʱ�ſɲ���
    	}while(dat);*/
}

//д��ʾ����
//dat:��ʾ����
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
//��LCD��������
//command :����
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
//�趨�е�ַ(ҳ)--X 0-7
void SetLine(uchar line)
{
    line &= 0x07; // 0<=line<=7
    line |= 0xb8; //1011 1xxx
    SendCommandToLCD(line);
}
//�趨�е�ַ--Y 0-63
void SetColumn(uchar column)
{
    column &= 0x3f; // 0=<column<=63
    column |= 0x40; //01xx xxxx
    SendCommandToLCD(column);
}
//�趨��ʾ��ʼ��--XX
void SetStartLine(uchar startline) //0--63
{
    //startline &= 0x07;
    startline |= 0xc0; //1100 0000
    SendCommandToLCD(startline);
}
//������ʾ
void SetOnOff(uchar onoff)
{
    onoff|=0x3e; //0011 111x
    SendCommandToLCD(onoff);
}
/*---------------------------------------------------------------------------------------------------*/
//ѡ����Ļ
//screen: 0-ȫ��,1-����,2-����
void SelectScreen(uchar screen)
{   //������ʾ��:����Ч cs1: 0--��; cs2: 0--��
    switch(screen)
    {
    case 0:
        cs1=0;//ȫ��
        nop();
        cs2=0;
        nop();
        break;
    case 1:
        cs1=1;//����
        nop();
        cs2=0;
        nop();
        break;
    case 2:
        cs1=0;//����
        nop();
        cs2=1;
        nop();
        break;
    default:
        break;
    }
}
/*---------------------------------------------------------------------------------------------------*/
//����
//screen: 0-ȫ��,1-����,2-��
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
//��ʾ8*8����
//lin:��(0-7), column: ��(0-127)
//address : ��ģ���׵�ַ
void Show8x8(uchar lin,uchar column,uchar *address)
{
    uchar i;
//	if(column>128) {return;}
    if(column<64)
    {
        SelectScreen(1); //�������<64��ӵ�һ���Ͽ�ʼд
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
        SelectScreen(2); //����ӵڶ����Ͽ�ʼд
        column-=64; //��ֹԽ��
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

//��ʾ����8*16
void ShowNumber(uchar lin,uchar column,uchar num)
{
    uchar *address;
    address=&Numcode[num];
    Show8x8(lin,column,address);
    Show8x8(lin+1,column,address+8);
}



//��ʾ����16*16
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

void InitLCD(void) //��ʼ��LCD
{
    uchar i=2000; //��ʱ
    while(i--);
    SetOnOff(1); //����ʾ
    ClearScreen(1);//����
    ClearScreen(2);
    SetStartLine(0); //��ʼ��:0
}

void ClearScreenall() { //��ȫ��
    ClearScreen(1);
    ClearScreen(2);
}

void digplaystart() {//������ʾ
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

void digplayscreen1() {//��ʾ��1
    ShowChina(0,8,4);//ԭ
    ShowChina(0,24,5);//��
    ShowChina(0,40,6);//��
    ShowChina(0,56,7);//ˮ
    ShowChina(0,72,8);//��
    ShowChina(0,88,9);//��
    ShowChina(0,104,10);//��
    ShowChina(3,0,11);//��
    ShowChina(3,16,12);//��
    ShowChina(3,32,15);//:
    ShowChina(6,0,6);//��
    ShowChina(6,16,7);//ˮ
    ShowChina(6,32,8);//��
    ShowChina(6,48,15);//:
}

void digplayscreen2() {//��ʾ��2
    ShowChina(0,0,11);//��
    ShowChina(0,16,12);//��
    ShowChina(0,32,16);//��
    ShowChina(0,48,17);//��
    ShowChina(0,64,15);//:
    ShowChina(3,0,6);//��
    ShowChina(3,16,7);//ˮ
    ShowChina(3,32,8);//��
    ShowChina(3,48,16);//��
    ShowChina(3,64,17);//��
    ShowChina(3,80,15);//:
    ShowChina(6,0,18);//��
    ShowChina(6,16,19);//��
    ShowChina(6,32,20);//��
    ShowChina(6,48,21);//��
    ShowChina(6,64,15);//:
}

void digplayOK()
{
    ShowChina(6,80,20);//��
    ShowChina(6,96,21);//��
}
void digplayNOTOK()
{
    ShowChina(6,80,22);//��
    ShowChina(6,96,20);//��
    ShowChina(6,112,21);//��
}

//����ʵʱ��ʾ������������ֵĺ���������x��Ӧ������ʾ�����У�y��Ӧ������ʾ�����У�m��Ӧ����Ҫ��ʾ������
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

////��ʾ���ݵ���ֵ
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