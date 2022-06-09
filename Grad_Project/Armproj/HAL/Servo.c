/*
 * Servo.c
 *
 *      Author: PC1
 */

#include "Servo.h"

#include "../Helpers/Utils.h"
static uint32 tick =0;
static uint32 dutyhand =0;
static uint32 dutyBase =0;
static uint32 dutyLeft =0;
static uint32 dutyRight =0;

void ServoHandler()
{
	tick++;
	if(tick==200)//1ms
	{
		SET_BIT(PORTC,SERVO_HAND_PIN);
		SET_BIT(PORTC,SERVO_BASE_PIN);
		SET_BIT(PORTC,SERVO_RIGHT_PIN);
		SET_BIT(PORTC,SERVO_LEFT_PIN);
		tick=0;
	}else
	{
		if (tick == dutyhand)
		{
			CLEAR_BIT(PORTC,SERVO_HAND_PIN);
		}
		if(tick ==dutyBase)
		{
			CLEAR_BIT(PORTC,SERVO_BASE_PIN);
		}
		if(tick ==dutyLeft)
		{
			CLEAR_BIT(PORTC,SERVO_LEFT_PIN);
		}
		if(tick ==dutyRight)
		{
			CLEAR_BIT(PORTC,SERVO_RIGHT_PIN);
		}
	}

}
void Servo_init()
{
	GPIO_init_pin(SERVO_HAND_PORT, SERVO_HAND_PIN, GPIO_OUTPUT);
	GPIO_init_pin(SERVO_BASE_PORT, SERVO_BASE_PIN, GPIO_OUTPUT);
	GPIO_init_pin(SERVO_LEFT_PORT, SERVO_LEFT_PIN, GPIO_OUTPUT);
	GPIO_init_pin(SERVO_RIGHT_PORT, SERVO_RIGHT_PIN, GPIO_OUTPUT);

	Timer2_SetInterruptTime_us(100,ServoHandler);
}
void Servo_Write(uint8 angle)
{
	switch(angle)
	{
	case 0:dutyhand=15;break;
	case 90:dutyhand=20;break;
	case 180:dutyhand=10;break;
	}

}
void Servo_move(enServomotorID_t ServomotorID,uint8 dutyValue)
{
	switch(ServomotorID)
	{
	case ServoMotor_Base:
		dutyBase=dutyValue;
		break;
	case ServoMotor_Hand:
		dutyhand=dutyValue;
			break;
	case ServoMotor_Left:
		dutyLeft=dutyValue;
			break;
	case ServoMotor_Right:
		dutyRight=dutyValue;
			break;

	}

}
