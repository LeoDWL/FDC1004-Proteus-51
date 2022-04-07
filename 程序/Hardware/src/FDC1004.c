#include <reg52.h>
#include "FDC1004.h"
#include "delay.h"
#include "LCD12864.h"

//void Display(unsigned int c)
//{
//    unsigned char i;
//    unsigned char Disp1[4];
//    i = c/10000;
//    Disp1[0] = digit[i];
//    i = c/1000%10;
//    Disp1[1] = digit[i];
//    i = c/100%10;
//    Disp1[2] = digit[i];
//    i = c/10%10;
//    Disp1[3] = digit[i];
//    for(i=0; i<4; i++)
//    {
//        LCDMEM[i+1] = Disp1[i];
//    }
//}

//void Write_Opm_Float(void)
//{
//    unsigned char *FDC1004_StartAdr=(unsigned char *)0x01000;
//    int *ptr=(int*)0x01000;
////  erase flash
//    FCTL1=0x0A502;            //Erease=1
//    FCTL3=0x0A500;            //LOCK=0
//    *ptr=0;
////
//    FCTL1=0X0A540;            //WRT=1
//    //存储量程
//    *FDC1004_StartAdr=f1.c[0];
//    *(FDC1004_StartAdr+1)=f1.c[1];
//    *(FDC1004_StartAdr+2)=f1.c[2];
//    *(FDC1004_StartAdr+3)=f1.c[3];
//    *(FDC1004_StartAdr+4)=dB_dBm_Flag;
//}
//void Read_Opm_Float(void)
//{
//    unsigned char *FDC1004_StartAdr=(unsigned char*)0x01000;
//    f1.c[0]=*FDC1004_StartAdr;
//    f1.c[1]=*(FDC1004_StartAdr+1);
//    f1.c[2]=*(FDC1004_StartAdr+2);
//    f1.c[3]=*(FDC1004_StartAdr+3);
//    dB_dBm_Flag=*(FDC1004_StartAdr+4);
//}

