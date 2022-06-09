/*****************************************************
*Author :PC1
*Version: v02
*Brief: TIMER peripheral
*******************************************************/
#ifndef TIMERS_H_
#define TIMERS_H_

#include "../../Helpers/Std_Types.h"
#include "../../Helpers/Utils.h"
#include "../../MCAL_REGISTERS/Mem_Map32.h"

/*************************** TIMER 0 *****************************************/
typedef enum{
	TIMER0_STOP=0,
	TIMER0_SCALER_1,
	TIMER0_SCALER_8,
	TIMER0_SCALER_64,
	TIMER0_SCALER_256,
	TIMER0_SCALER_1024,
	EXTERNALl_FALLING,
	EXTERNAL_RISING
}TIMER0_Prescalar;
typedef enum{
	TIMER2_STOP=0,
	TIMER2_SCALER_1,
	TIMER2_SCALER_8,
	TIMER2_SCALER_64,
	TIMER2_SCALER_256,
	TIMER2_SCALER_1024,

}TIMER2_Prescalar;
typedef enum
{
	TIMER0_NORMAL_MODE=0,
	TIMER0_PHASECORRECT_MODE,
	TIMER0_CTC_MODE,
	TIMER0_FASTPWM_MODE
}TIMER0_Mode_t;
typedef enum
{
	TIMER2_NORMAL_MODE=0,
	TIMER2_PHASECORRECT_MODE,
	TIMER2_CTC_MODE,
	TIMER2_FASTPWM_MODE
}TIMER2_Mode_t;

typedef enum
{
	OCO_DISCONNECTED=0,
	OCO_TOGGLE,
	OCO_NON_INVERTING,
	OCO_INVERTING

}TIMER0_OC0Mode_tt;

typedef enum
{
	OC2_DISCONNECTED=0,
	OC2_TOGGLE,
	OC2_NON_INVERTING,
	OC2_INVERTING

}TIMER2_OC0Mode_tt;
#define  TIM_STOP	0x07 // 1111 1000  portb&=~(0x07)
#define  TIMER0_CONTROL_PRES_ADJ(scalar)		(TCCR0=((TCCR0)&(~0x07))|(scalar))
#define  TIMER2_CONTROL_PRES_ADJ(scalar)		(TCCR2=((TCCR2)&(~0x07))|(scalar))

void TIMER0_Init( TIMER0_Mode_t mode,TIMER0_Prescalar scaler ,TIMER0_OC0Mode_tt oc_mode);
void TIMER0_reload(uint8 TIMER0_val);
void TIMER0_Compare_reload(uint8 TIMER0_comp_val);

uint8 Get_Current_Timer();
void TIMER0_Stop();
void TIMER0_restart();

void TIMER0_OV_InterruptEnable(void);
void TIMER0_OV_InterruptDisable(void);
void TIMER0_OC_InterruptEnable(void);
void TIMER0_OC_InterruptDisable(void);

void TIMER0_OVF_SetCallBack(void(*LocalFptr)(void));//TIMER0_OVF_SetCallBack(fun)
void TIMER0_CMP_SetCallBack(void(*LocalFptr)(void));

/************************************************************************************************/
/*************************** TIMER 1 *****************************************/

typedef enum{
	TIMER1_STOP=0,
	TIMER1_SCALER_1,
	TIMER1_SCALER_8,
	TIMER1_SCALER_64,
	TIMER1_SCALER_256,
	EXTERNAL0_FALLING,
	TIMER1_SCALER_1024,
	EXTERNAL0_RISING
}Timer1Scaler_t;

typedef enum
{
	TIMER1_NORMAL_MODE=0,
	TIMER1_CTC_ICR_TOP_MODE,
	TIMER1_CTC_OCRA_TOP_MODE,
	TIMER1_FASTPWM_ICR_TOP_MODE,
	TIMER1_FASTPWM_OCRA_TOP_MODE

}Timer1Mode_t;

typedef enum
{
	OCRA_DISCONNECTED=0,
	OCRA_TOGGLE,
	OCRA_NON_INVERTING,
	OCRA_INVERTING

}OC1A_Mode_t;

typedef enum
{
	OCRB_DISCONNECTED=0,
	OCRB_TOGGLE,
	OCRB_NON_INVERTING,
	OCRB_INVERTING

}OC1B_Mode_t;


typedef enum{
	RISING,
	FALLING
}ICU_Edge_t;

typedef enum
{
	OCO_PB3,
	OC1B_PD4,
	OC1A_PD5,
	OC2_PD7
}enAnalogPin_t;
void Timer1_Init( Timer1Mode_t mode,Timer1Scaler_t scaler,OC1A_Mode_t oc1a_mode,OC1B_Mode_t oc1b_mode);
void Timer1_reload(uint16 Timer1_val);

void Timer1_CompareA_reload(uint16 Timer1_compA_val);
void Timer1_CompareB_reload(uint16 Timer1_compB_val);
void Timer1_ICU_reload(uint16 Timer1_ICU_val);

uint16 Get_Current_Timer1();
void Timer1_Stop();
void timer1_restart();

void Timer1_ICU_InterruptEnable(void);
void Timer1_ICU_InterruptDisable(void);
void Timer1_OVF_InterruptEnable(void);
void Timer1_OVF_InterruptDisable(void);
void Timer1_OCA_InterruptEnable(void);
void Timer1_OCA_InterruptDisable(void);
void Timer1_OCB_InterruptEnable(void);
void Timer1_OCB_InterruptDisable(void);

void Timer1_OVF_SetCallBack(void(*LocalFptr)(void));
void Timer1_OCA_SetCallBack(void(*LocalFptr)(void));
void Timer1_OCB_SetCallBack(void(*LocalFptr)(void));
void Timer1_ICU_SetCallBack(void(*LocalFptr)(void));
void Timer1_InputCaptureEdge(ICU_Edge_t edge);

/************************************************************************************************/
/*************************** Serivce *****************************************/
void Timer1_SetInterruptTime_us (uint32 time,void(*LocalFptr)(void));

/**TASK******/
void PWM_Init(void);
void PWM_Freq_KHZ(uint16 freq);
void PWM_Freq_HZ(uint16 freq);
void PWM_Duty(uint16 duty);
void PWM_Measure(uint32* Pfreq,uint32* Pduty);
void PWM_Measure2(uint32* Pfreq,uint32* Pduty);

void AnalogWrite(enAnalogPin_t,uint16 value );

/**Videos***/

#endif /* TIMERS_H_ */
