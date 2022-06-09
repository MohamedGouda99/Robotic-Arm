#include "Timers.h"

volatile uint16 t1,t2,t3,flag=0;
static volatile uint8 scaler0_global=0;
static volatile uint8 scaler1_global=0;
static volatile uint8 scaler2_global=0;

/****************************Pointer to functions to be assigned to ISR*********************************/
static void (*TIMER0_OVF_Fptr) (void)=NULLPTR;
static void (*TIMER0_CMP_Fptr) (void)=NULLPTR;
static void (*TIMER2_OVF_Fptr) (void)=NULLPTR;
static void (*TIMER2_CMP_Fptr) (void)=NULLPTR;
static void (*Timer1_OVF_Fptr) (void)=NULLPTR;
static void (*Timer1_OCA_Fptr) (void)=NULLPTR;
static void (*Timer1_OCB_Fptr) (void)=NULLPTR;
static void (*Timer1_ICU_Fptr) (void)=NULLPTR;
/**********************************Timer 0 ****************************************************/
//static void Func_ICU(void);
/**
 * @fn void TIMER0_Init(TIMER0_Mode_t, TIMER0_Prescalar, TIMER0_OC0Mode_tt)
 * @brief
 * Timer initaasdasdasd
 * @param mode
 *  TIMER0_NORMAL_MODE=0,
	TIMER0_PHASECORRECT_MODE,
	TIMER0_CTC_MODE,
	TIMER0_FASTPWM_MODE
 * @param scaler
 * TIMER0_NORMAL_MODE=0,
	TIMER0_PHASECORRECT_MODE,
	TIMER0_CTC_MODE,
	TIMER0_FASTPWM_MODE
 * @param oc_mode
 */
void TIMER0_Init( TIMER0_Mode_t mode,TIMER0_Prescalar scaler ,TIMER0_OC0Mode_tt oc_mode)
{
	/*
	Timer/Counter Control
	Register  TCCR0
	FOC0 WGM00 COM01 COM00 WGM01 CS02 CS01 CS00

	 0 0 Normal 0xFF Immediate MAX
	 0 1 PWM, Phase Correct 0xFF TOP BOTTOM
	 1 0 CTC OCR0 Immediate MAX
	 1 1 Fast PWM 0xFF BOTTOM MAX
	 */
	switch (mode)
	{
	case TIMER0_NORMAL_MODE:
		CLEAR_BIT(TCCR0,WGM00);
		CLEAR_BIT(TCCR0,WGM01);
		break;
	case TIMER0_PHASECORRECT_MODE:
		SET_BIT(TCCR0,WGM00);
		CLEAR_BIT(TCCR0,WGM01);
		break;
	case TIMER0_CTC_MODE:
		CLEAR_BIT(TCCR0,WGM00);
		SET_BIT(TCCR0,WGM01);
		break;
	case TIMER0_FASTPWM_MODE:
		SET_BIT(TCCR0,WGM00);
		SET_BIT(TCCR0,WGM01);

		break;
	}


	TIMER0_CONTROL_PRES_ADJ(scaler);
	scaler0_global=scaler;
	switch (oc_mode)
	{
	case OCO_DISCONNECTED:
		CLEAR_BIT(TCCR0,COM00);
		CLEAR_BIT(TCCR0,COM01);
		break;
	case OCO_TOGGLE:
		SET_BIT(TCCR0,COM00);
		CLEAR_BIT(TCCR0,COM01);
		break;
	case OCO_NON_INVERTING:
		CLEAR_BIT(TCCR0,COM00);
		SET_BIT(TCCR0,COM01);
		break;
	case OCO_INVERTING:
		SET_BIT(TCCR0,COM00);
		SET_BIT(TCCR0,COM01);
		break;
	}


}

