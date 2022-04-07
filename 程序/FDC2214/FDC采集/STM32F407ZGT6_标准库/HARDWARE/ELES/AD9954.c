#include "AD9954.h"
#include "delay.h"


//double fH_Num=11.2204;
//double fH_Num=10.73741824;
//double fH_Num=11.3671588397205;//
//double fH_Num = 11.3025455157895;
//double fH_Num=17.17986918;
/*
** 函数名称 ：void GPIO_AD9954_Init(void)
** 函数功能 ：AD9954接口IO初始化
**            AD9954_CS----------PF13	 		OUT
**	      		AD9954_SCLK--------PF12	 		OUT
**            AD9954_SDIO--------PF11 	  OUT
**            AD9954_OSK---------PB2  	 	OUT
**          	PS0----------------PB1			OUT
**	      		PS1----------------PB0	 		OUT
**            IOUPDATE-----------PB15	  	OUT

**	      		AD9954_SDO---------PF14	  	IN
**            AD9954_IOSY--------PF15	  	OUT
**            AD9954_RES---------PG0    	OUT
**            AD9954_PWR---------PG1    	OUT
*/
// Init of AD9954
void AD9954Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOF时钟

  //GPIOF9,F10初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//上拉
  GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化
	
GPIO_ResetBits(GPIOF,GPIO_Pin_0 |GPIO_Pin_1 |GPIO_Pin_2 |GPIO_Pin_3 |GPIO_Pin_4 |GPIO_Pin_5 |GPIO_Pin_6 );//GPIOF9,F10设置高，灯灭

}

void AD9954Update(void)
{
	IO_UPDATE_L;
	//delay_us(10);
	IO_UPDATE_H;
	//delay_us(5);
	//IO_UPDATE_L;
}

// Send one byte to AD9954
void AD9954Send(unsigned char dat)
{
    unsigned char i;
    for(i=0; i<8; ++i)
    {
        AD9954_CLK_L;
        if(dat & 0x80){
        	AD9954_SDIO_H;
        }
        else {
        	AD9954_SDIO_L;
        }

		AD9954_CLK_H;
		dat = dat << 1;
		AD9954_CLK_L;
    }
}

// 设置单正弦波的频率
// ulFreq -- 要设置的频率，单位为Hz
void SetFreqToHz(unsigned long ulFreq)
{
	unsigned long ulRegValue;   //
	unsigned char ucTemp;
	int i = 0;
	
	ulRegValue = (unsigned long)(AD9954_FREQ_PER_HZ * ulFreq);  // 单位由 乘数 宏AD9954_FREQ_PER_HZ 决定
	
	AD9954_CS_L;
	
	AD9954Send(FTW0);         // FTW0 (0x ff ff ff ff ==> 400MHz)
	for(i=3; i >= 0; --i){
		ucTemp = (unsigned char)((ulRegValue >> (8*i)) & 0x000000ff);
		AD9954Send(ucTemp);
	}
	
	AD9954_CS_H;
	AD9954Update();
}

// 设置AD9954的一个八位寄存器的值
// ucRegName - 寄存器的名字， uiVaue - 要设置的值
void AD9954SetReg8(unsigned char ucRegName, unsigned int uiValue)
{
	AD9954_CS_L; 
	AD9954Send(ucRegName);

	AD9954Send((unsigned char)((uiValue >> 8) & 0x00ff));
	AD9954Send((unsigned char)(uiValue & 0x00ff));

	AD9954_CS_H;
	AD9954Update();
}

// 线性扫描模式
void LinearSweepMode(void)
{
	AD9954_CS_L;
	AD9954Send(CFR1);
	AD9954Send(0x00);
	AD9954Send(0x20);
	AD9954Send(0x02);
	AD9954Send(0x00);

	AD9954Send(FTW1);
	AD9954Send(0x19);
	AD9954Send(0x99);
	AD9954Send(0x99);
	AD9954Send(0x9A);

	AD9954Send(NLSCW);
	AD9954Send(0x0ff);
	AD9954Send(0x00);
	AD9954Send(0x00);
	AD9954Send(0x00);
	AD9954Send(0x9f);

	AD9954Send(PLSCW);
	AD9954Send(0xff);
	AD9954Send(0x00);
	AD9954Send(0x00);
	AD9954Send(0x00);
	AD9954Send(0x09);

	AD9954_CS_H;
	delay_us(10);
	IO_UPDATE_L;
	delay_us(10);
	IO_UPDATE_H;
	delay_us(10);
	IO_UPDATE_L;
}

