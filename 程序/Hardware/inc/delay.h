#ifndef __DELAY_H__
#define __DELAY_H__

#define uchar unsigned char

void delayms(unsigned int n);//延时nms
void delayus(unsigned int n);//延时10*nus
void delay10ms(void);//延时10ms
void delay5ms(void);//延时5ms
void delay(uchar t);//自定义延时
void delay_us();//延时4-5微秒
#endif