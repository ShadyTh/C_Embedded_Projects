/*
 * timer1.c
 *
 *  Created on: Mar 1, 2021
 *      Author: Shady Thomas
 */

#include "timer1.h"

static volatile void (*g_callbackPtr) (void) = NULL_PTR;

ISR(TIM1_COMPA_vect)
{
	if(g_callbackPtr != NULL_PTR)
		(*g_callbackPtr)();
}

void TIMER1_CTC_init(void)
{
	TCCR1A = 0;
	/* disable OC1A and OC1B, non-PWM mode */

	TCCR1B = (1<<WGM12) | (1<<CS12);
	/* CTC mode 4, pre-scaler = 256 */

	TCCR1C = (1<<FOC1A);

	TIMSK1 |= (1<<OCIE1A);	/* enable OC interrupt */

	TCNT1 = 0;				/* initial value of timer */

	OCR1A = 4 * 8;			/* compare value for incrementing 1 milli - second * X */

	/* F_CPU = 1MHz, F_Timer = 3.906 KHz, T_Timer = 256 usec */

}

void TIMER1_setCallback(void (*ptr) (void))
{
	g_callbackPtr = ptr;
}
