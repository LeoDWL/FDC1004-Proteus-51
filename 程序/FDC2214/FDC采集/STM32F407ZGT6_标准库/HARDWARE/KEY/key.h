#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h" 

#define KEY1 		PEin(2)   
#define KEY2 		PEin(3)		
#define KEY3 		PEin(4)		
#define WK_UP 		PAin(0)		

#define KEY1_PRES 	1
#define KEY2_PRES	2
#define KEY3_PRES	3
#define WKUP_PRES   4

#define RolKeyInit() ADC1_DMA_MORE() //初始化1s之后再采样判断,DMA启动有速度

typedef struct _RolKey_typedef
{
    char     x                 ;
    char     y                 ;
	char     sta                  ;
	char	 dx					;
	char	 dy					;
	
	
}RolKey_typedef;

void RolKeyScan1(RolKey_typedef * RolKey,u8 mode);
void RolKeyScan2(RolKey_typedef * RolKey,u8 mode);
void KEY_Init(void);	//IO初始化
u8 KEY_Scan(u8);  		//按键扫描函数	

#endif
