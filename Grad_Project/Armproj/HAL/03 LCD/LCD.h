/*****************************************************
*Author :PC1
*Version: v02
*Brief: LCD COMPONENT
*******************************************************/
#ifndef LCD_H_
#define LCD_H_

#include "../../MCAL/01GPIO/GPIO.h"
#include <util/delay.h>

/**********************************     LCD Modes   **************************************************/
#define M_4BIT_SAMEPORT  0
#define M_4BIT_SEPARATED 1
#define M_8BIT           2


/********************************* define control pins     *******************************************/

/*********************************   define LCD Mode       *******************************************/
#define LCD_MODE  M_4BIT_SEPARATED

/*******************for 8 bit mode and 4 bit_sameport mode define the LCD port **********************/
#define LCD_PORT			GPIO_A // data wires on portA
#define LCD_PORT_CONTROL	GPIO_B // RS RW EN portB
#define D0  
#define D1  
#define D2  
#define D3  
#define D4  PIN_4
#define D5  PIN_5
#define D6  PIN_6
#define D7  PIN_7

#define RS  PIN_1
#define RW  PIN_2
#define EN  PIN_3
/*******************for 4 bit same port mode define pins mask in port *******************************/
#define M_4BIT_MASK 0Xf0
/***************************************************************************************************/

extern void LCD_Init(void);
extern void LCD_WriteChar(uint8 Character);
extern void LCD_WriteSentence(uint8 *PtrSentance);
extern void LCD_GoTo(uint8 Line, uint8 x);
extern void LCD_out(uint8 Line, uint8 x,uint8 *PtrSentance);

extern void LCD_WriteNumber(uint32 Number); //lcd 21 "21"
extern void LCD_WriteNumber2(uint16 Number);//

extern void LCD_Create_Character(uint8 *Pattern,uint8 Location);//

extern void LCD_Clear(void);
/*********************************** service for lcd*************************************************/
uint8 *convert_data(uint32 val); //12 ->  "12"
uint32  str_to_int(uint8*ptr);//"12" ->   12

#endif