/*******************************************************************************
IO口模拟IIC，读写FDC1004
********************************************************************************/
/*******************************************************************************
* 函数名        :  FDC1004_Start
* 功能描述	  	:
* 入口函数      :
* 返回值  	  	:
*******************************************************************************/
void FDC1004_Start(void)
{
    FDC1004_SDA_HIG;
    delayus(1);//10*1us
    FDC1004_SCL_HIG;
    delayus(1);//10*1us
    FDC1004_SDA_LOW;
    delayus(1);//10*1us
    FDC1004_SCL_LOW;
}
/*******************************************************************************
* 函数名        :  FDC1004_Start
* 功能描述	  	:
* 入口函数      :
* 返回值  	  	:
*******************************************************************************/
void FDC1004_Stop(void)
{
    FDC1004_SDA_LOW;
    delayus(1);//10*1us
    FDC1004_SCL_HIG;
    delayus(1);//10*1us
    FDC1004_SDA_HIG;
    delayus(1);//10*1us
    FDC1004_SCL_LOW;
}
/*******************************************************************************
* 函数名        :  Answer
* 功能描述	  	:
* 入口函数      :
* 返回值  	  	:
*******************************************************************************/
void Answer(void)
{
    FDC1004_SCL_LOW;
    delayus(1);//10*1us
    FDC1004_SDA_LOW;
    delayus(1);//10*1us
    FDC1004_SCL_HIG;
    delayus(1);//10*1us
    FDC1004_SCL_LOW;
    FDC1004_SDA_HIG;
}
/*******************************************************************************
* 函数名        :  NAnswer
* 功能描述	  	:
* 入口函数      :
* 返回值  	  	:
*******************************************************************************/
void NAnswer(void)
{
    FDC1004_SDA_OUT;
    FDC1004_SDA_HIG;
    delayus(1);//10*1us
    FDC1004_SCL_HIG;
    delayus(1);//10*1us
    FDC1004_SCL_LOW;
}
/*******************************************************************************
* 函数名        :  ACK MASTER
* 功能描述	  	:
* 入口函数      :
* 返回值  	  	:
*******************************************************************************/
unsigned char Ack_Master(void)
{
    FDC1004_SCL_HIG;
    delayus(1);//10*1us
    FDC1004_SDA_IN;
    delayus(1);//10*1us
    if(FDC1004_SDA == 0)
    {
        FDC1004_SCL_LOW;
        FDC1004_SDA_OUT;
        return(0);
    }
    else
    {
        FDC1004_SCL_LOW;
        FDC1004_SDA_OUT;
        return(1);
    }
}
void SAck_Master(void)
{
    FDC1004_SDA_OUT;
    delayus(1);//10*1us
    FDC1004_SDA_LOW;
    delayus(1);//10*1us
    FDC1004_SCL_HIG;
    delayus(1);//10*1us
    FDC1004_SCL_LOW;
}
/*******************************************************************************
* 函数名        :  NACK MASTER
* 功能描述	  	:
* 入口函数      :
* 返回值  	  	:
*******************************************************************************/
unsigned char Nack_Master(void)
{
    delayus(1);//10*1us
    FDC1004_SCL_HIG;
    delayus(1);//10*1us
    FDC1004_SDA_IN;
    if(FDC1004_SDA == 0)
    {
        FDC1004_SCL_LOW;
        FDC1004_SDA_OUT;
        return(0);
    }
    else
    {
        FDC1004_SCL_LOW;
        FDC1004_SDA_OUT;
        return(1);
    }
}
void SNack_Master(void)
{
    FDC1004_SDA_HIG;
    delayus(1);//10*1us
    FDC1004_SCL_HIG;
    delayus(1);//10*1us
    FDC1004_SCL_LOW;
}
/*******************************************************************************
* 函数名        :  RAnswer
* 功能描述	  	:
* 入口函数      :
* 返回值  	  	:
*******************************************************************************/
unsigned char RAnswer(void)
{
    FDC1004_SDA_HIG;
    delayus(1);//10*1us
    FDC1004_SCL_HIG;
    delayus(1);//10*1us
    if(FDC1004_SDA == 0)
    {
        FDC1004_SCL_LOW;
        return(0);
    }
    else
    {
        FDC1004_SCL_LOW;
        return(1);
    }
}
/*******************************************************************************
* 函数名        :  FDC1004_Start
* 功能描述	  	:
* 入口函数      :
* 返回值  	  	:
*******************************************************************************/
void WriteData(unsigned char Data)
{   
    unsigned char i;
    for(i=0; i<8; i++)
    {
        FDC1004_SDA=Data&0x80;
        delayus(1);//10*1us
        FDC1004_SCL_HIG;
        delayus(1);//10*1us
        FDC1004_SCL_LOW;
        Data<<=1;
    }
    delayus(1);//10*1us

    if(RAnswer()==1)
    {
        FDC1004_Stop();
    }
}
/*******************************************************************************
* 函数名        :  FDC1004_Start
* 功能描述	  	:
* 入口函数      :
* 返回值  	  	:
*******************************************************************************/
void Write_Pointer(unsigned char Data)
{   
    unsigned char i;
    FDC1004_SDA_OUT;
    FDC1004_SCL_LOW;
    delayus(1);//10*1us
    for(i=0; i<8; i++)
    {
        FDC1004_SDA=Data&0x80;
        delayus(1);//10*1us
        FDC1004_SCL_HIG;
        delayus(1);//10*1us
        FDC1004_SCL_LOW;
        Data=Data<<1;
    }
    delayus(1);//10*1us
    FDC1004_SDA_LOW;
    delayus(1);//10*1us
    FDC1004_SCL_HIG;
    delayus(1);//10*1us
    FDC1004_SCL_LOW;
    delayus(1);//10*1us
    FDC1004_SDA_HIG;
    delayus(1);//10*1us
    FDC1004_SCL_HIG;
}
void FDC1004_Write_Addr(unsigned char Data)
{
    unsigned char i;
    Data = Data*2;//7位地址补齐成8位
    FDC1004_SDA_OUT;
    FDC1004_SCL_LOW;
    delayus(1);//10*1us
    for(i=0; i<8; i++)
    {
        FDC1004_SDA=Data&0x80;
        delayus(1);//10*1us
        FDC1004_SCL_HIG;
        delayus(1);//10*1us
        FDC1004_SCL_LOW;
        Data=Data<<1;
    }
    delayus(1);//10*1us
    if(RAnswer()==1)
    {
        FDC1004_Stop();
    }
}
void FDC1004_Read_Addr(unsigned char Data)
{
    unsigned char i;
    Data = Data*2+1;//7位地址补齐成8位,最后一位变高
    FDC1004_SDA_OUT;
    FDC1004_SCL_LOW;
    delayus(1);//10*1us
    for(i=0; i<8; i++)
    {
        FDC1004_SDA=Data&0x80;
        delayus(1);//10*1us//10*1us
        FDC1004_SCL_HIG;
        delayus(1);//10*1us
        FDC1004_SCL_LOW;
        Data=Data<<1;
    }
    delayus(1);//10*1us

    if(RAnswer()==1)
    {
        FDC1004_Stop();
    }
}

