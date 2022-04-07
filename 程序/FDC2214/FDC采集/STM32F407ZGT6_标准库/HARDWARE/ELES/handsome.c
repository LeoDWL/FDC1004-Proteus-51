#include "handsome.h"



//求阶乘,返回x`n
//使用方式:x= Pow(2,3);
//结果:x=8
u16 Pow(u16 x ,u8 n)
{
  u16 num=1;
  while(n--)num*=x;
  return num;
}

//求最大公约数(mode=0)与最小公倍数(mode=1)
//使用方式:x= Community(14,6,1)
//结果:x=42
u16 Community(u16 m, u16 n, u8 mode)
{
	u16 nm, r, t;
	if (m < n)
	{
		t = n; n = m; m = t;
	}
	nm = n*m;
	r = m%n;
	while (r != 0)
	{
		m = n; n = r; r = m%n;
	}
	if (mode == 0) return n;
	else return nm / n;
}

//开根号
//使用方法:x=Sqrt(4);
//结果:x=2;
u16 Sqrt(unsigned long M)
{
	unsigned int N, i;
	unsigned long tmp, ttp;
	if (M == 0)
		return 0;
	N = 0;
	tmp = (M >> 30);
	M <<= 2;
	if (tmp > 1)
	{
		N++;
		tmp -= N;
	}
	for (i = 15; i > 0; i--)
	{
		N <<= 1;
		tmp <<= 2;
		tmp += (M >> 30);
		ttp = N;
		ttp = (ttp << 1) + 1;
		M <<= 2;
		if (tmp >= ttp)
		{
			tmp -= ttp;
			N++;
		}
	}
	return N;
}

//判断是否为素数
//若为素数返回 1，不是则返回 0
//使用方式:x= is_prime(8)
//结果:x=0
u16  is_prime(u16 m)
{
	int i, k;
	k = Sqrt(m);
	for (i = 2; i < k; i++)
	{
		if (m%i == 0) break;
	}
	if (i > k)return 1;
	else return 0;
}

//对有n位的数组进行升序(mode=UP)或降序(mode=DOWN)
//使用方法:a[3]={2,3,1};Sort (a,3,1);
//结果:a[3]={1,2,3};
void Sort(int *a, char n, char mode)
{
	int i, j, imin, s;
	for (i = 0; i < n - 1; i++)
	{
		imin = i;
		for (j = i + 1; j<n; j++)
		{
			if (mode == 1)
			{
				if (a[imin]>a[j]) imin = j;
			}
			else if (mode == 0)
			{
				if (a[imin] < a[j]) imin = j;
			}
		}
		if (i != imin)
		{
			s = a[i]; a[i] = a[imin]; a[imin] = s;
		}
	}
}
//二分法查找某数在数组中第一次出现的位置
//使用方法:a[3]={2,3,1};num = Find(a,3,3);
//结果:num=2;在数组的第二个位置
int Find(int *a, int _long, int num)
{
	int Low = 0;
	int High = _long - 1;
	int Mid;
	Sort(a, _long, 1);
	while (Low <= High)
	{
		Mid = (High + Low) / 2;
		if (a[Mid] > num)
		{
			High = Mid - 1;
		}
		else if (a[Mid] < num)
		{
			Low = Mid + 1;
		}
		else
		{
			return (Mid + 1);
		}
	}
	return 0;
}

//将一个数比如123.3456直接得到整数部分123与小数部分346,如果保留3位小数
//使用方法:float num = 123.3456;Part(num, &num1, &num2, 3);
//结果:num1=123,num2=346;
void Float_part(float x, u16 *a, u16 *b, char _b)
{
	u16 aa, bb;
	aa = x;//123
	x -= aa;//0.3456
	x *= Pow(10, _b);//345.6
	bb = x;//345
	x -= bb;//0.6
	x *= 10;//6
	if ((int)x % 10 >= 5)
	{
		if (_b == 0)aa++;
		else bb ++;//346
	}
	*a = aa;
	*b = bb;
}

//将一个整数分成两部分
//使用方法:num = 3456;Divide(456,&a,&b,2);
//结果:a=4;b=56;
void Int_part(u16 x, u16 *a, u16 *b, char _b)
{
	u16 b_ = Pow(10, _b);
	*b = x%b_;
	*a = x / b_;
}

//将一个整数的每个位按顺序存在一个数组内
//使用方法:num = 456;Divide(456, 3,p);
//结果:p[3]={4,5,6};
void Divide(u16 x, u8 n,u16 *p)
{
	while (n-- > 0)
	{
		p[n] = x % 10;
		x /= 10;
	}
}

//阶乘递归,注意,此函数中0!=0;
//使用方法:num =fact(4);
//结果:num =24;
u16	fact(u16 num)
{
	u16 n = num;
	while (n>1)num *= --n;
	return num;
}

//求数组的平均值
//使用方法:p[4]={1,2,3,4};num =Array_aver(p,1,2);
//结果:num =2.5;
float Array_aver(u16 *p, u8 m, u8 n)
{
	u8 _m = m;
	u16 sum = 0;
	float num;
	while (n+1 - m)sum += p[m++];
	num= sum*1.0 / (n+1-_m);
	return num;
}

//PID自动调节函数,抗干扰能力强
//使用方法:float Kp = 0.5,Ki = 0.3,Kd = 0.05,
//			当前真实速度ActualSpeed = 150,设定速度SetSpeed = 200;
//		    输出的速度ActualSpeed = PID(Kp, Ki, Kd, ActualSpeed, SetSpeed);
//结果:ActualSpeed的值逐渐趋近于200
float PID(float Kp, float Ki, float Kd, float ActualSpeed, float SetSpeed)
{
	static float err = 0.0,err_last = 0.0,voltage = 0.0,integral = 0.0;
	err = SetSpeed - ActualSpeed;
	integral += err;
	ActualSpeed = Kp*err + Ki*integral + Kd*(err - err_last);
	err_last = err;
	return ActualSpeed;
}

//求整数的绝对值
//使用方法:p=-123;num =Abs(p);
//结果:num =123;
u16 Abs(int x)
{
	if (x < 0)return (-x);
	else return x;
}

//求两个数合为一个整数
//使用方法:p=12,q=34;num =Together(p,q);
//结果:num =1234;
u16 Together(u16 a, u16 b)
{
	u8 temp, num;
	for (temp = 0;; temp++)
	{
		num = Pow(10, temp);
		if (b / num == 0)break;
	}
	a *= num;
	return a + b;
}

//求一个整数高低位顺序颠倒
//使用方法:p=1234;num =Turn(p,4);
//结果:num =4321;
u16 Turn(u16 x, u8 n)
{
	u16 sum = 0, num = 0;
	while (n--)
	{
		num = x % 10;
		x /= 10;
		sum += (num*Pow(10, n));
	}
	return sum;
}

//求一个整数有多少位
//使用方法:count=CountNumLength(1234);
//结果:count =4;
u8 CountNumLength(u32 num)
{
	u8 count=0,i=0;
	for (i=0;i<10;i++)
	{
		num/=10;
		count++;
		if(num==0)return count;
	}
	
}
