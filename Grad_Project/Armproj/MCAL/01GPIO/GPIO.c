
#include "GPIO.h"
#include <avr/interrupt.h>
#include "../../Helpers/Std_Types.h"
#include "../../Helpers/Utils.h"
#include "../../MCAL_REGISTERS/Mem_Map32.h"

// int x[3]={y,z,l}; remember that is wrong definination.
volatile uint8 *registers_dir[4]={&DDRA,&DDRB,&DDRC,&DDRD};
volatile uint8 *registers_data[4]={&PORTA,&PORTB,&PORTC,&PORTD};
volatile uint8 *registers_input[4]={&PINA,&PINB,&PINC,&PIND};

static void(*GPIO_int0_ptr)(void); /* to catch interrupt function address an pass it to ISR function. */
static void(*GPIO_int1_ptr)(void); /* to catch interrupt function address an pass it to ISR function. */
static void(*GPIO_int2_ptr)(void); /* to catch interrupt function address an pass it to ISR function. */
//------------------------------------------------------------------------------------------------------
/********************************************************************
 * Func : init_pin
 * Args
 * GPIO_INPUT : reg -> register name(BASE_A,BASE_B,BASE_C,or BASE_D)
       : pin -> pin number (0,1,2,3,4,5,6,7)
	   : dir -> GPIO_INPUT
	         -> GPIO_OUTPUT
 *Return : state -> it includes two possiblilities
          S_PASS  = function success
		  E_FAIL  = function fail
 **********************************************************************/
GPIO_error_t GPIO_init_pin(uint8 reg, uint8 pin, uint8 dir) {
	GPIO_error_t state = S_PASS;
	if ((GPIO_OUTPUT == dir||GPIO_INPUT == dir)
			&&(pin<=7)
			&&(reg<=3)
	)
	{
		WRITE_BIT(*registers_dir[reg], pin , dir);
	}
	else
	{

		state=F_PASS;
	}
	return state;
}
//------------------------------------------------------------------
/*********************************************************
 * Func : init_port
 * Args : reg -> register name(BASE_A,BASE_B,BASE_C,or BASE_D)
	   : dir -> PORT_INPUT
	         -> PORT_OUTPUT
example:
		GPIO_init_port(GPIO_A,0x0f);
		DDRA |=0x0f; 0bxxxx 1111
************************************************************/

void GPIO_init_port(uint8 reg, uint8 val) {
	WRITE_PORT(*registers_dir[reg],val) ;

}
void GPIO_init_port_mask(uint8 reg, uint8 val)
{
	PORT_SET_MASK(*registers_dir[reg], val);
}
//------------------------------------------------------------------
/**
 * Func : write_pin
 * Args : reg -> register name(BASE_A,BASE_B,BASE_C,or BASE_D)
       : pin -> pin number (0,1,2,3,4,5,6,or 7)
	   : val -> HIGH
	         -> LOW
 *Return : state -> it includes two possiblilities
          S_PASS  = function success
		  E_FAIL  = function fail
 */
GPIO_error_t GPIO_write_pin(uint8 reg, uint8 pin, uint8 val) {
	GPIO_error_t state = S_PASS ;
	if ((HIGH == val||(LOW == val))&&(pin<=MAX_PINS)) {
		WRITE_BIT(*registers_data[reg], pin , val);
	}
	else
	{
		state = F_PASS;
	}

	return state;
}
//------------------------------------------------------------------
/**
 * Func : write_port
 * Args : reg -> register name(BASE_A,BASE_B,BASE_C,or BASE_D)
	   : val -> PORT_HIGH
	         -> PORT_LOW
 */
void GPIO_write_port(uint8 reg, uint8 val) {
	WRITE_PORT(*registers_data[reg], val);

}
void GPIO_write_port_set_mask(uint8 reg, uint8 val){
	PORT_SET_MASK(*registers_data[reg], val);
}
void GPIO_write_port_clear_mask(uint8 reg, uint8 val){
	PORT_CLEAR_MASK(*registers_data[reg], val);
}

