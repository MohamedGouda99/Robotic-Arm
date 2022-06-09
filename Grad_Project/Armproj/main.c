
#include <stdint.h>
#include <util/delay.h>

#include "HAL/Servo.h"
/*
 * main.c
 *
 *      Author: PC1
 */

#include "Helpers/Std_Types.h"
#include "MCAL/02Timer/Timers.h"
#include "UART_Services.h"
#include "MCAL/05UART/UART.h"


#include "MCAL/01GPIO/GPIO.h"

#define GREEN_PIN  PIN_5
#define RED_PIN    PIN_6
#define BLUE_PIN   PIN_7

char recieved_buffer[20]={0};
uint8 stopflag=0;
uint8 i=0;
uint16 timeout=0;
uint8 FlagCatch=0;
uint8 cnt_base=0;
//Simple Wait Function
void PostionsSearchColor(uint8_t color);
void Postion1();
void Postion2();
void Postion3();
void OpenHand();
void SearchPostion2(uint8_t color);
void SearchPostion3(uint8_t color);
void BackToPatient(uint8_t color);
void Searchpostion1(uint8_t color);
void CloseHand();
void RecieveFromBt()
{
	recieved_buffer[i]=UART_ReceiveByteNoBlock();
	i++;
}


int main()
{

	UART_Init();
	UART_RX_InterruptEnable();
	UART_RX_SetCallBack(RecieveFromBt);
	LCD_Init();
	Servo_init();
	GPIO_init_pin(GPIO_D, PIN_6, GPIO_INPUT);
	GPIO_set_pullup(GPIO_D, PIN_6);//blue

	GPIO_init_pin(GPIO_D, PIN_7, GPIO_INPUT);
	GPIO_set_pullup(GPIO_D, PIN_7);//red

	sei();

	OpenHand();
	_delay_ms(2000);
	CloseHand();
	while(1)
	{
		if(timeout<1000)
		{
			timeout++;
		}
		else if(i!=0)
		{

			LCD_WriteSentence(recieved_buffer);

			if(strstr(recieved_buffer,(char*)"1") != 0 ||strstr(recieved_buffer,(char*)"one") != 0)
			{
				Postion1();
			}
			else if(strstr(recieved_buffer,(char*)"2") != 0 ||strstr(recieved_buffer,(char*)"two") != 0)
			{
				Postion2();
			}
			else if(strstr(recieved_buffer,(char*)"3") != 0 ||strstr(recieved_buffer,(char*)"three") != 0)
			{
				Postion3();
			}
			else if(strstr(recieved_buffer,(char*)"up") != 0 ||strstr(recieved_buffer,(char*)"Up") != 0)
			{
				ArmUp();

			}
			else if(strstr(recieved_buffer,(char*)"down") != 0 ||strstr(recieved_buffer,(char*)"Down") != 0)
			{
				ArmDownLunch();
			}
			else if(strstr(recieved_buffer,(char*)"open") != 0 ||strstr(recieved_buffer,(char*)"leave") != 0)
			{
				if(FlagCatch==1)
				{
					LCD_Clear();
					LCD_GoTo(0, 0);
					LCD_WriteSentence("Take it Please");
					LCD_GoTo(1,0);
					LCD_WriteSentence("Thanks to you");
					FlagCatch=0;

				}else
				{	LCD_Clear();
				LCD_GoTo(0, 5);
				LCD_WriteSentence("Medicine");
				LCD_GoTo(1, 5);
				LCD_WriteSentence("unavailable");
				FlagCatch=0;

				}
				OpenHand();
			}
			else if(strstr(recieved_buffer,(char*)"close") != 0 ||strstr(recieved_buffer,(char*)"tch") != 0)
			{
				CloseHand();
			}
			else if(strstr(recieved_buffer,(char*)"head") != 0 ||strstr(recieved_buffer,(char*)"ache") != 0)
			{
				PostionsSearchColor(2);//red
			}
			else if(strstr(recieved_buffer,(char*)"diab") != 0 ||strstr(recieved_buffer,(char*)"sugar") != 0)
			{
				PostionsSearchColor(1);//blue
			}
			else if(strstr(recieved_buffer,(char*)"heart") != 0 ||strstr(recieved_buffer,(char*)"tack") != 0)
			{
				PostionsSearchColor(3);//green
			}
			memset(recieved_buffer,0,sizeof recieved_buffer);

			_delay_ms(1000);
			i=0;
			timeout=0;

		}
	}
}
//1 red
//2 blue