void TIMER2_Init( TIMER2_Mode_t mode,TIMER2_Prescalar scaler ,TIMER2_OC0Mode_tt oc_mode)
{
	/*
	Timer/Counter Control
	Register  TCCR0
	FOC0 WGM00 COM01 COM00 WGM01 CS02 CS01 CS00

	 0 0 Normal 0xFF Immediate MAX
	 0 1 PWM, Phase Correct 0xFF TOP BOTTOM
	 1 0 CTC OCR0 Immediate MAX
	 1 1 Fast PWM 0xFF BOTTOM MAX
	 */
	switch (mode)
	{
	case TIMER2_NORMAL_MODE:
		CLEAR_BIT(TCCR2,WGM20);
		CLEAR_BIT(TCCR2,WGM21);
		break;
	case TIMER2_PHASECORRECT_MODE:
		SET_BIT(TCCR2,WGM20);
		CLEAR_BIT(TCCR2,WGM21);
		break;
	case TIMER2_CTC_MODE:
		CLEAR_BIT(TCCR2,WGM20);
		SET_BIT(TCCR2,WGM21);
		break;
	case TIMER2_FASTPWM_MODE:
		SET_BIT(TCCR2,WGM20);
		SET_BIT(TCCR2,WGM21);

		break;
	}


	TIMER2_CONTROL_PRES_ADJ(scaler);
	scaler2_global=scaler;
	switch (oc_mode)
	{
	case OC2_DISCONNECTED:
		CLEAR_BIT(TCCR2,COM20);
		CLEAR_BIT(TCCR2,COM21);
		break;
	case OC2_TOGGLE:
		SET_BIT(TCCR2,COM20);
		CLEAR_BIT(TCCR2,COM21);
		break;
	case OC2_NON_INVERTING:
		CLEAR_BIT(TCCR2,COM20);
		SET_BIT(TCCR2,COM21);
		break;
	case OC2_INVERTING:
		SET_BIT(TCCR2,COM20);
		SET_BIT(TCCR2,COM21);
		break;
	}


}

void TIMER0_OV_InterruptEnable(void)
{
	SET_BIT(TIMSK,TOIE0);
}
void TIMER0_OV_InterruptDisable(void)
{
	CLEAR_BIT(TIMSK,TOIE0);
}
void TIMER0_OC_InterruptEnable(void)
{
	SET_BIT(TIMSK,OCIE0);
}
void TIMER0_OC_InterruptDisable(void)
{
	CLEAR_BIT(TIMSK,OCIE0);
}
void TIMER0_OVF_SetCallBack(void(*LocalFptr)(void))
{
	TIMER0_OVF_Fptr=LocalFptr;
}
void TIMER0_CMP_SetCallBack(void(*LocalFptr)(void))
{
	TIMER0_CMP_Fptr=LocalFptr;
}
void TIMER0_reload(uint8 TIMER0_val)
{
	TCNT0=TIMER0_val;
}
void TIMER0_Compare_reload(uint8 TIMER0_comp_val)
{
	OCR0=TIMER0_comp_val;
}
uint8 Get_Current_Timer()
{
	return TCNT0;
}
void TIMER0_Stop()
{
	PORT_CLEAR_MASK(TCCR0,TIM_STOP);
}
void TIMER0_restart(){

	TCCR0|=scaler0_global;
}

/*****************************
 *Note: Maximum 255 in time
 *****************************/

//void Timer0_SetInterruptTime_us (uint8 time,void(*LocalFptr)(void))
//{
//	TCNT0=0;
//	TIMER0_Init(TIMER0_CTC_MODE,TIMER0_SCALER_1,OCO_DISCONNECTED);
//	OCR0=((time));//(P/F)*(OCR-1)
//	TIMER0_CMP_SetCallBack(LocalFptr);
//	TIMER0_OC_InterruptEnable();
//}


/********************************Timer2******************************************/
//void TIMER2_OV_InterruptEnable(void)
//{
//	SET_BIT(TIMSK,TOIE2);
//}
//void TIMER2_OV_InterruptDisable(void)
//{
//	CLEAR_BIT(TIMSK,TOIE2);
//}
void TIMER2_OC_InterruptEnable(void)
{
	SET_BIT(TIMSK,OCIE2);
}
void TIMER2_OC_InterruptDisable(void)
{
	CLEAR_BIT(TIMSK,OCIE2);
}
void TIMER2_OVF_SetCallBack(void(*LocalFptr)(void))
{
	TIMER2_OVF_Fptr=LocalFptr;
}
void TIMER2_CMP_SetCallBack(void(*LocalFptr)(void))
{
	TIMER2_CMP_Fptr=LocalFptr;
}
void TIMER2_reload(uint8 TIMER2_val)
{
	TCNT2=TIMER2_val;
}
void TIMER2_Compare_reload(uint8 TIMER2_comp_val)
{
	OCR2=TIMER2_comp_val;
}
uint8 Get_Current_Timer2()
{
	return TCNT2;
}
void TIMER2_Stop()
{
	PORT_CLEAR_MASK(TCCR2,TIM_STOP);
}
void TIMER2_restart(){

	TCCR2|=scaler2_global;
}
void Timer2_SetInterruptTime_us (uint8 time,void(*LocalFptr)(void))
{
	TCNT2=0;
	TIMER2_Init(TIMER2_CTC_MODE,TIMER2_SCALER_8,OC2_DISCONNECTED);
	OCR2=((time));//(P/F)*(OCR-1)
	TIMER2_CMP_SetCallBack(LocalFptr);
	TIMER2_OC_InterruptEnable();
}

