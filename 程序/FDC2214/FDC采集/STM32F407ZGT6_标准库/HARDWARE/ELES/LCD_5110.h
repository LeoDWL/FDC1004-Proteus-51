 #ifndef __LCD_5110_H
 #define __LCD_5110_H

 #include "sys.h"
 #include "delay.h" 
///////////////////////////////////////////////////////
//------------------------------------/				 //
// SCE    ------------PA5  //Ƭѡ					 //
// RST    ------------PA4  //λѡ					 //
//  D/C    ------------PA3  // 1д���ݣ�0дָ��		 //
// SD     ------------PA2	   //����				 //
// SCLK   ------------PA1	   //ʱ��				 //
// LCD    ------------PA0      //����				 //
///////////////////////////////////////////////////////
 #define res0   PAout(7)=0  //��λ,0��λ
  #define res1   PAout(7)=1  //��λ,0��λ
  
#define sce0   PAout(5)=0  //Ƭѡ
  #define sce1  PAout(5)=1   //Ƭѡ
 
 #define dc0    PAout(1)=0   //1д���ݣ�0дָ��
  #define dc1   PAout(1)=1    //1д���ݣ�0дָ��

 #define sdin0  PCout(3)=0   //����
  #define sdin1  PCout(3)=1   //����

 #define sclk0  PCout(1)=0  //ʱ��
 #define sclk1  PCout(1)=1    //ʱ��
	    
void LCD_WriteByte(unsigned char dt, unsigned char command);
void LCD_Init_5110(void);
void LCD_Set_XY(unsigned char X, unsigned char Y);
void LCD_Clear_5110(void);
void LCD_WriteChar(unsigned char X,unsigned char Y,unsigned char c);
void LCD_WriteString(unsigned char X,unsigned char Y,unsigned char *s);
void LCD_WriteNumber(unsigned char x,unsigned char y, unsigned long number);

#endif
