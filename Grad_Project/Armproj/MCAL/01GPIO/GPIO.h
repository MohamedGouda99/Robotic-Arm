/*****************************************************
*Author :PC1
*Version: v02
*Brief: GPIO peripheral
*******************************************************/
#ifndef GPIO_H__
#define GPIO_H__


#include "../../MCAL_REGISTERS/Mem_Map32.h"
#include "../../Helpers/Std_Types.h"
#include "../../Helpers/Utils.h"

#define GPIO_INPUT  0
#define GPIO_OUTPUT 1

#define GPIO_HIGH 1
#define GPIO_LOW  0

#define PORT_INPUT  0x00
#define PORT_OUTPUT 0xFF

#define PORT_LOW  0x00
#define PORT_HIGH 0xFF

#define MAX_PINS 7

#define PIN_0 0
#define PIN_1 1
#define PIN_2 2
#define PIN_3 3
#define PIN_4 4
#define PIN_5 5
#define PIN_6 6
#define PIN_7 7

typedef enum {
		S_PASS=1,
		F_PASS
		}GPIO_error_t;

typedef enum interrupt_pin{
    EXT_INTERRPUT_INT0_PD2 = 1 ,
    EXT_INTERRPUT_INT1_PD3,
    EXT_INTERRPUT_INT2_PB2
}GPIO_interruptPin_t;

typedef enum interrupt_sense_control{
    LOW_LEVEL_OF_INTERRUPT											 = 1 ,
	ANY_LOGICAL_CHANGE_ON_INTERRUPT										 ,
	FALLING_EDGE_OF_INTERRUPT											 ,
	RISING_EDGE_OF_INTERRUPT
}GPIO_interruptSenseControl_t;

typedef struct GPIO_interrupt{
  	 GPIO_interruptPin_t interruptPinSelect ;
	 GPIO_interruptSenseControl_t interruptSenseControl ;
	 void(*interruptFunctionPtr)(void);
}GPIO_interrupt_t;

typedef enum
{
	GPIO_A,
	GPIO_B,
	GPIO_C,
	GPIO_D,

}GPIO_port_name_t;
typedef enum restyp{
	PULLDOWN,
	PULLUP
}restype_t;

typedef struct GPIO_config{
	GPIO_port_name_t port_name ;
	uint8 pin_select[8];
	uint8 pin_direction[8];
	uint8 pin_res_type[8];
}GPIO_config_t;

GPIO_error_t GPIO_init_pin(uint8 reg, uint8 pin, uint8 dir);
void GPIO_init_port(uint8 reg, uint8 val);
void GPIO_init_port_mask(uint8 reg, uint8 val);

GPIO_error_t GPIO_write_pin(uint8 reg, uint8 pin, uint8 val);
void GPIO_write_port(uint8 reg, uint8 val);
void GPIO_write_port_set_mask(uint8 reg, uint8 val);
void GPIO_write_port_clear_mask(uint8 reg, uint8 val);
void GPIO_write_port_mask(uint8 reg, uint8 setval,uint8 clearval);
GPIO_error_t GPIO_read_pin_argument(uint8 reg, uint8 pin, uint8 * data);
GPIO_error_t GPIO_read_port_argument(uint8 reg, uint8 *val);
uint8 GPIO_read_pin(uint8 reg, uint8 pin);
uint8 GPIO_read_port(uint8 reg);

GPIO_error_t GPIO_set_pullup(uint8 reg, uint8 pin);
GPIO_error_t GPIO_set_pulldown(uint8 reg, uint8 pin);
void GPIO_set_gpiopad(GPIO_config_t GPIO_pad);//struct
void GPIO_init_interrupt(GPIO_interrupt_t *obj);//ptrtostruct

#endif /* GPIO_H_ */
