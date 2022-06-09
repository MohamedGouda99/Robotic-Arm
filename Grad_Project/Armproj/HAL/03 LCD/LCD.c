/*****************************************************
*Author :PC1
*Version: v02
*Brief: LCD COMPONENT
*******************************************************/
#include "LCD.h"

#include <string.h>
/************************************* 8 BIT Mode functions********************************************/
#if (LCD_MODE==M_8BIT)

static void LCD_WriteCommand(uint8 Command)
{
	GPIO_write_pin(LCD_PORT_CONTROL,RS,LOW);//RS=0
	GPIO_write_port(LCD_PORT,(Command));// example: portA=0x01;

	GPIO_write_pin(LCD_PORT_CONTROL,EN,HIGH);
	_delay_us(2);
	GPIO_write_pin(LCD_PORT_CONTROL,EN,LOW);
	_delay_us(2);
}

void LCD_Init(void)
{
	GPIO_init_port(LCD_PORT,PORT_OUTPUT);//data GPIO_OUTPUT
	GPIO_init_pin(LCD_PORT_CONTROL,RS,GPIO_OUTPUT);//Rs GPIO_OUTPUT
	GPIO_init_pin(LCD_PORT_CONTROL,RW,GPIO_OUTPUT);//rw GPIO_OUTPUT
	GPIO_init_pin(LCD_PORT_CONTROL,EN,GPIO_OUTPUT); //en GPIO_OUTPUT

	GPIO_write_pin(LCD_PORT_CONTROL,RW,LOW);//RW =0

	_delay_ms(50);          // said in Data sheet to delay after power on for 1st time
	LCD_WriteCommand(0x38); //N=1 , F=0
	_delay_ms(1);           //delay 1ms a must delay as said in Data sheet
	LCD_WriteCommand(0x0c); //D=1(LCD itself on or off) C=0, B=0
	_delay_ms(1);    //delay 1ms
	LCD_WriteCommand(0x01); //clear LCD
	_delay_ms(20);
	LCD_WriteCommand(0x80);
}

static void LCD_WriteData(uint8 Data)
{
	GPIO_write_pin(LCD_PORT_CONTROL,RS,HIGH);//RS=1
	GPIO_write_port(LCD_PORT,(Data));//PORTA='a';
	GPIO_write_pin(LCD_PORT_CONTROL,EN,HIGH);
	_delay_ms(2);
	GPIO_write_pin(LCD_PORT_CONTROL,EN,LOW);
	_delay_ms(2);
}
/************************************* 4 BIT Mode functions********************************************/
#elif (LCD_MODE==M_4BIT_SAMEPORT)

