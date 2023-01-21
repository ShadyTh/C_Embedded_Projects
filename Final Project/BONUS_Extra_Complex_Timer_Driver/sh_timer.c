/*
 * sh_timer.c
 *
 * Fully Configurable Timer Driver
 *
 *  Created on: Apr 1, 2021
 *      Author: Shady Thomas
 */

#include "sh_timer.h"

/******************************************************/
/**************   Timer 0  CALLBACKs ******************/
/******************************************************/
/****** OVF *******/
static volatile void (*g_t0_OVF_Callback) (void) = NULL_PTR;

ISR(TIMER0_OVF_vect)
{
	if(g_t0_OVF_Callback != NULL_PTR)
	{
		(*g_t0_OVF_Callback)();
	}
}

void TIMER0_OVF_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_t0_OVF_Callback = a_ptr;
}

/****** CTC *******/
static volatile void (*g_t0_CTC_Callback) (void) = NULL_PTR;

ISR(TIMER0_COMP_vect)
{
	if(g_t0_CTC_Callback != NULL_PTR)
	{
		(*g_t0_CTC_Callback)();
	}
}

void TIMER0_CTC_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_t0_CTC_Callback = a_ptr;
}

/******************************************************/
/**************   Timer 2  CALLBACKs ******************/
/******************************************************/
/****** OVF *******/
static volatile void (*g_t2_OVF_Callback) (void) = NULL_PTR;

ISR(TIMER2_OVF_vect)
{
	if(g_t2_OVF_Callback != NULL_PTR)
	{
		(*g_t2_OVF_Callback)();
	}
}

void TIMER2_OVF_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_t2_OVF_Callback = a_ptr;
}

/****** CTC *******/
static volatile void (*g_t2_CTC_Callback) (void) = NULL_PTR;

ISR(TIMER2_COMP_vect)
{
	if(g_t2_CTC_Callback != NULL_PTR)
	{
		(*g_t2_CTC_Callback)();
	}
}

void TIMER2_CTC_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_t2_CTC_Callback = a_ptr;
}

/******************************************************/
/**************   Timer 1  CALLBACKs ******************/
/******************************************************/
/****** OVF *******/
static volatile void (*g_t1_OVF_Callback) (void) = NULL_PTR;

ISR(TIMER1_OVF_vect)
{
	if(g_t1_OVF_Callback != NULL_PTR)
	{
		(*g_t1_OVF_Callback)();
	}
}

void TIMER1_OVF_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_t1_OVF_Callback = a_ptr;
}

/****** CTC  A *******/
static volatile void (*g_t1_CTC_A_Callback) (void) = NULL_PTR;

ISR(TIMER1_COMPA_vect)
{
	if(g_t1_CTC_A_Callback != NULL_PTR)
	{
		(*g_t1_CTC_A_Callback)();
	}
}

void TIMER1_CTC_A_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_t1_CTC_A_Callback = a_ptr;
}

/****** CTC  B *******/
static volatile void (*g_t1_CTC_B_Callback) (void) = NULL_PTR;

ISR(TIMER1_COMPB_vect)
{
	if(g_t1_CTC_B_Callback != NULL_PTR)
	{
		(*g_t1_CTC_B_Callback)();
	}
}

void TIMER1_CTC_B_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_t1_CTC_B_Callback = a_ptr;
}

/****** ICU *******/
static volatile void (*g_t1_ICU_Callback) (void) = NULL_PTR;

ISR(TIMER1_CAPT_vect)
{
	if(g_t1_ICU_Callback != NULL_PTR)
	{
		(*g_t1_ICU_Callback)();
	}
}

void TIMER1_ICU_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_t1_ICU_Callback = a_ptr;
}

/******************************************************/
/*********** 	Initialization	***********************/
/******************************************************/

