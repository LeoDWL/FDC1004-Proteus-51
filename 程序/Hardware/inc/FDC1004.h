#ifndef __FDC1004_H__
#define __FDC1004_H__

#include "i2c.h"

#define FDC1004_SDA         sda
#define FDC1004_SCL         scl

#define FDC1004_SDA_IN      sda = 1//为了保证读取的时候正确，需要先让IO输出1
#define FDC1004_SDA_OUT     sda = 0

#define FDC1004_SDA_HIG     sda = 1
#define FDC1004_SCL_HIG     scl = 1

#define FDC1004_SDA_LOW     sda = 0
#define FDC1004_SCL_LOW     scl = 0

#define NO_KEY           0
#define ON_OFF_DOWN      1
#define ON_OFF_UP        2
#define BL_SET_DOWN      3
#define BL_SET_UP        4
#define STORE_SET_DOWN   5
#define STORE_SET_UP     6
#define UPLOAD_SET_DOWN  7
#define UPLOAD_SET_UP    8
#define ZERO_SET_DOWN    9
#define ZERO_SET_UP      10
#define ENTER_SET_DOWN   11
#define ENTER_SET_UP     12

float absfloat(float Data);
/////////////////////////FDC1004 IIC////////////////////////////
void FDC1004_Start(void);
void FDC1004_Stop(void);
void Answer(void);
void NAnswer(void);
unsigned char Ack_Master(void);
unsigned char Nack_Master(void);
void SAck_Master(void);
void SNack_Master(void);
unsigned char RAnswer(void);
void WriteData(unsigned char Data);
void Write_Pointer(unsigned char Data);
void FDC1004_Write_Addr(unsigned char Data);
void FDC1004_Read_Addr(unsigned char Data);
unsigned char ReadData(void);
void Config_FDC1004(unsigned char result,unsigned char address);
void Config_FDC1004_Single(unsigned int Data,unsigned char address);//单独配置FDC的每个通道采样
void Triggering_FDC1004(unsigned char address);
void WaitFor_FDC1004(unsigned char address);
unsigned int ReadID_FDC1004(unsigned char address);
unsigned long int ReadData_FDC1004(unsigned char address);
unsigned int ReadData_Avg(unsigned char result,unsigned char address);
void Cap_Cac_Diplay(unsigned char lin,unsigned char column,unsigned long int Data);
void Moisture_Cac_Diplay(unsigned char lin,unsigned char column,unsigned long int Data);
void Cap_accuracy_Diplay(unsigned char lin,unsigned char column,unsigned long int Cap_Data,float Set_Cap_Data);//电容精度显示
float Moisture_accuracy_Diplay(unsigned char lin,unsigned char column,unsigned long int Cap_Data,float Set_Moisture_Data);//含水率精度显示

#endif