void PostionsSearchColor(uint8_t color)
{
	FlagCatch=0;
	cnt_base=0;
	LCD_Clear();
	LCD_GoTo(0, 0);
	LCD_WriteSentence("Searching.....");
	_delay_ms(1000);
	Searchpostion1(color);
	if(FlagCatch == 1)
	{
		//nothing
	}else
	{
		SearchPostion2(color);
		if(FlagCatch==1)
		{
			//nothing
		}else
		{
			SearchPostion3(color);
		}
	}
	BackToPatient(color);


}
void Searchpostion1(uint8_t color)
{
	for(int i=5;i<8;i++)//0-180
	{
		Servo_move(ServoMotor_Base,i);	 //Set servo shaft at -90� position
		_delay_ms(500);
	}
	//
	/**********Hand************/
	OpenHand();
	/**************Up and Down*******/
	ArmDownLunch();
	if (color == 1)//blue
	{
		if(GPIO_read_pin(GPIO_D, BLUE_PIN)==0)
		{
			CloseHand();
			FlagCatch=1;
//			cnt_base=8;
		}
	}
	else if(color == 2)//red
	{
		if(GPIO_read_pin(GPIO_D, RED_PIN)==0)
		{
			CloseHand();
			FlagCatch=1;
//			cnt_base=8;
		}
	}
	else if(color == 3)//green
	{
		if(GPIO_read_pin(GPIO_D, GREEN_PIN)==0)
		{
			CloseHand();
			FlagCatch=1;
//			cnt_base=8;
		}
	}
	cnt_base=8;

}
void SearchPostion2(uint8_t color)
{
	ArmUp();
	_delay_ms(1000);
	for(int i=8;i<12;i++)//0-180
	{
		Servo_move(ServoMotor_Base,i);	 //Set servo shaft at -90� position
		_delay_ms(500);
	}
	/**********Hand************/
	OpenHand();
	/**************Up and Down*******/
	ArmDownLunch();
	if (color == 1)
	{
		if(GPIO_read_pin(GPIO_D, BLUE_PIN) != 1)
		{
			CloseHand();
			FlagCatch=1;

		}
	}
	else if(color == 2)
	{
		if(GPIO_read_pin(GPIO_D, RED_PIN)==0)
		{
			CloseHand();
			FlagCatch=1;
//			cnt_base=12;

		}
	}
	else if(color == 3)
	{
		if(GPIO_read_pin(GPIO_D, GREEN_PIN)==0)
		{
			CloseHand();
			FlagCatch=1;
//			cnt_base=12;
		}
	}
	cnt_base=12;
}
void SearchPostion3(uint8_t color)
{
	ArmUp();
	_delay_ms(1000);

	for(int i=12;i<16;i++)//0-180
	{
		Servo_move(ServoMotor_Base,i);	 //Set servo shaft at -90� position
		_delay_ms(500);
	}
	/**********Hand************/
	OpenHand();
	/**************Up and Down*******/
	ArmDownLunch();
	if (color == 1)
	{
		if(GPIO_read_pin(GPIO_D, BLUE_PIN) != 1)
		{
			CloseHand();
			FlagCatch=1;
		}
	}
	else if(color == 2)
	{
		if(GPIO_read_pin(GPIO_D, RED_PIN)==0)
		{
			CloseHand();
			FlagCatch=1;
//			cnt_base=16;
		}
	}
	else if(color == 3)
	{
		if(GPIO_read_pin(GPIO_D, GREEN_PIN)==0)
		{

			CloseHand();
			FlagCatch=1;
//			cnt_base=16;
		}
	}
	//	_delay_ms(1000);
	_delay_ms(1000);
	cnt_base=16;

}

void BackToPatient(uint8_t color)
{
	ArmUp();
	for(int i=cnt_base;i<20;i++)//0-180
	{
		Servo_move(ServoMotor_Base,i);	 //Set servo shaft at -90� position
		_delay_ms(500);
	}
	ArmDownLunch();
}
void OpenHand()
{
	for(int i=25;i>6;i--)//open
	{
		Servo_move(ServoMotor_Hand,i);	// Set servo shaft at -90� position
		_delay_ms(20);
	}
}
void CloseHand()
{
	for(int i=5;i<=25;i++)//close
	{
		Servo_move(ServoMotor_Hand,i);	// Set servo shaft at -90� position
		_delay_ms(20);
	}
}
void ArmDownLunch()
{
	Servo_move(ServoMotor_Right,10);	 //Set servo shaft at -90� position
	Servo_move(ServoMotor_Right,200);	 //Set servo shaft at -90� position
	for(int j=20;j>=10;j--)//0-180
	{
		Servo_move(ServoMotor_Left,j);	 //Set servo shaft at -90� position
		_delay_ms(120);
	}
}
void ArmUp()
{
	for(int j=10;j<=20;j++)//0-180
	{
		Servo_move(ServoMotor_Left,j);	 //Set servo shaft at -90� position
		_delay_ms(120);
	}
}
void Postion1()
{
	for(int i=5;i<10;i++)//0-180
	{
		Servo_move(ServoMotor_Base,i);	 //Set servo shaft at -90� position
		_delay_ms(500);
	}
	/**********Hand************/
	OpenHand();
	/**************Up and Down*******/
	ArmDownLunch();
	CloseHand();
	_delay_ms(1000);
	ArmUp();
	_delay_ms(1000);
}

void Postion2()
{
	for(int i=10;i<15;i++)//0-180
	{
		Servo_move(ServoMotor_Base,i);	 //Set servo shaft at -90� position
		_delay_ms(500);
	}
	/**********Hand************/
	OpenHand();
	//	Servo_move(ServoMotor_Right,10);	 //Set servo shaft at -90� position
	//	Servo_move(ServoMotor_Right,200);	 //Set servo shaft at -90� position

	/**************Up and Down*******/
	ArmDownLunch();
	CloseHand();
	_delay_ms(1000);
	ArmUp();
	_delay_ms(1000);
}
void Postion3()
{
	for(int i=15;i<20;i++)//0-180
	{
		Servo_move(ServoMotor_Base,i);	 //Set servo shaft at -90� position
		_delay_ms(500);
	}
	/**********Hand************/
	OpenHand();
	//	Servo_move(ServoMotor_Right,10);	 //Set servo shaft at -90� position
	//	Servo_move(ServoMotor_Right,200);	 //Set servo shaft at -90� position

	/**************Up and Down*******/
	ArmDownLunch();
	CloseHand();
	_delay_ms(1000);
	ArmUp();
	_delay_ms(1000);
}
