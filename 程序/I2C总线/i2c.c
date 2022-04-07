#include <reg52.h>
					   
#define uchar unsigned char	 

uchar code table[] = "Write:";
uchar code table2[] = "Read :";
uchar code write_data[] = "World";

sbit port_rs = P3 ^ 4;  //数据/命令选择端，1是数据，0是命令（状态）
sbit port_rw = P3 ^ 5;	//读写控制，1是读，0是写
sbit port_en = P3 ^ 6;	//使能端，高电平允许

sbit scl = P1 ^ 0;
sbit sda = P1 ^ 1;

void delay(uchar t);
void set_lcd(uchar val);
void set_data(uchar dat);
void write_at_addr(uchar addr,uchar dat);
uchar read_at_addr(uchar addr);
void init();
void delay_us();

void main()
{
	uchar i;
	init();

	set_lcd(0x80);//把位置设置为第一行第一位
	for(i=0;i<6;i++)
		set_data(table[i]);

	set_lcd(0x80+0x40);//把位置设置到第二行第一位	 
	for(i=0;i<6;i++)
		set_data(table2[i]);

	delay(5);

	/*在芯片中写入字符LXJ，再从芯片读取出来，同时显示在液晶上*/
	for(i=0;*(write_data+i)!='\0';i++)
	{
		set_lcd(0x80+6+i+1);
		set_data(*(write_data+i));	
		write_at_addr(i+1,*(write_data+i));
	
		set_lcd(0x80+0x40+6+i+1);
		set_data( read_at_addr(i+1) );
	}

	//下面的形式，要在后面加延时
	/*set_lcd(0x80+7);
	set_data('H');	
	set_data('e');
	set_data('l');
	set_data('l');
	set_data('o');
	write_at_addr(1,'H'); delay(5); 
	write_at_addr(2,'e'); delay(5);
	write_at_addr(3,'l'); delay(5);
	write_at_addr(4,'l'); delay(5);
	write_at_addr(5,'o'); delay(5);

	set_lcd(0x80+0x40+7);
	set_data( read_at_addr(1) );  
	set_data( read_at_addr(2) );
	set_data( read_at_addr(3) );
	set_data( read_at_addr(4) );
	set_data( read_at_addr(5) ); */

	while(1);
}

void init()
{	
	//设置液晶
	set_lcd(0x38);//设置16*12显示，5*7点阵，8位数据接口
	set_lcd(0x0c);//设置开显示，不显示光标
	set_lcd(0x06);//写一个字符后地址指针加1
	set_lcd(0x01);//显示清0，数据指针清0
	set_lcd(0x0f);//显示光标，并让光标闪烁

	//设置I2C芯片
	sda = 1;
	delay_us();
	scl = 1;
	delay_us();
}

/*********************************液晶部分***********************************/
//延时
void delay(uchar t)
{				 
	uchar i,j; 
	for(i=0;i<t;i++)
		for(j=0;j<110;j++)
			;
}

//设置液晶屏参数
void set_lcd(uchar val)
{
	port_rs = 0;//设置成命令
	delay(1);
	port_rw = 0;
	delay(1);
	port_en = 1;
	delay(1);
	P2 = val;//设置命令
	delay(5);
	port_en = 0;
}

//设置液晶屏数据
void set_data(uchar dat)
{	   
	port_rs = 1;//设置成数据
	delay(1);
	port_rw = 0;
	delay(1);
	port_en = 1;
	delay(1);
	P2 = dat;//设置数据
	delay(5);
	port_en = 0;
}

/*****************************************存储芯片部分********************************/
//延时4-5微秒
void delay_us()
{
	;;;
}

//开始
void start()
{
	sda = 1;
	delay_us();
	scl = 1;
	delay_us();
	sda = 0;
	delay_us();		
}

//结束
void stop()
{
	sda = 0;
	delay_us();
	scl = 1;
	delay_us();
	sda = 1;
	delay_us();
}

//等待应答
void respons()
{
	uchar i;
	scl = 1;
	delay_us();

	while((sda==1) && (i<250))
		i++;
		
	scl = 0;
	delay_us(); 
}

//写数据
void write_byte(uchar dat)
{
	uchar i,temp;
	temp = dat;
	for(i=0;i<8;i++)
	{
		temp = temp << 1;
		scl = 0;
		delay_us();
		sda = CY;
		delay_us();
		scl = 1;
		delay_us();
	}

	scl = 0;
	delay_us();
	sda = 1;
	delay_us();
}

//读数据
uchar read_byte()
{
	uchar i,k;
	scl = 0;
	delay_us();
	sda = 1;
	delay_us();
	for(i=0; i<8; i++)
	{
		scl = 1;
		delay_us();
		k = (k<<1) | sda;
		scl = 0;
		delay_us();
	}
	return k;
}

//向芯片的指定地址，写入数据
void write_at_addr(uchar addr,uchar dat)
{
	start();
	//设定地址，AT24C02固定地址为1010，再加上设定的地址，
	//我设置为000，最后一位表示读或写，0表示写，所以为1010 0000，即0xa0
	write_byte(0xa0);
	respons();//等待芯片应答
	write_byte(addr);//设置要写入数据的地址
	respons();//等待芯片应答
	write_byte(dat);//写入要保存的数据
	respons();//等待芯片应答
	stop();
}

//读取芯片指定地址的数据
uchar read_at_addr(uchar addr)
{
	uchar dat;
	start();
	write_byte(0xa0);//向芯片发送一个写操作信号
	respons();
	write_byte(addr);//设置要读取的地址
	respons();
	start();
	write_byte(0xa1);//向芯片发送一个读操作信号
	respons();
	dat = read_byte();
	stop();
	return dat;
}