static void LCD_WriteCommand(uint8 Command)
{
	GPIO_write_pin(LCD_PORT_CONTROL,RS,LOW);
	GPIO_write_pin(RW,LOW);
	GPIO_write_port_clear_mask(LCD_PORT,M_4BIT_MASK);
	GPIO_write_port_set_mask(LCD_PORT,((Command&0x0f0)>>(4-D4)));
	GPIO_write_pin(LCD_PORT_CONTROL,EN,HIGH);
	_delay_ms(2);
	GPIO_write_pin(LCD_PORT_CONTROL,EN,LOW);
	_delay_ms(2);
	GPIO_write_port_clear_mask(LCD_PORT,M_4BIT_MASK);
	GPIO_write_port_set_mask(LCD_PORT,(Command&0x0f<<(D4)));
	GPIO_write_pin(LCD_PORT_CONTROL,EN,HIGH);
	_delay_ms(2);
	GPIO_write_pin(LCD_PORT_CONTROL,EN,LOW);
	_delay_ms(2);
}

 static void LCD_WriteData(uint8 Data)
{
	GPIO_write_pin(LCD_PORT_CONTROL,RS,HIGH);
	GPIO_write_pin(RW,LOW);
	
	GPIO_write_port_clear_mask(LCD_PORT,M_4BIT_MASK);
	GPIO_write_port_set_mask(LCD_PORT,((Data&0x0f0)>>(4-D4)));
	
	GPIO_write_pin(LCD_PORT_CONTROL,EN,HIGH);
	_delay_ms(2);
	GPIO_write_pin(LCD_PORT_CONTROL,EN,LOW);
	_delay_ms(2);
	
	GPIO_write_port_clear_mask(LCD_PORT,M_4BIT_MASK);
	GPIO_write_port_set_mask(LCD_PORT,(Data&0x0f<<(D4)));
	
	GPIO_write_pin(LCD_PORT_CONTROL,EN,HIGH);
	_delay_ms(2);
	GPIO_write_pin(LCD_PORT_CONTROL,EN,LOW);
	_delay_ms(2);
}
void LCD_Init(void)
{	
	GPIO_init_pin(LCD_PORT,D4,GPIO_OUTPUT);
	GPIO_init_pin(LCD_PORT,D5,GPIO_OUTPUT);
	GPIO_init_pin(LCD_PORT,D6,GPIO_OUTPUT);
	GPIO_init_pin(LCD_PORT,D7,GPIO_OUTPUT);
	GPIO_init_pin(LCD_PORT_CONTROL,RS,GPIO_OUTPUT);
	GPIO_init_pin(LCD_PORT_CONTROL,RW,GPIO_OUTPUT);
	GPIO_init_pin(LCD_PORT_CONTROL,EN,GPIO_OUTPUT);
	_delay_ms(50);          // said in Data sheet to delay after power on for 1st time
	LCD_WriteCommand(0x02);//4 bit mode only
	_delay_ms(1); 
	LCD_WriteCommand(0x28); //N=1 , F=0
	_delay_ms(1);           //delay 1ms a must delay as said in Data sheet
	LCD_WriteCommand(0x0c); //D=1(LCD itself on or off) C=0, B=0
	_delay_ms(1);    //delay 1ms
	LCD_WriteCommand(0x01); //clear LCD
	_delay_ms(20);
	LCD_WriteCommand(0x06);
	LCD_WriteCommand(0x80);
}
/************************************* 4 BIT Mode separated Pins functions********************************************/
#elif (LCD_MODE==M_4BIT_SEPARATED)

static void LCD_WriteCommand(uint8 Command)
{
	GPIO_write_pin(LCD_PORT_CONTROL,RS,LOW);
	GPIO_write_pin(LCD_PORT_CONTROL,RW,LOW);
	GPIO_write_pin(LCD_PORT,D4,GET_BIT(Command,4));
	GPIO_write_pin(LCD_PORT,D5,GET_BIT(Command,5));
	GPIO_write_pin(LCD_PORT,D6,GET_BIT(Command,6));
	GPIO_write_pin(LCD_PORT,D7,GET_BIT(Command,7));
	GPIO_write_pin(LCD_PORT_CONTROL,EN,HIGH);
	_delay_ms(2);
	GPIO_write_pin(LCD_PORT_CONTROL,EN,LOW);
	_delay_ms(2);
	GPIO_write_pin(LCD_PORT,D4,GET_BIT(Command,0));
	GPIO_write_pin(LCD_PORT,D5,GET_BIT(Command,1));
	GPIO_write_pin(LCD_PORT,D6,GET_BIT(Command,2));
	GPIO_write_pin(LCD_PORT,D7,GET_BIT(Command,3));
	GPIO_write_pin(LCD_PORT_CONTROL,EN,HIGH);
	_delay_ms(2);
	GPIO_write_pin(LCD_PORT_CONTROL,EN,LOW);
	_delay_ms(2);
}

