#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "sys.h"
#include "delay.h"

//关于OLED显示屏
	//OLED 模块的特点是低功耗,画面清晰,驱动简单的特点,控制器是 SSD1306，
	//接口模式可由BS1,BS2调节,若使用spi模式,则都作接地处理.

//接线除去VC,GND还有五根线
	//CS		 - PD6：	OLED 片选信号,即芯片使能信号
	//RST(RES)   - PG15：	硬复位 OLED。
	//DC		 - PD3：	命令/数据标志（0，读写命令；1，读写数据）。
	//SCLK		 - PC0：	串行时钟线,D0 
	//SDIN 		 - PC1：	串行数据线,D1 

//程序设计思路:
	//1,设置 STM32 与 OLED  模块相连接的 IO
	//2,初始化 OLED,开启显示,清空显存 
	//3,通过函数将字符和数字更改OLED_GRAM数组
	//4,将OLED_GRAM数组刷到 OLED 显存上,屏幕自动一直读显存,点亮屏幕

//关于显示函数
	//先创建一个显示单个点的函数,之后的"显示字符"以画点函数为基石,而实现功能
	//再后面的"显示数字""显示字符串"等都是以"显示字符"为基础进行显示

//关于显存
	//OLED自带一帧画面128*64的显存,列不变,每八行结合为一个8位的数据,
	//OLED的显存按照某一顺序有128*8个字节,又因为使用spi模式只能写入
	//不能读取,所以无法实现精确画点的功能,所以解决办法是创建一个[128][8]
	//数组,每次操作都是对数组操作,然后将数据重刷到显存里面,不过这种方法不太
	//适用小存储量单片机如51单片机

//常用命令
//连续两个字节决定一个命令,第一个字节相当于地址,第二个字节表示要执行的命令
	//0X81,随后写入的一个8位数据决定亮度,值越大屏幕越亮。
	//0X8D,随后写入的0x14/0x10决定电荷泵的开启/关闭。模块初始化的时候，必须选择开启
//单独一个字节就是命令,前四位表示地址,后四位表示命令
	//0XAE/0XAF,0XAE 为关闭显示命令；0XAF 为开启显示命令
	//0XB0~B7，用于设置页地址，可表示一共8页,列一共64个像素,每8位像素合在一起为一页
	//0X00~0X0F，该指令用于设置显示时的起始列地址低四位。
	//0X10~0X1F，该指令用于设置显示时的起始列地址高四位。两条配合使用可表示一共128列

u8 OLED_GRAM[128][8];	//128行,8列,一个数据包含8个像素
//写入的时候一列一列地写进显存
	//		0	1	2	3	4	5	6	7 
	//0		F	A	F	A	F	A	F	A
	//1		A	F	A	F	A	F	A	F
	//2		F	A	F	A	F	A	F	A
	//3		A	F	A	F	A	F	A	F
	//4		A	F	A	F	A	F	A	F
	//5		A	F	A	F	A	F	A	F
	//6		A	F	A	F	A	F	A	F
	//7		A	F	A	F	A	F	A	F
	//,,, ,,	
	//126 A	F	A	F	A	F	A	F
	//127 F	A	F	A	F	A	F	A

//将数组的所有数据刷新到显存里面,因为屏幕只能读取显存里面的数据
void OLED_Refresh_Gram(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		
		OLED_WR_Byte (0x00,OLED_CMD);
		OLED_WR_Byte (0x10,OLED_CMD);     
//这两个数据各取低四位,组成一个8位数据,表示0~127,决定下面写入数据的起始地址
		
		for(n=0;n<128;n++)
			OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA);
	}   
}

//向SSD1306写入数据或执行命令
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
//为了增强可读性,已经做过头定义处理
//#define OLED_CMD  0	设置配置的时候用"命令"
//#define OLED_DATA 1	 对显存写入数据的时候用"数据"
void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	u8 i;			  
	OLED_RS=cmd; 
	OLED_CS=0;		  
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK=0;
		if(dat&0x80)OLED_SDIN=1;
		else OLED_SDIN=0;
		OLED_SCLK=1;
		dat<<=1;   
	}				 
	OLED_CS=1;		  
	OLED_RS=1;   	  
} 
	  	  
//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //即将设置电荷泵
	OLED_WR_Byte(0X14,OLED_CMD);  //将电荷泵打开(接通电源)
	OLED_WR_Byte(0XAF,OLED_CMD);  //开启显示
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //即将设置电荷泵
	OLED_WR_Byte(0X10,OLED_CMD);  //将电荷泵关闭(关闭电源)
	OLED_WR_Byte(0XAE,OLED_CMD);  //关闭显示
}		   			

//清屏函数,将数组的数据全部写入0,屏幕全部熄灭
void OLED_Clear(void)  
{  
	u8 i,n;  
	for(i=0;i<8;i++)for(n=0;n<128;n++)OLED_GRAM[n][i]=0X00;  
	OLED_Refresh_Gram();//更新显示
}