// RAM 模式
void RAMControlledMode(void)
{
	AD9954_PS0_L;
	AD9954_PS1_L;
	
//	AD9954_CS_L;
//	AD9954Send(CFR1);
	// RAM phase
//	AD9954Send(0xC0);   // 1100  <31><30> = 11  RAM enable and Destination bit is set to Phase Adder
//	AD9954Send(0x00);   // <21> is cleared, inactive other fitures by default 
//	AD9954Send(0x02);   // <8>=1 MSB first <9>=1 SDIO as Input only(3-line Mode)  
//	AD9954Send(0x42);   // <1>=1 SYNC_CLK inactive <5>=0 DAC is enable <6>=1 comparator is disable 
	//AD9954Send(0x02);   //bit6 comp PD,bit1 SYNC_clk Disable;SYNC_clk = DDSclk/4;
//	 RAM frequence
//	AD9954Send(0x80);
//	AD9954Send(0x00);
//	AD9954Send(0x02);
//	AD9954Send(0x00);
//	AD9954Update();

	AD9954Send(RSCW0);
	AD9954Send(0xff);
	AD9954Send(0xff);
	AD9954Send(0x00);
	AD9954Send(0x00);
	AD9954Send(0x00);
	
	AD9954Send(RSCW1);
	AD9954Send(0xff);   // 24 -39 扫描RAM地址时停留SYNC_CLK的周期数：1-65535
	AD9954Send(0xff);   
	AD9954Send(0x00);   // <23:16> <9:8>： Final address
	AD9954Send(0x01);   // <15:10> = 0000_00
	AD9954Send(0x04);   // <3:0> = 0100; <3:0> <15:10>:  Start address

	AD9954Send(RSCW2);
	AD9954Send(0xff);
	AD9954Send(0xff);
	AD9954Send(0x00);
	AD9954Send(0x02);
	AD9954Send(0x08);

	AD9954Send(RSCW3);
	AD9954Send(0xff);
	AD9954Send(0xff);
	AD9954Send(0x00);
	AD9954Send(0x03);
	AD9954Send(0x0c);

	AD9954_CS_H;
	AD9954Update();
}

void RamInit4PS(void)
{
	AD9954_PS0_L; // <PS1:PS0> = 00
	AD9954_PS1_L;
	
	AD9954_CS_L;  // Write Enable
	AD9954Send(CFR1); // RAM phase
	AD9954Send(0xC0); // 1100  <31><30> = 11 <30>=1: RAM输出到相位偏移器，实现相移
	AD9954Send(0x00); // <21> is cleared, inactive other fitures by default 
	AD9954Send(0x02); // <8>=1 MSB first <9>=1 SDIO as Input only(3-line Mode)  
	AD9954Send(0x42); // <1>=1 SYNC_CLK inactive <5>=0 DAC is enable <6>=1 comparator is disable 
	AD9954_CS_H;
	AD9954Update();
	
	AD9954_CS_L;  // Write Enable
	AD9954Send(RAM);
	// Phase 0° 0x00_00_00_00
	AD9954Send(0x00);
	AD9954Send(0x00);
	AD9954Send(0x00);
	AD9954Send(0x00);
	AD9954_CS_H;
	AD9954Update();

	AD9954_PS0_H; //<PS1:PS0> = 01
	AD9954_PS1_L;
	AD9954_CS_L;
	AD9954Send(RAM);
	// Phase 90° 0x00_00_00_00
	AD9954Send(0x40);
	AD9954Send(0x00);
	AD9954Send(0x00);
	AD9954Send(0x00);
	AD9954_CS_H;
	AD9954Update();

	AD9954_PS0_L;  //<PS1:PS0> = 10
	AD9954_PS1_H;
	AD9954_CS_L;
	AD9954Send(RAM);
	// Phase 180° 0x00_00_10_00
	AD9954Send(0x80);
	AD9954Send(0x00);
	AD9954Send(0x00);
	AD9954Send(0x00);
	AD9954_CS_H;
	AD9954Update();

	AD9954_PS0_H; //<PS1:PS0> = 11
	AD9954_PS1_H;
	AD9954_CS_L;
	AD9954Send(RAM);
	// Phase 270° 0x00_00_18_00
	AD9954Send(0xc0);
	AD9954Send(0x00);
	AD9954Send(0x00);
	AD9954Send(0x00);
	AD9954_CS_H;
	AD9954Update();
}

