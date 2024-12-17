#include<reg51.h>

//lcd display pins 
#define lcd P0 //port 0-data pins
sbit rs=P2^0;
sbit en=P2^1;

//keypad pins
sbit r1=P1^6;
sbit r2=P1^5;
sbit r3=P1^4;
sbit r4=P1^3;
sbit c3=P1^2;
sbit c2=P1^1;
sbit c1=P1^0;


void keypad(void);
void lcd_init(void);
void lcd_cmd(unsigned char);
void lcd_data(unsigned char);
void lcd_string(unsigned char*);
void lcd_delay(void);
void delay(unsigned int);
void wait_ms(void);

const unsigned char password[]="7018"; 
unsigned char pass[]="****";
int index=0;

void main(void)
{
	lcd_init();
	
	EA=1;
	EX0=1;
	IT0=1;
		
	while(1)
	{
		lcd_cmd(0x80);
		lcd_string("Enter password:");
		lcd_cmd(0xc7);
		keypad();
	}
}

void keypad(void)
{
	c1=c2=c3=1;
	r2=r3=r4=1;
	r1=0;
	if(c1==0)
	{
		while(c1==0);
		lcd_data('1');
		if(index<4)
			pass[index]='1';
		index++;
	}
	else if(c2==0)
	{
		while(c2==0);
		lcd_data('2');
		if(index<4)
			pass[index]='2';
		index++;
	}
	else if(c3==0)
	{
		while(c3==0);
		lcd_data('3');
		if(index<4)
			pass[index]='3';
		index++;
	}
	
	//row 2
	r1=1,r2=0;
	if(c1==0)
	{
		while(c1==0);
		lcd_data('4');
		if(index<4)
			pass[index]='4';
		index++;
	}
	else if(c2==0)
	{
		while(c2==0);
		lcd_data('5');
		if(index<4)
			pass[index]='5';
		index++;
	}
	else if(c3==0)
	{
		while(c3==0);
		lcd_data('6');
		if(index<4)
			pass[index]='6';
		index++;
	}
	
	//row 3
	r2=1,r3=0;
	if(c1==0)
	{
		while(c1==0);
		lcd_data('7');
		if(index<4)
			pass[index]='7';
		index++;
	}
	else if(c2==0)
	{
		while(c2==0);
		lcd_data('8');
		if(index<4)
			pass[index]='8';
		index++;
	}
	else if(c3==0)
	{
		while(c3==0);
		lcd_data('9');
		if(index<4)
			pass[index]='9';
		index++;
	}
	
	//row 4
	r3=1,r4=0;
	if(c1==0)
	{
		while(c1==0);
		lcd_data('*');
		if(index<4)
			pass[index]='*';
		index++;
	}
	else if(c2==0)
	{
		while(c2==0);
		lcd_data('0');
		if(index<4)
			pass[index]='0';
		index++;
	}
	else if(c3==0)
	{
		while(c3==0);
		lcd_data('#');
		if(index<4)
			pass[index]='#';
		index++;
	}
}

void lcd_init(void)
{
	lcd_cmd(0x38);
	lcd_cmd(0x0c);
	lcd_cmd(0x06);
	lcd_cmd(0x01);
}

void lcd_cmd(unsigned char ch)
{
	lcd=ch;
	rs=0;
	en=1;
	lcd_delay();
	en=0;
}

void lcd_data(unsigned char ch)
{
	lcd=ch;
	rs=1;
	en=1;
	lcd_delay();
	en=0;
}

void lcd_string(unsigned char *ch)
{
	while(*ch)
		lcd_data(*ch++);
}

void lcd_delay(void)
{
	unsigned int i;
	for(i=0;i<1000;i++);
}

void delay(unsigned int n)
{
	unsigned int i=0;
	for(i=0;i<n;i++)
	wait_ms();
}

void wait_ms(void)
{
	TMOD=0x01;//timer 0 mode 1
	
	TH0=0xFC;
	TH1=0x66;
	
	TR0=1;//start timer 0
	while(TF0==0);
	TR0=0;
	TF0=0;
}

void External0_ISR(void) interrupt 0
{
	unsigned char *p,*u;
	int i;
	p=password;
	u=pass;
	if(index!=4)
	{
		lcd_cmd(0x80);
		lcd_string("Access Denied..");
		lcd_cmd(0xc0);
		lcd_string("Wait for ");
		for(i=5;i>=0;i--)
		{
			lcd_cmd(0xc9);
			lcd_data(i+'0');
			lcd_string(" sec..");
			delay(990);
		}
		for(i=0;i<4;i++)
		pass[i]='*';
		index=0;
		lcd_cmd(0x01);
		return;
	}
		
	while(*p && *u )
	{
		if(*p!=*u)
		{
			lcd_cmd(0x80);
			lcd_string("Access Denied..");
			lcd_cmd(0xc0);
			lcd_string("Wait for ");
			for(i=5;i>=0;i--)
			{
				lcd_cmd(0xc9);
				lcd_data(i+'0');
				lcd_string(" sec..");
				delay(950);
			}
			for(i=0;i<4;i++)
	    pass[i]='*';
	    index=0;
			lcd_cmd(0x01);
			return;
		}
		p++;
		u++;
	}
	lcd_cmd(0x80);
	lcd_string("Access Granted...");
	lcd_cmd(0xc0);
	lcd_string("Locks in ");
	for(i=5;i>=0;i--)
	{
		lcd_cmd(0xc9);
		lcd_data(i+'0');
		lcd_string(" sec..");
		delay(950);
	}
	for(i=0;i<4;i++)
	pass[i]='*';
	index=0;
	lcd_cmd(0x01);
}