void TIMER_init(const TIMER_CONFIG * config_ptr)
{
	if((config_ptr->timer_type) == TIMER0)
	{
		// configure WGM00 and WGM01
		TCCR0 = (TCCR0 & ~0x48) | ((config_ptr->t0_ptr->t0_wgm & 0x01)<<WGM00)
				| ((config_ptr->t0_ptr->t0_wgm & 0x10)<<WGM01);

		//configure COM00 and COM01
		TCCR0 = (TCCR0 & ~0x30) | ((config_ptr->t0_ptr->t0_com) << COM00);

		//OCx Pin set as output, when used
		if(config_ptr->t0_ptr->t0_com != OC0_DISCONNECTED)
		{
			SET_BIT(DDRB,PB3);
		}

		//configure pre-scaler
		TCCR0 = (TCCR0 & ~0x07) | ((config_ptr->t0_ptr->t0_prescaler));

		//if WGM is Normal or CTC, FOC0 = 1
		if(config_ptr->t0_ptr->t0_wgm == 0x00 || config_ptr->t0_ptr->t0_wgm == 0x02)
		{
			TCCR0 |= (1<<FOC0);
		}
		else	//PWM Mode, FOC0 = 0
		{
			TCCR0 &= ~(1<<FOC0);
		}

		TCNT0 = config_ptr->t0_ptr->t0_tcnt0;
		OCR0  = config_ptr->t0_ptr->t0_ocr0;

		TIMSK = (TIMSK & 0xFE) | (config_ptr->t0_ptr->t0_toie);
		TIMSK = (TIMSK & 0xFD) | ((config_ptr->t0_ptr->t0_ocie) << OCIE0);
	}

	else if((config_ptr->timer_type) == TIMER1)
	{
		/* configure COM1A and COM1B */
		TCCR1A = (TCCR1A & 0X0F) | \
				((config_ptr->t1_ptr->t1_com_a) <<  COM1A0) | \
				((config_ptr->t1_ptr->t1_com_b) <<  COM1B0);

		/*  Set OC1A and OC1B as output pins when used: */
		if(config_ptr->t1_ptr->t1_com_a != OC1A_DISCONNECTED)
			SET_BIT(DDRD,PD5);

		if(config_ptr->t1_ptr->t1_com_b != OC1B_DISCONNECTED)
			SET_BIT(DDRD,PD4);

		/* Configure Waveform Generation Mode bits (WGM13-10) */
		TCCR1A = (TCCR1A & 0XFC)  | ((config_ptr->t1_ptr->t1_wgm) & 0x03);
		TCCR1B = (TCCR1B & ~0x18) | (((config_ptr->t1_ptr->t1_wgm) & 0xC0) << 1);

		/* Configure Pre-scaler */
		TCCR1B = (TCCR1B & ~0x07) | (config_ptr->t1_ptr->t1_prescaler);

		//if WGM is Normal or CTC, FOC1A and FOC1B = 1
		if(config_ptr->t1_ptr->t1_wgm == 0 || \
				config_ptr->t1_ptr->t1_wgm == 4 || \
				config_ptr->t1_ptr->t1_wgm == 12)
		{
			TCCR1A |= (1 << FOC1A) | (1 << FOC1B);
		}
		else	//PWM Mode, FOC1A-B = 0
		{
			TCCR1A &= ~(1 << FOC1A) & ~(1 << FOC1B);
		}

		/* ICU Noise Canceler and Edge Select */
		TCCR1B = (TCCR1B & 0x3F) | \
				(config_ptr->t1_ptr->t1_icu_nc << ICNC1) | \
				((config_ptr->t1_ptr->t1_icu_edge << ICES1));

		TCNT1 = config_ptr->t1_ptr->t1_tcnt1;
		OCR1A = config_ptr->t1_ptr->t1_ocr1a;
		OCR1B = config_ptr->t1_ptr->t1_ocr1b;
		ICR1  = config_ptr->t1_ptr->t1_icr1;

		/* Configure Interrupts */
		TIMSK =  (TIMSK & 0xC3) | \
				((config_ptr->t1_ptr->t1_interrupt_icu) << TICIE1) | \
				((config_ptr->t1_ptr->t1_interrupt_oc1a) << OCIE1A) | \
				((config_ptr->t1_ptr->t1_interrupt_oc1b) << OCIE1B) | \
				((config_ptr->t1_ptr->t1_interrupt_ovf) << TOIE1);
	}

	else if((config_ptr->timer_type) == TIMER2)
	{
		/* Configure COM2 */
		TCCR2 = (TCCR2 & 0xCF) | ((config_ptr->t2_ptr->t2_com) << COM20);

		/* Set OC2 as output when used */
		if(config_ptr->t2_ptr->t2_com != OC2_DISCONNECTED)
			SET_BIT(DDRD, PD7);

		/* Configure WGM */
		TCCR2 = (TCCR2 & ~0x48) | \
				(((config_ptr->t2_ptr->t2_wgm) & 0x01) << WGM20) | \
				(((config_ptr->t2_ptr->t2_wgm) & 0x02) << WGM21);

		//if WGM is Normal or CTC, FOC2 = 1
		if(config_ptr->t2_ptr->t2_wgm == 0x00 || config_ptr->t2_ptr->t2_wgm == 0x02)
		{
			TCCR2 |= (1<<FOC2);
		}
		else	//PWM Mode, FOC0 = 0
		{
			TCCR2 &= ~(1<<FOC2);
		}

		/* Configure Pre-scaler */
		TCCR2 = (TCCR2 & 0xF1) | (config_ptr->t2_ptr->t2_prescaler);

		TCNT2 = config_ptr->t2_ptr->t2_tcnt2;
		OCR2  = config_ptr->t2_ptr->t2_ocr2;

		TIMSK = (TIMSK & 0x3F) | \
				((config_ptr->t2_ptr->t2_ocie) << OCIE2) |	\
				((config_ptr->t2_ptr->t2_toie) << TOIE2);
	}
}

void TIMER_deinit(const TIMER_CONFIG * config_ptr)
{
	if((config_ptr->timer_type) == TIMER0)
	{
		/* Stop Clock */
		TCCR0 &= ~(0x07);
		/* Disable all interrupts */
		TIMSK &= 0xFC;
	}
	else if((config_ptr->timer_type) == TIMER1)
	{
		TCCR1B &= ~(0x07);
		TIMSK  &= 0xC3;
	}
	else if((config_ptr->timer_type) == TIMER2)
	{
		TCCR2  &= ~0x07;
		TIMSK  &= 0x3F;
	}
}
