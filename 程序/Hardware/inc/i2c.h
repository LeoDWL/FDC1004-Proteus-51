#ifndef __I2C_H__
#define __I2C_H__

#include "delay.h"

//i2cÒı½Å¶¨Òå
sbit scl = P2 ^ 3;
sbit sda = P2 ^ 4;

void start();
void stop();
void respons();
void write_byte(uchar dat);
uchar read_byte();
void write_at_addr(uchar addr,uchar dat);
uchar read_at_addr(uchar addr);

#endif