/*******************************************************************************
* 函数名        :  ReadData
* 功能描述	  	:
* 入口函数      :
* 返回值  	  	:
*******************************************************************************/
unsigned char ReadData(void)
{
    unsigned char tmp;
    unsigned char i;
    FDC1004_SCL_LOW;
    delayus(1);
    FDC1004_SDA_IN;
    for(i=0; i<8; i++)
    {
        FDC1004_SCL_HIG;
        delayus(1);
        tmp = (tmp<<1) | FDC1004_SDA;
        FDC1004_SCL_LOW;
        delayus(1);
    }
    delayus(1);
    return(tmp);
}

void Config_FDC1004(unsigned char result,unsigned char address)//配置FDC1004
{
    unsigned char i;
    for(i=0; i<result; i++)
    {
        FDC1004_Start();
        FDC1004_Write_Addr(0x50);//first write address
        WriteData(address);          // pointer register 0x08
        //Write_Pointer(address);
        WriteData(0x08);          // high bit data
        WriteData(0x00);          //low bit data
        FDC1004_Stop();
        delay5ms();
        address++;
    }
}

//单独配置FDC的每个通道采样
void Config_FDC1004_Single(unsigned int Data,unsigned char address)
{
    unsigned char Data_MSB=0,Data_LSB=0;
    Data_MSB = (Data&0xFF00)>>8;
    Data_LSB = (Data&0x00FF);
    FDC1004_Start();
    FDC1004_Write_Addr(0x50);//first write address
    WriteData(address);          // pointer register 0x08
    //Write_Pointer(address);
    WriteData(Data_MSB);          // high bit data
    WriteData(Data_MSB);          //low bit data
    FDC1004_Stop();
    delay5ms();
}