static void LCD_WriteData(uint8 Data)
{
	GPIO_write_pin(LCD_PORT_CONTROL,RS,HIGH);
	GPIO_write_pin(LCD_PORT_CONTROL,RW,LOW);
	GPIO_write_pin(LCD_PORT,D4,GET_BIT(Data,4));
	GPIO_write_pin(LCD_PORT,D5,GET_BIT(Data,5));
	GPIO_write_pin(LCD_PORT,D6,GET_BIT(Data,6));
	GPIO_write_pin(LCD_PORT,D7,GET_BIT(Data,7));
	GPIO_write_pin(LCD_PORT_CONTROL,EN,HIGH);
	_delay_ms(2);
	GPIO_write_pin(LCD_PORT_CONTROL,EN,LOW);
	_delay_ms(2);
	GPIO_write_pin(LCD_PORT,D4,GET_BIT(Data,0));
	GPIO_write_pin(LCD_PORT,D5,GET_BIT(Data,1));
	GPIO_write_pin(LCD_PORT,D6,GET_BIT(Data,2));
	GPIO_write_pin(LCD_PORT,D7,GET_BIT(Data,3));
	GPIO_write_pin(LCD_PORT_CONTROL,EN,HIGH);
	_delay_ms(2);
	GPIO_write_pin(LCD_PORT_CONTROL,EN,LOW);
	_delay_ms(2);
}
void LCD_Init(void)
{	
	GPIO_init_pin(LCD_PORT,D4,GPIO_OUTPUT);
	GPIO_init_pin(LCD_PORT,D5,GPIO_OUTPUT);
	GPIO_init_pin(LCD_PORT,D6,GPIO_OUTPUT);
	GPIO_init_pin(LCD_PORT,D7,GPIO_OUTPUT);
	GPIO_init_pin(LCD_PORT_CONTROL,RS,GPIO_OUTPUT);
	GPIO_init_pin(LCD_PORT_CONTROL,RW,GPIO_OUTPUT);
	GPIO_init_pin(LCD_PORT_CONTROL,EN,GPIO_OUTPUT);
	
	_delay_ms(50);          // said in Data sheet to delay after power on for 1st time
	LCD_WriteCommand(0x02);//4 bit mode only
	_delay_ms(1);
	LCD_WriteCommand(0x28); //N=1 , F=0
	_delay_ms(1);           //delay 1ms a must delay as said in Data sheet
	LCD_WriteCommand(0x0c); //D=1(LCD itself on or off) C=0, B=0
	_delay_ms(1);    //delay 1ms
	LCD_WriteCommand(0x01); //clear LCD
	_delay_ms(20);
	LCD_WriteCommand(0x06);
	LCD_WriteCommand(0x80);
}
#endif

/*************************** service functions*****************************************************/
void LCD_WriteChar(uint8 character)
{
	LCD_WriteData(character);
}



void LCD_WriteSentence(uint8 *PtrSentance)//"ahmed0"
{
	uint8 i=0;
	while(PtrSentance[i])
	{
		LCD_WriteData(PtrSentance[i]);
		i++;
	}
}
void LCD_GoTo(uint8 Line, uint8 x)// 0 1
{
	if(0 == Line)
	{
		if ( x <=15)
		{
			LCD_WriteCommand(0x80 + x);
		}
	}
	else if (1 == Line)
	{
		if ( x <=15)
		{
			LCD_WriteCommand(0xC0 + x);
		}
	}
}


void LCD_WriteNumber(uint32 Number)
{
	uint8 i=0,j;
	uint8 arr[10]={0};
	if(Number==0)
	LCD_WriteData('0');
	else{
			while(Number)
			{
				arr[i]=Number%10+'0';
				Number/=10;
				i++;
			}
			for(j=i;j>0;j--)
			{
				LCD_WriteData(arr[j-1]);
			}
	   }
}
void LCD_WriteNumber2(uint16 Number)
{
	LCD_WriteData(Number%10000/1000+'0');
	LCD_WriteData(Number%1000/100+'0');
	LCD_WriteData(Number%100/10+'0');
	LCD_WriteData(Number%10/1+'0');
}
uint8 *convert_data(uint32 val)
{
	uint8 i=0;
	static uint8 arr[10]={0};
	if(val==0)
	{arr[i]='0';}
	else{
		while(val)//123
		{
			arr[8-i]=val%10+'0';
			val/=10;
			i++;
		}
	}	
	return &arr[8-i+1];
}
 uint32 str_to_int(uint8*ptr)
 {
	 uint32 number=0;
	 for(uint16 i=0;i<strlen((char*)ptr);i++)
	 {
		 number=(ptr[i]-48)+number*10;
		 
	 }
	 return number;
 }
void Create_Character(uint8 *Pattern,uint8 Location) {

	uint8 iLoop=0;

	LCD_WriteCommand(0x40+(Location*8)); //Send the Address of CGRAM
	for(iLoop=0;iLoop<8;iLoop++)
	LCD_WriteData(Pattern[iLoop]); //Pass the bytes of pattern on LCD

}
void LCD_Clear(void)
{

	LCD_WriteCommand(0x01);

}

	
 void LCD_out(uint8 Line, uint8 x,uint8 *PtrSentance)
{
	LCD_GoTo( Line,  x);
	LCD_WriteSentence(PtrSentance);
}