void GPIO_write_port_mask(uint8 reg, uint8 setval,uint8 clearval){
	PORT_MASK(*registers_data[reg], setval,clearval);
}
//------------------------------------------------------------------
/**
 * Func : read_pin
 * Args : reg -> register name(BASE_A,BASE_B,BASE_C,or BASE_D)
       : pin -> pin number (0,1,2,3,4,5,6,or 7)
	   : *data->pointer to return the value in
 *Return : state -> it includes two possiblilities
          S_PASS  = function success
		  E_FAIL  = function fail
		  GPIO_read_pin_argument(GPIO_A,2,&x);
 */
GPIO_error_t GPIO_read_pin_argument(uint8 reg, uint8 pin, uint8 * data) {
	GPIO_error_t state = S_PASS;
	if ((pin<=MAX_PINS))
	{
		*data = GET_BIT(*registers_input[reg] , pin);
	}
	else
	{
		state = F_PASS;
	}
	return state;
}

uint8 GPIO_read_pin(uint8 reg, uint8 pin)
{
	return 	GET_BIT(*registers_input[reg] , pin );
}
//----------------------------------------------------------------
/**
 * Func : read_port
 * Args : reg -> register name(BASE_A,BASE_B,BASE_C,or BASE_D)
       : pin -> pin number (0,1,2,3,4,5,6,or 7)
	   : *val ->pointer to return the value in
 */
GPIO_error_t  GPIO_read_port_argument(uint8 reg, uint8 *val) {
	GPIO_error_t status=S_PASS;
	if(reg<4)
	{
		*val = ReadPort(*registers_input[reg]);
	}
	else
	{
		status=F_PASS;
	}
	return status;
}
uint8 GPIO_read_port(uint8 reg) {
	return (ReadPort(*registers_input[reg]));
}

GPIO_error_t GPIO_set_pullup(uint8 reg, uint8 pin)
{
	GPIO_error_t state = S_PASS;
	if ((pin<=MAX_PINS)&&(pin>=0))
	{
		WRITE_BIT(*registers_data[reg] , pin,GPIO_HIGH);
	}
	else
	{
		state = F_PASS;
	}
	return state;
}
GPIO_error_t GPIO_set_pulldown(uint8 reg, uint8 pin)
{
	GPIO_error_t state = S_PASS;
	if ((pin<=MAX_PINS)&&(pin>=0))
	{
		WRITE_BIT(*registers_data[reg] , pin,LOW);
	}
	else
	{
		state = F_PASS;
	}
	return state;
}
void GPIO_set_gpiopad(GPIO_config_t GPIO_pad)
{
	for (uint8 index=0;index<=MAX_PINS;index++)
	{
		if (GPIO_pad.pin_select[index]==1)
		{
			GPIO_init_pin(GPIO_pad.port_name ,index , GPIO_pad.pin_direction[index]);
			if (GPIO_pad.pin_res_type[index]==1)
			{
				GPIO_set_pullup(GPIO_pad.port_name,index);
			}else
			{
				GPIO_set_pulldown(GPIO_pad.port_name,index);
			}

		}
	}
}