//一个通用的在点（x，y）置 1 表达式为：OLED_GRAM[x][7-y/8]|=1<<(7-y%8)；
//画点 
//x:0~127
//y:0~63
//t:1 点亮 ,0 熄灭该点				   
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;//超出范围了.
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	    
}

//x1,y1,x2,y2 填充区域的对角坐标
//dot:0,清空;1,填充	  
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  
{  
	u8 x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}													    
	OLED_Refresh_Gram();//设置完立即更新显示
}

//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 12/16/24

//如果选择的12号字体,那么数据是12*6个像素,有12个字节,
	//依次操作时,第一个字节都是像素,第二个字节只有高4位
	//是像素,低四位无关就都是零,然后切换到第二列,,,一共有6列
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数
	chr=chr-' ';//得到偏移后的值		 
    for(t=0;t<csize;t++)
    {   
		if(size==12)temp=asc2_1206_[chr][t]; 	 	//调用1206字体
		else if(size==16)temp=asc2_1608_[chr][t];	//调用1608字体
		else if(size==24)temp=asc2_2412_[chr][t];	//调用2412字体
		else return;							
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
//对temp的最高位进行打点操作,打完之后,将次高位向左移移到最高位,循环操作直到8个点打完
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)//可以使无效像素的"0"被忽略掉
			{
				y=y0;
				x++;
				break;//执行完了之后跳出循环,对下一个字节进行打点
			}
		}  	 
    }
}

//计算10^n函数
u32 mypow(u8 n)
{
	u32 result=1;	 
	while(n--)result*=10;    
	return result;
}				  

//显示数字
//x,y :起点坐标	 
//len : 的位数
//size: 大小
//num:数值(0~4294967295);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(len-t-1))%10;//得到当前要显示的某一位,从高位依次下降
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size,1);//如果位数过少，则向右对齐
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1); //将当前位用字符的形式表达出来
	}
	OLED_Refresh_Gram();
} 
//显示字符串
//x,y:起点坐标  
//size:字体大小 
//*p:字符串起始地址 
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size)
{	
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符
    {       
        if(x>(128-(size/2)))
					{x=0;y+=size;}
        if(y>(64-size))
					{y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,size,1);	 
        x+=size/2;
        p++;
    }  
	OLED_Refresh_Gram();
}	   

//初始化SSD1306					    
void OLED_Init(void)
{ 	
    FAST_INIT(PF,0,PP);
	FAST_INIT(PF,2,PP);
	FAST_INIT(PF,4,PP);
	FAST_INIT(PF,6,PP);
	FAST_INIT(PF,8,PP);

	OLED_CS=1;
	OLED_RS=1;	 
	
	OLED_RST=0;
	delay_ms(100);
	OLED_RST=1; 
	
//以下是厂家提供的默认初始化配置,不需要弄清楚细节	
//***********************************************************
	OLED_WR_Byte(0xAE,OLED_CMD); //关闭显示
	OLED_WR_Byte(0xD5,OLED_CMD); //设置时钟分频因子,震荡频率
	OLED_WR_Byte(80,OLED_CMD);   //[3:0],分频因子;[7:4],震荡频率
	OLED_WR_Byte(0xA8,OLED_CMD); //设置驱动路数
	OLED_WR_Byte(0X3F,OLED_CMD); //默认0X3F(1/64) 
	OLED_WR_Byte(0xD3,OLED_CMD); //设置显示偏移
	OLED_WR_Byte(0X00,OLED_CMD); //默认为0

	OLED_WR_Byte(0x40,OLED_CMD); //设置显示开始行 [5:0],行数.
													    
	OLED_WR_Byte(0x8D,OLED_CMD); //电荷泵设置
	OLED_WR_Byte(0x14,OLED_CMD); //bit2，开启/关闭
	OLED_WR_Byte(0x20,OLED_CMD); //设置内存地址模式
	OLED_WR_Byte(0x02,OLED_CMD); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
	OLED_WR_Byte(0xA1,OLED_CMD); //段重定义设置,bit0:0,0->0;1,0->127;
	OLED_WR_Byte(0xC0,OLED_CMD); //设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
	OLED_WR_Byte(0xDA,OLED_CMD); //设置COM硬件引脚配置
	OLED_WR_Byte(0x12,OLED_CMD); //[5:4]配置
		 
	OLED_WR_Byte(0x81,OLED_CMD); //对比度设置
	OLED_WR_Byte(0xEF,OLED_CMD); //1~255;默认0X7F (亮度设置,越大越亮)
	OLED_WR_Byte(0xD9,OLED_CMD); //设置预充电周期
	OLED_WR_Byte(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
	OLED_WR_Byte(0xDB,OLED_CMD); //设置VCOMH 电压倍率
	OLED_WR_Byte(0x30,OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	OLED_WR_Byte(0xA4,OLED_CMD); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
	OLED_WR_Byte(0xA6,OLED_CMD); //设置显示方式;bit0:1,反相显示;0,正常显示	    						   
	OLED_WR_Byte(0xAF,OLED_CMD); //开启显示	 
//************************************************************

	OLED_Clear();
//将显存全部写入零,即熄灭屏幕
}  





























