#include <reg52.h>
					   
#define uchar unsigned char	 

uchar code table[] = "Write:";
uchar code table2[] = "Read :";
uchar code write_data[] = "World";

sbit port_rs = P3 ^ 4;  //����/����ѡ��ˣ�1�����ݣ�0�����״̬��
sbit port_rw = P3 ^ 5;	//��д���ƣ�1�Ƕ���0��д
sbit port_en = P3 ^ 6;	//ʹ�ܶˣ��ߵ�ƽ����

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

	set_lcd(0x80);//��λ������Ϊ��һ�е�һλ
	for(i=0;i<6;i++)
		set_data(table[i]);

	set_lcd(0x80+0x40);//��λ�����õ��ڶ��е�һλ	 
	for(i=0;i<6;i++)
		set_data(table2[i]);

	delay(5);

	/*��оƬ��д���ַ�LXJ���ٴ�оƬ��ȡ������ͬʱ��ʾ��Һ����*/
	for(i=0;*(write_data+i)!='\0';i++)
	{
		set_lcd(0x80+6+i+1);
		set_data(*(write_data+i));	
		write_at_addr(i+1,*(write_data+i));
	
		set_lcd(0x80+0x40+6+i+1);
		set_data( read_at_addr(i+1) );
	}

	//�������ʽ��Ҫ�ں������ʱ
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
	//����Һ��
	set_lcd(0x38);//����16*12��ʾ��5*7����8λ���ݽӿ�
	set_lcd(0x0c);//���ÿ���ʾ������ʾ���
	set_lcd(0x06);//дһ���ַ����ַָ���1
	set_lcd(0x01);//��ʾ��0������ָ����0
	set_lcd(0x0f);//��ʾ��꣬���ù����˸

	//����I2CоƬ
	sda = 1;
	delay_us();
	scl = 1;
	delay_us();
}

/*********************************Һ������***********************************/
//��ʱ
void delay(uchar t)
{				 
	uchar i,j; 
	for(i=0;i<t;i++)
		for(j=0;j<110;j++)
			;
}

//����Һ��������
void set_lcd(uchar val)
{
	port_rs = 0;//���ó�����
	delay(1);
	port_rw = 0;
	delay(1);
	port_en = 1;
	delay(1);
	P2 = val;//��������
	delay(5);
	port_en = 0;
}

//����Һ��������
void set_data(uchar dat)
{	   
	port_rs = 1;//���ó�����
	delay(1);
	port_rw = 0;
	delay(1);
	port_en = 1;
	delay(1);
	P2 = dat;//��������
	delay(5);
	port_en = 0;
}

/*****************************************�洢оƬ����********************************/
//��ʱ4-5΢��
void delay_us()
{
	;;;
}

//��ʼ
void start()
{
	sda = 1;
	delay_us();
	scl = 1;
	delay_us();
	sda = 0;
	delay_us();		
}

//����
void stop()
{
	sda = 0;
	delay_us();
	scl = 1;
	delay_us();
	sda = 1;
	delay_us();
}

//�ȴ�Ӧ��
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

//д����
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

//������
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

//��оƬ��ָ����ַ��д������
void write_at_addr(uchar addr,uchar dat)
{
	start();
	//�趨��ַ��AT24C02�̶���ַΪ1010���ټ����趨�ĵ�ַ��
	//������Ϊ000�����һλ��ʾ����д��0��ʾд������Ϊ1010 0000����0xa0
	write_byte(0xa0);
	respons();//�ȴ�оƬӦ��
	write_byte(addr);//����Ҫд�����ݵĵ�ַ
	respons();//�ȴ�оƬӦ��
	write_byte(dat);//д��Ҫ���������
	respons();//�ȴ�оƬӦ��
	stop();
}

//��ȡоƬָ����ַ������
uchar read_at_addr(uchar addr)
{
	uchar dat;
	start();
	write_byte(0xa0);//��оƬ����һ��д�����ź�
	respons();
	write_byte(addr);//����Ҫ��ȡ�ĵ�ַ
	respons();
	start();
	write_byte(0xa1);//��оƬ����һ���������ź�
	respons();
	dat = read_byte();
	stop();
	return dat;
}