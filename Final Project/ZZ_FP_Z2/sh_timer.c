/*
 * sh_timer.c
 *
 * A simpler timer driver used for delays
 *
 *  Created on: Apr 21, 2021
 *      Author: Shady Thomas
 */

#include "sh_timer.h"

/******************************************************/
/*********** 	Initialization	***********************/
/******************************************************/
void (*const TIMER_delay[3])(uint16) = {TIMER0_delay, TIMER1_delay, TIMER2_delay};

void TIMER0_delay(uint16 millis)
{
	/* F_CPU = 8MHz, Pre-scaler = 256
	 * F_Timer = 31.25 KHz
	 * T_timer = 32us
	 * no of counts in 1 ms = 31.25 = 31
	 *
	 * CTC Mode, non-PWM*/

	while(millis > 0)
	{
		TCCR0 = (1<<FOC0) | (1<<WGM01) | (1<<CS02);
		TCNT0 = 0;
		OCR0 = 31;
		while(BIT_IS_CLEAR(TIFR, OCF0));	/* delay for 1 ms */
		millis--;
		SET_BIT(TIFR,OCF0);		/* Clear flag by writing 1 to it*/
	}

	/* stop timer */
	TCCR0 &= ~(0x07);
}


void TIMER1_delay(uint16 millis)
{
	/* F_CPU = 8MHz, Pre-scaler = 256
	 * F_Timer = 31.25 KHz
	 * T_timer = 32us
	 * no of counts in 1 ms = 31.25 = 31
	 *
	 * CTC Mode, non-PWM*/
	TCCR1A = (1<<FOC1A);
	TCCR1B = (1<<WGM12) | (1<<CS12);

	TCNT1 = 0;
	OCR1A = 31;

	while(millis > 0)
	{
		while(BIT_IS_CLEAR(TIFR, OCF1A));	/* delay for 1 ms */
		millis--;
		SET_BIT(TIFR,OCF1A);		/* Clear flag by writing 1 to it*/
	}

	/* stop timer */
	TCCR1B &= ~(0x07);

}

/* delays of 10ms */
void TIMER2_delay(uint16 ten_millis)
{
	/*F_CPU = 8MHz, Pre-scaler = 1024
	 * F_Timer = 7.812KHz
	 * T_timer = 128us
	 * Counts per 10ms = 78
	 * */
	TCCR2 = (1<<FOC2) | (1<<WGM21) | (1<<CS22);
	TCNT2 = 0;
	OCR2 = 78;

	while(ten_millis > 0)
	{
		while(BIT_IS_CLEAR(TIFR, OCF2));	/* delay for 10 ms */
		ten_millis--;
		SET_BIT(TIFR,OCF2);		/* Clear flag by writing 1 to it*/
	}

	/* stop timer */
	TCCR2 &= ~(0x07);
}

void TIMER0_PWM_init()
{
	/* Generate a 50% duty cycle, 122 Hz PWM Signal on Pin OC0 */

	TCNT0 = 0;
	OCR0  = 128;
	SET_BIT(DDRB,PB3);	/*Set OC0 (PB3) as output*/

	/* Fast PWM mode, non-inverting
	 * PWM_frequency = 8,000,000 / (256 * Pre-scaler)
	 * Pre-scaler = 256
	 * PWM_frequency = 122 Hz */
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS02);
}

void TIMER0_deinit(void)
{
	TCCR0 &= ~(0x07);
}