void RamInit4FS(void)
{
	AD9954_PS0_L;
	AD9954_PS1_L;
	
	AD9954_CS_L;  // Write Enable
	AD9954Send(CFR1); // Set CFR1
	AD9954Send(0x80); // RAM frequence, <30>=0:RAM输出到相位累计器，实现频移
	AD9954Send(0x00);
	AD9954Send(0x02);
	AD9954Send(0x00);
	AD9954_CS_H;
	AD9954Update();
	
	AD9954_CS_L;
	AD9954Send(RAM);
	// 10MHz 0x06_66_66_66
	AD9954Send(0x06);
	AD9954Send(0x66);
	AD9954Send(0x66);
	AD9954Send(0x66);
	AD9954_CS_H;
	AD9954Update();

	AD9954_PS0_H;
	AD9954_PS1_L;
	AD9954_CS_L;
	AD9954Send(RAM);
	// 20MHz 0x0C_CC_CC_CD
	AD9954Send(0x0C);
	AD9954Send(0xCC);
	AD9954Send(0xCC);
	AD9954Send(0xCD);
	AD9954_CS_H;
	AD9954Update();

	AD9954_PS0_L;
	AD9954_PS1_H;
	AD9954_CS_L;
	AD9954Send(RAM);
	// 30MHz 0x13_33_33_33
	AD9954Send(0x13);
	AD9954Send(0x33);
	AD9954Send(0X33);
	AD9954Send(0x33);
	AD9954_CS_H;
	AD9954Update();

	AD9954_PS0_H;
	AD9954_PS1_H;
	AD9954_CS_L;
	AD9954Send(RAM);
	// 40MHz 0x19_99_99_9A
	AD9954Send(0x19);
	AD9954Send(0x99);
	AD9954Send(0x99);
	AD9954Send(0x9A);
	AD9954_CS_H;
	AD9954Update();
}

// 移相
void PhaseShift(unsigned int uiValue)
{
	AD9954_CS_L;          //POW0.Phase shift (0x 3f ff ==> 360)
	AD9954Send(POW0);

	AD9954Send((unsigned char)((uiValue >> 8) & 0x00ff));
	AD9954Send((unsigned char)(uiValue & 0x00ff));

	AD9954_CS_H;
	AD9954Update();
}

// 单一正弦
void SingleTone(void)
{
	AD9954_CS_L;

	AD9954Send(CFR1);        //CFR1  0-7 8-15 16-23 24-31
	AD9954Send(0x02);        //CFR<25> bit1 OSK Enable,bit0 Auto OSK Enable;  // 调节幅度 <25:24>要置一
	AD9954Send(0x00);        // CFR1<4> 使用片上晶振 and CFR2<7:3> 倍频倍数
	AD9954Send(0x02);
	AD9954Send(0x00);        //bit6 comp PD,bit1 SYNC_clk Disable;SYNC_clk = DDSclk/4;
	AD9954Update();

	AD9954Send(CFR2);         //CFR2  0-7 8-15 16-23
	AD9954Send(0x00);         //not used;
	AD9954Send(0x00);         //bit3 High Speed SYNC Enable;
	AD9954Send(0x84);         //<7:3> = 0001 0000 = 16，16*25MHz = 400MHz
	AD9954Update();

	AD9954Send(ASF);          //ASF,when OSK Enabled(CFR1 bit25);
    AD9954Send(0x3f);         //<15:14> 01 -> 2倍, <13:0> 3fff
    AD9954Send(0xff);

	AD9954Send(FTW0);         // FTW0 1MHz (0x ff ff ff ff ==> 400MHz)
	AD9954Send(0x00);         
	AD9954Send(0xa3);
    AD9954Send(0xd7);
    AD9954Send(0x0a);

	AD9954_CS_H;
	AD9954Update();
}

