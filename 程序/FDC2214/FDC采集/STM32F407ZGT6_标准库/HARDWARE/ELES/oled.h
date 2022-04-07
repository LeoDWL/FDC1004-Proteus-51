#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	    

//使用spi串行接口
#define OLED_SCLK   PFout(0)
#define OLED_SDIN   PFout(2)
#define OLED_RST    PFout(4)
#define OLED_RS     PFout(6)
#define OLED_CS     PFout(8)
	     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

//OLED控制用函数
u32 mypow(u8 n);
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);  		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size);	 
#endif  
	 







 

