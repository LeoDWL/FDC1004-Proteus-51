#ifndef __HANDSOME_H
#define __HANDSOME_H	     
#include "sys.h"
#include "delay.h"


#define SET_BIT_(x, n)			 (x | (1<<n))
#define CLR_BIT(x, n)			 (x & ~(1<<n))
#define GET_BITS(x, nb, ne)		 ((x & ~(~(0U)<<(ne-nb+1))<<nb) >> nb)
#define SET_BITS(x, nb, ne,set_num)  (x|(( ~(~(0U)<<(ne-nb+1))&set_num)<<nb))



u16 Pow(u16 x ,u8 n);
u16 Community(u16 m,u16 n,u8 mode);
u16 Sqrt(unsigned long M);
u16  is_prime(u16 m);
void Sort(int *a,char n,char mode);
int Find(int *a, int _long, int num);
void Float_part(float x, u16 *a, u16 *b, char _b);
void Int_part(u16 x, u16 *a, u16 *b, char _b);
void Divide(u16 x, u8 n, u16 *p);
u16	fact(u16 num);
float Array_aver(u16 *p, u8 m, u8 n);
float PID(float Kp, float Ki, float Kd, float ActualSpeed, float SetSpeed);
u16 Abs(int x);
u16 Turn(u16 x, u8 n);
u16 Together(u16 a, u16 b);
u16 Abs(int x);
u8 CountNumLength(u32 num);

#endif  