void Triggering_FDC1004(unsigned char address)//触发FDC1004
{
    FDC1004_Start();
    FDC1004_Write_Addr(0x50);//first write address
    WriteData(address);          // pointer register 0x0C
    //Write_Pointer(address);
    WriteData(0x05);          // high bit data
    WriteData(0xF0);          //low bit data
    FDC1004_Stop();

}
void WaitFor_FDC1004(unsigned char address)//等待测量结束FDC1004
{
    unsigned int rdata;
    unsigned char Data;
    while(1)
    {
        FDC1004_Start();
        FDC1004_Write_Addr(0x50);//first write address
        Write_Pointer(address);          // pointer register 0x0C
        FDC1004_Start();
        FDC1004_Read_Addr(0x50);
        rdata = ReadData();
        SAck_Master();
        //if(Ack_Master()==1)
        //{FDC1004_Stop();}
        Data = ReadData();
        SNack_Master();
        FDC1004_Stop();
        //if(Nack_Master()==1)
        //{FDC1004_Stop();}
        rdata = rdata*256 + Data;
        if((rdata&0x000F)==0x000F)
        {
            break;
        }
        delayms(100);
    }
}
//////////////read from FDC1004 by one byte///////////////
unsigned int ReadID_FDC1004(unsigned char address)               /*单字节*/
{
    unsigned int rdata;
    unsigned char Data;
    FDC1004_Start();
    FDC1004_Write_Addr(0x50);//first write address
    Write_Pointer(address);          // pointer register 0x00
    FDC1004_Start();
    FDC1004_Read_Addr(0x50);
    rdata = ReadData(); 
    SAck_Master(); 
    Data = ReadData();
    SNack_Master();
    FDC1004_Stop(); 
    rdata = rdata*256 + Data;

    return(rdata);
}
//////////////read from FDC1004 by one byte///////////////
unsigned long int ReadData_FDC1004(unsigned char address)               /*单字节*/
{
    unsigned int rdata1,rdata2;
    unsigned long int rdata=0;
    unsigned char Data;
    FDC1004_Start();
    FDC1004_Write_Addr(0x50);//first write address
    Write_Pointer(address);          // pointer register 0x00
    FDC1004_Start();
    FDC1004_Read_Addr(0x50);
    rdata1 = ReadData();
    SAck_Master();
    //if(Ack_Master()==1)
    //{FDC1004_Stop();}
    Data = ReadData();
    SNack_Master();
    FDC1004_Stop();
    //if(Nack_Master()==1)
    //{FDC1004_Stop();}
    rdata = rdata1*256 + Data;

    delayus(10);
    FDC1004_Start();
    FDC1004_Write_Addr(0x50);//first write address
    Write_Pointer(address+1);          // pointer register 0x00
    FDC1004_Read_Addr(0x50);
    rdata2 = ReadData();
    //rdata2 = 0;
    SAck_Master();
    //if(Ack_Master()==1)
    //{FDC1004_Stop();}
    Data = ReadData();
    SNack_Master();
    FDC1004_Stop();
    //if(Nack_Master()==1)
    //{FDC1004_Stop();}
    //rdata2 = rdata2<<8 + Data;
    rdata = rdata*256 + rdata2;
    return(rdata);
}
unsigned int ReadData_Avg(unsigned char result,unsigned char address)
{
    unsigned int avg = 0;
    unsigned char i;
    address = 0x00;
    for(i=0; i<result; i++)
    {
        avg += ReadData_FDC1004(address);
        address +=2;
    }
    return(avg);
}

void Cap_Cac_Diplay(unsigned char lin,unsigned char column,unsigned long int Data)
{
    unsigned char i;
    float CapResult;
    CapResult = (float)Data/(524288.0);//Cap为长整型，FDC1004返回24位有效数值（补码形式），对其/2^19,也就是524288，
                        //也就是右移19位，得到最终的电容数值xx.xxxpf。本工程保留四位小数精度。
    i = (unsigned char)CapResult/10;//十位
    if(i != 0)
        DisP_parameters(lin,column,i);
    else
        _nop_();
    i = (unsigned long)CapResult%10;//个位
    DisP_parameters(lin,column+8,i);
    //小数点
    DisP_parameters(lin,column+16,10);
    i = (unsigned long)(CapResult*10.0)%10;//十分位
    DisP_parameters(lin,column+24,i);
    i = (unsigned long)(CapResult*100.0)%10;//百分位
    DisP_parameters(lin,column+32,i);
    i = (unsigned long)(CapResult*1000.0)%10;//千分位
    DisP_parameters(lin,column+40,i);
    i = (unsigned long)(CapResult*10000.0)%10;//万分位
    DisP_parameters(lin,column+48,i);
    //字符p
    DisP_parameters(lin,column+56,11);
    //字符f
    DisP_parameters(lin,column+64,12);
}

void Moisture_Cac_Diplay(unsigned char lin,unsigned char column,unsigned long int Data)
{
    unsigned char i;
    float CapResult,MoistureResult;
    CapResult = (float)Data/(524288.0);//先转化Cap的值
    MoistureResult = (CapResult-2.1173)*3.14907;//根据论文数据拟合曲线得到含水量和电容的关系
    i = (unsigned char)MoistureResult/10;//十位
    if(i != 0)
        DisP_parameters(lin,column,i);
    else
        _nop_();
    i = (unsigned long)MoistureResult%10;//个位
    DisP_parameters(lin,column+8,i);
    //小数点
    DisP_parameters(lin,column+16,10);
    i = (unsigned long)(MoistureResult*10.0)%10;//十分位
    DisP_parameters(lin,column+24,i);
    i = (unsigned long)(MoistureResult*100.0)%10;//百分位
    DisP_parameters(lin,column+32,i);
    //字符%
    DisP_parameters(lin,column+40,13);
}
