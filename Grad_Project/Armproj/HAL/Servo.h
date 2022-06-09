/*
 * Servo.h
 *
 *      Author: PC1
 */

#ifndef HAL_SERVO_H_
#define HAL_SERVO_H_
#include "../MCAL/01GPIO/GPIO.h"
#include "../MCAL/02Timer/Timers.h"

#define SERVO_HAND_PORT GPIO_C
#define SERVO_HAND_PIN  PIN_1

#define SERVO_BASE_PORT GPIO_C
#define SERVO_BASE_PIN  PIN_0

#define SERVO_LEFT_PORT GPIO_C
#define SERVO_LEFT_PIN  PIN_2

#define SERVO_RIGHT_PORT GPIO_C
#define SERVO_RIGHT_PIN  PIN_3



typedef enum
{
	ServoMotor_Base,
	ServoMotor_Hand,
	ServoMotor_Left,
	ServoMotor_Right,

}enServomotorID_t;
void Servo_init();
void Servo_Write(uint8 angle);
void Servo_move(enServomotorID_t ServomotorID,uint8 dutyValue);

#endif /* HAL_SERVO_H_ */