/*************************************************TIMER 1 **************************************************/


void Timer1_Init( Timer1Mode_t mode,Timer1Scaler_t scaler,OC1A_Mode_t oc1a_mode,OC1B_Mode_t oc1b_mode)

{
	/*
	Timer/Counter1
	Control Register A –
	TCCR1A
			COM1A1 COM1A0 COM1B1 COM1B0 FOC1A FOC1B WGM11 WGM10 
	 */
	switch (mode)
	{
	case TIMER1_NORMAL_MODE:
		CLEAR_BIT(TCCR1A,WGM10);
		CLEAR_BIT(TCCR1A,WGM11);
		CLEAR_BIT(TCCR1B,WGM12);
		CLEAR_BIT(TCCR1B,WGM13);
		break;
	case TIMER1_CTC_ICR_TOP_MODE:
		CLEAR_BIT(TCCR1A,WGM10);
		CLEAR_BIT(TCCR1A,WGM11);
		SET_BIT(TCCR1B,WGM12);
		SET_BIT(TCCR1B,WGM13);
		break;

	case TIMER1_CTC_OCRA_TOP_MODE:
		CLEAR_BIT(TCCR1A,WGM10);
		CLEAR_BIT(TCCR1A,WGM11);
		SET_BIT(TCCR1B,WGM12);
		CLEAR_BIT(TCCR1B,WGM13);
		break;

	case TIMER1_FASTPWM_ICR_TOP_MODE:
		CLEAR_BIT(TCCR1A,WGM10);
		SET_BIT(TCCR1A,WGM11);
		SET_BIT(TCCR1B,WGM12);
		SET_BIT(TCCR1B,WGM13);
		break;

	case TIMER1_FASTPWM_OCRA_TOP_MODE:
		SET_BIT(TCCR1A,WGM10);
		SET_BIT(TCCR1A,WGM11);
		SET_BIT(TCCR1B,WGM12);
		SET_BIT(TCCR1B,WGM13);
		break;
	}
	switch (oc1a_mode)
	{
	case OCRA_DISCONNECTED:
		CLEAR_BIT(TCCR1A,COM1A0);
		CLEAR_BIT(TCCR1A,COM1A1);
		break;
	case OCRA_TOGGLE:
		SET_BIT(TCCR1A,COM1A0);
		CLEAR_BIT(TCCR1A,COM1A1);
		break;
	case OCRA_NON_INVERTING:
		CLEAR_BIT(TCCR1A,COM1A0);
		SET_BIT(TCCR1A,COM1A1);
		break;
	case OCRA_INVERTING:
		SET_BIT(TCCR1A,COM1A0);
		SET_BIT(TCCR1A,COM1A1);
		break;
	}
	switch (oc1b_mode)
	{
	case OCRB_DISCONNECTED:
		CLEAR_BIT(TCCR1A,COM1B0);
		CLEAR_BIT(TCCR1A,COM1B1);
		break;
	case OCRB_TOGGLE:
		SET_BIT(TCCR1A,COM1B0);
		CLEAR_BIT(TCCR1A,COM1B1);
		break;
	case OCRB_NON_INVERTING:
		CLEAR_BIT(TCCR1A,COM1B0);
		SET_BIT(TCCR1A,COM1B1);
		break;
	case OCRB_INVERTING:
		SET_BIT(TCCR1A,COM1B0);
		SET_BIT(TCCR1A,COM1B1);
		break;
	}


	TCCR1B&=0XF8;
	TCCR1B|=scaler;
	scaler1_global=scaler;
}

void Timer1_InputCaptureEdge(ICU_Edge_t edge)
{
	if(edge==RISING)
		SET_BIT(TCCR1B,ICES1);

	else if(edge==FALLING)
		CLEAR_BIT(TCCR1B,ICES1);

}

