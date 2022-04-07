 #ifndef __LCD_5110_H
 #define __LCD_5110_H

 #include "sys.h"
 #include "delay.h" 
///////////////////////////////////////////////////////
//------------------------------------/				 //
// SCE    ------------PA5  //片选					 //
// RST    ------------PA4  //位选					 //
//  D/C    ------------PA3  // 1写数据，0写指令		 //
// SD     ------------PA2	   //数据				 //
// SCLK   ------------PA1	   //时钟				 //
// LCD    ------------PA0      //背光				 //
///////////////////////////////////////////////////////
 #define res0   PAout(7)=0  //复位,0复位
  #define res1   PAout(7)=1  //复位,0复位
  
#define sce0   PAout(5)=0  //片选
  #define sce1  PAout(5)=1   //片选
 
 #define dc0    PAout(1)=0   //1写数据，0写指令
  #define dc1   PAout(1)=1    //1写数据，0写指令

 #define sdin0  PCout(3)=0   //数据
  #define sdin1  PCout(3)=1   //数据

 #define sclk0  PCout(1)=0  //时钟
 #define sclk1  PCout(1)=1    //时钟
	    
void LCD_WriteByte(unsigned char dt, unsigned char command);
void LCD_Init_5110(void);
void LCD_Set_XY(unsigned char X, unsigned char Y);
void LCD_Clear_5110(void);
void LCD_WriteChar(unsigned char X,unsigned char Y,unsigned char c);
void LCD_WriteString(unsigned char X,unsigned char Y,unsigned char *s);
void LCD_WriteNumber(unsigned char x,unsigned char y, unsigned long number);

#endif
