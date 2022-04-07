#ifndef __AD9954_H
#define __AD9954_H	 
#include "sys.h"
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
#define AD9954_CS_H   PFout(0)=1
#define AD9954_CS_L   PFout(0)=0
#define AD9954_CLK_H  PFout(1)=1
#define AD9954_CLK_L  PFout(1)=0
#define AD9954_SDIO_H PFout(2)=1
#define AD9954_SDIO_L PFout(2)=0
#define AD9954_OSK_H  PFout(3)=1
#define AD9954_OSK_L  PFout(3)=0
#define AD9954_PS0_H  PFout(4)=1
#define AD9954_PS0_L  PFout(4)=0
#define AD9954_PS1_H  PFout(5)=1
#define AD9954_PS1_L  PFout(5)=0
#define IO_UPDATE_H   PFout(6)=1
#define IO_UPDATE_L   PFout(6)=0

// Freq factor
#define AD9954_FREQ_PER_KHZ  (10737.418)  // per kHz
#define AD9954_FREQ_PER_HZ   (10.737418)  // per Hz

// Defines of AD9954's Register
#define 	CFR1		0x00
#define 	CFR2 		0x01
#define 	ASF   		0x02
#define 	ARR 		0x03
#define 	FTW0		0x04
#define 	POW0		0x05
#define 	FTW1		0x06
#define 	NLSCW		0x07
#define 	PLSCW		0x08
#define 	RSCW0 		0x07
#define 	RSCW1 		0x08
#define 	RSCW2 		0x09
#define 	RSCW3 		0x0a
#define 	RAM 		0x0b

// Function list
void AD9954Init(void);
void AD9954Send(unsigned char dat);
void SetFreqToHz(unsigned long ulFreq);
void AD9954SetReg8(unsigned char ucRegName, unsigned int uiValue);
void LinearSweepMode(void);
void RAMControlledMode(void);
void RamInit4PS(void);
void RamInit4FS(void);
void PhaseShift(unsigned int uiValue);
void SingleTone(void);

#endif    // !__AD9954_H__