void Timer1_reload(uint16 Timer1_val){TCNT1=Timer1_val;}
void Timer1_CompareA_reload(uint16 Timer1_compA_val){OCR1A=Timer1_compA_val;}
void Timer1_CompareB_reload(uint16 Timer1_compB_val){OCR1B=Timer1_compB_val;}
void Timer1_ICU_reload(uint16 Timer1_ICU_val){ICR1=Timer1_ICU_val;}
uint16 Get_Current_Timer1(){return TCNT1;}
void Timer1_Stop()
{
	PORT_CLEAR_MASK(TCCR1B,TIM_STOP);
}
void timer1_restart()
{
	Timer1_Stop();
	TCCR1B|=scaler1_global;
}
/****************************Timer  Interrupt functions**************************************/

void Timer1_ICU_InterruptEnable(void)
{
	SET_BIT(TIMSK,TICIE1);
}
void Timer1_ICU_InterruptDisable(void)
{
	CLEAR_BIT(TIMSK,TICIE1);
}
void Timer1_OVF_InterruptEnable(void)
{
	SET_BIT(TIMSK,TOIE1);
}
void Timer1_OVF_InterruptDisable(void)
{
	CLEAR_BIT(TIMSK,TOIE1);
}
void Timer1_OCA_InterruptEnable(void)
{
	SET_BIT(TIMSK,OCIE1A);
}
void Timer1_OCA_InterruptDisable(void)
{
	CLEAR_BIT(TIMSK,OCIE1A);
}
void Timer1_OCB_InterruptEnable(void)
{
	SET_BIT(TIMSK,OCIE1B);
}
void Timer1_OCB_InterruptDisable(void)
{
	CLEAR_BIT(TIMSK,OCIE1B);
}

/*********************************Timer 1 Call Back functions*****************************************/
//Timer1_OVF_SetCallBack
void Timer1_OVF_SetCallBack(void(*LocalFptr)(void))
{
	Timer1_OVF_Fptr=LocalFptr;
}
void Timer1_OCA_SetCallBack(void(*LocalFptr)(void))
{
	Timer1_OCA_Fptr=LocalFptr;
}
void Timer1_OCB_SetCallBack(void(*LocalFptr)(void))
{
	Timer1_OCB_Fptr=LocalFptr;
}
void Timer1_ICU_SetCallBack(void(*LocalFptr)(void))
{
	Timer1_ICU_Fptr=LocalFptr;
}


/*********************************Timer 1 ISR functions*********************************************/
//ISR(TIMER0_OVF_vect)
//{
//	if(TIMER0_OVF_Fptr!=NULLPTR)
//	{
//		TIMER0_OVF_Fptr();
//	}
//}
ISR(TIMER0_COMP_vect)
{
	if(TIMER0_CMP_Fptr!=NULLPTR)
	{
		TIMER0_CMP_Fptr();
	}
}
ISR(TIMER2_OVF_vect)
{
	if(TIMER2_OVF_Fptr!=NULLPTR)
	{
		TIMER2_OVF_Fptr();
	}
}
ISR(TIMER2_COMP_vect)
{
	if(TIMER2_CMP_Fptr!=NULLPTR)
	{
		TIMER2_CMP_Fptr();
	}
}
ISR(TIMER1_OVF_vect)
{
	if(Timer1_OVF_Fptr!=NULLPTR)
	{
		Timer1_OVF_Fptr();
	}
}

ISR(TIMER1_COMPA_vect)
{
	if(Timer1_OCA_Fptr!=NULLPTR)
	{
		Timer1_OCA_Fptr();
	}
}
ISR(TIMER1_COMPB_vect)
{
	if(Timer1_OCB_Fptr!=NULLPTR)
	{
		Timer1_OCB_Fptr();
	}
}
ISR(TIMER1_CAPT_vect)
{
	if(Timer1_ICU_Fptr!=NULLPTR)
	{
		Timer1_ICU_Fptr();
	}
}


/*****************************************************************************************/

/**********************************************PWM*********************************************************/
void PWM_Init(void)
{

	Timer1_Init(TIMER1_FASTPWM_ICR_TOP_MODE,TIMER1_SCALER_8,OCRA_NON_INVERTING,OCRB_DISCONNECTED);

}