/************************************************************************/
/*                      Interrupt Functions                             */
/************************************************************************/
static void init_GPIO_INT0_interrupt(GPIO_interrupt_t *obj){

	GPIO_int0_ptr = obj->interruptFunctionPtr;//local pointerto function ->fun//setcallback
	WRITE_BIT(GICR , PIN_6 , HIGH);//Peripheral interrupt 0

	if (obj->interruptSenseControl == LOW_LEVEL_OF_INTERRUPT)
	{
		WRITE_BIT(MCUCR , PIN_0 , LOW);
		WRITE_BIT(MCUCR , PIN_1 , LOW);
	}
	else if (obj->interruptSenseControl ==ANY_LOGICAL_CHANGE_ON_INTERRUPT)
	{
		WRITE_BIT(MCUCR , PIN_0 , HIGH);
		WRITE_BIT(MCUCR , PIN_1 , LOW);
	}
	else if (obj->interruptSenseControl ==FALLING_EDGE_OF_INTERRUPT)
	{
		WRITE_BIT(MCUCR , PIN_0 , LOW);
		WRITE_BIT(MCUCR , PIN_1 , HIGH);
	}
	else if (obj->interruptSenseControl == RISING_EDGE_OF_INTERRUPT)
	{
		WRITE_BIT(MCUCR , PIN_0 , HIGH);
		WRITE_BIT(MCUCR , PIN_1 , HIGH);
	}
}
//-----------------------------------------------------------------------
static void init_GPIO_INT1_interrupt(GPIO_interrupt_t *obj){

	GPIO_int1_ptr = obj->interruptFunctionPtr;
	WRITE_BIT(GICR , PIN_7 , HIGH);

	if (obj->interruptSenseControl == LOW_LEVEL_OF_INTERRUPT)
	{
		WRITE_BIT(MCUCR , PIN_2 , LOW);
		WRITE_BIT(MCUCR , PIN_3 , LOW);
	}
	else if (obj->interruptSenseControl ==ANY_LOGICAL_CHANGE_ON_INTERRUPT)
	{
		WRITE_BIT(MCUCR , PIN_2 , HIGH);
		WRITE_BIT(MCUCR , PIN_3 , LOW);
	}
	else if (obj->interruptSenseControl ==FALLING_EDGE_OF_INTERRUPT)
	{
		WRITE_BIT(MCUCR , PIN_2 , LOW);
		WRITE_BIT(MCUCR , PIN_3 , HIGH);
	}
	else if (obj->interruptSenseControl == RISING_EDGE_OF_INTERRUPT)
	{
		WRITE_BIT(MCUCR , PIN_2 , HIGH);
		WRITE_BIT(MCUCR , PIN_3 , HIGH);
	}

}
//----------------------------------------------------------------------
static void init_GPIO_INT2_interrupt(GPIO_interrupt_t *obj){

	GPIO_int2_ptr = obj->interruptFunctionPtr;//localptr
	WRITE_BIT(GICR , PIN_5 , HIGH);

	if (obj->interruptSenseControl ==FALLING_EDGE_OF_INTERRUPT)
	{
		WRITE_BIT(MCUCSR , PIN_6 , LOW);

	}
	else if (obj->interruptSenseControl == RISING_EDGE_OF_INTERRUPT)
	{
		WRITE_BIT(MCUCSR , PIN_6 , HIGH);
	}

}
//-----------------------------------------------------------------------

void GPIO_init_interrupt(GPIO_interrupt_t *obj){

	//sei();//global interrupt enable
	if (obj->interruptPinSelect == EXT_INTERRPUT_INT0_PD2)
	{
		init_GPIO_INT0_interrupt(obj);
	}
	else if (obj->interruptPinSelect == EXT_INTERRPUT_INT1_PD3)
	{
		init_GPIO_INT1_interrupt(obj);
	}
	else if (obj->interruptPinSelect == EXT_INTERRPUT_INT2_PB2)
	{
		init_GPIO_INT2_interrupt(obj);
	}
}

void GPIOInterrupt_SetEdge(	GPIO_interruptSenseControl_t interruptSenseControl )
{
	if (interruptSenseControl ==FALLING_EDGE_OF_INTERRUPT)
	{
		WRITE_BIT(MCUCR , PIN_0 , LOW);
		WRITE_BIT(MCUCR , PIN_1 , HIGH);

	}
	else if (interruptSenseControl == RISING_EDGE_OF_INTERRUPT)
	{
		WRITE_BIT(MCUCR , PIN_0 , HIGH);
		WRITE_BIT(MCUCR , PIN_1 , HIGH);
	}

}
void GPIOInterrupt_SetCallBack(	 void(*interruptFunctionPtr)(void))
{
	GPIO_int0_ptr = interruptFunctionPtr;//local pointerto function ->fun//setcallback

}
//------------------------------------------
ISR(INT0_vect){

	GPIO_int0_ptr();
}
//-------------------------------------
//ISR(INT1_vect){
//	(*GPIO_int1_ptr)();
//}
//-----------------------------------
ISR(INT2_vect){
	(*GPIO_int2_ptr)();
}