void PWM_Freq_KHZ_(uint16 freq)
{
#if defined(F_CPU)&& (F_CPU==8000000)
	if (freq==0)
	{
		uint16 Ttotal=1000/freq;
		if (Ttotal>=1)
		{
			ICR1=Ttotal-1;
		}
		else ICR1=0;

	}
#else
#warning "FCPU MUST TO BE 8 MHZ"
#endif
}
void PWM_Freq_HZ(uint16 freq)
{
#if defined(F_CPU)&& (F_CPU==8000000)
	if (freq==0)
	{
		uint16 Ttotal=(uint32)1000000/freq;
		if (Ttotal>=1)
		{
			ICR1=Ttotal-1;
		}
		else ICR1=0;
	}
#else
#warning "FCPU MUST TO BE 8 MHZ"
#endif


}
void AnalogWrite(enAnalogPin_t enAnalogPin,uint16 value )
{
	static uint8 inital0=0;
	static uint8 inital1b=0;
	static uint8 inital1a=0;
	static uint8 inital2=0;

	switch(enAnalogPin)
	{
	case OCO_PB3:
		if(inital0==0)
			TIMER0_Init(TIMER0_FASTPWM_MODE, TIMER0_SCALER_8, OCO_NON_INVERTING);
		OCR0=value;
		inital0=1;
		break;
	case OC1B_PD4:
		if(inital1b==0)
			Timer1_Init(TIMER1_FASTPWM_ICR_TOP_MODE,TIMER1_SCALER_8,OCRA_NON_INVERTING,OCRB_NON_INVERTING);
		ICR1L=255;
		ICR1H=0;
		OCR1B=value;
		inital1b=1;
		break;
	case OC1A_PD5:
		if(inital1a==0)
			Timer1_Init(TIMER1_FASTPWM_ICR_TOP_MODE,TIMER1_SCALER_8,OCRA_NON_INVERTING,OCRB_NON_INVERTING);
		ICR1L=255;
		ICR1H=0;
		OCR1A=value;
		inital1a=1;
		break;
	case OC2_PD7:
		if(inital2==0)
		{
			TIMER2_Init(TIMER2_FASTPWM_MODE, TIMER2_SCALER_8, OC2_NON_INVERTING);

		}
		OCR2=value;
		inital2=1;
		break;

	}

}

void PWM_Duty(uint16 duty)
{
	if (duty<=100)
	{
		uint16 Ton=((uint32)duty*(ICR1+1))/100;
		if (Ton>1)
		{
			OCR1A=Ton-1;
		}
		else
			OCR1A=0;
	}
}
/******************************************Set Interrupt Time******************************************************/

void Timer1_SetInterruptTime_us (uint32 time,void(*LocalFptr)(void))
{
	Timer1_reload(0);
	Timer1_Init(TIMER1_CTC_OCRA_TOP_MODE,TIMER1_SCALER_8,OCRA_DISCONNECTED,OCRB_DISCONNECTED);
	OCR1A=((uint32)(time*1))-1;//us
	Timer1_OCA_SetCallBack(LocalFptr);
	Timer1_OCA_InterruptEnable();

}


/**********************************************Measure PWM*********************************************************/
//void PWM_Measure2(uint32* Pfreq,uint32* Pduty)
//{
//	uint32 Ton,Toff;
//	flag=0;
//	TCNT1=0;
//	while (GET_BIT(PIND,6));
//	while (!GET_BIT(PIND,6));
//	TCNT1=0;
//	while (GET_BIT(PIND,6));
//	Ton=TCNT1;
//	TCNT1=0;
//	while (!GET_BIT(PIND,6));
//	Toff=TCNT1;
//	*Pduty=((uint32)Ton*100)/(Ton+Toff);
//	*Pfreq=(uint32)1000000/((uint32)Toff+Ton);
//}
//
//
//void PWM_Measure(uint32* Pfreq,uint32* Pduty)
//{
//	uint32 Ton,Toff;
//
//	TCNT1=0;
//	Timer1_ICU_SetCallBack(Func_ICU);
//	Timer1_InputCaptureEdge(RISING);
//	Timer1_ICU_InterruptEnable();
//	flag=0;
//	while(flag<3);
//	Ton=t2-t1;
//	Toff=t3-t2;
//
//	*Pduty=((uint32)Ton*100)/((uint32)Ton+Toff);
//	*Pfreq=(uint32)1000000/((uint32)Toff+Ton);
//
//}
//
//static void Func_ICU(void)
//{
//	if (flag==0)
//	{
//		t1=ICR1;
//		Timer1_InputCaptureEdge(FALLING);
//		flag=1;
//	}
//	else if (flag==1)
//	{
//		t2=ICR1;
//		Timer1_InputCaptureEdge(RISING);
//		flag=2;
//	}
//	else if (flag==2)
//	{
//		t3=ICR1;
//		Timer1_ICU_InterruptDisable();
//		flag=3;
//	}
//
//
//
//}
