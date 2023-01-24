/*
 * timer1.c
 *
 *  Created on: Mar 1, 2021
 *      Author: Shady Thomas
 */

#include "timer0.h"

static volatile void (*g_callbackPtr) (void) = NULL_PTR;

ISR(TIM0_COMPA_vect)
{
	if(g_callbackPtr != NULL_PTR)
		(*g_callbackPtr)();
}

void TIMER0_CTC_init(void)
{
	/* disable OC0A and OC0B, non-PWM mode, CTC Mode */
	TCCR0A = (1<<WGM01);

	TCCR0B = (1<<FOC0A) | (1<<CS02) | (1<<CS00);
	/* CTC mode 4, pre-scaler = 256 */

	TIMSK0 |= (1<<OCIE0A);	/* enable OC interrupt */

	TCNT0 = 0;				/* initial value of timer */

	OCR0A = 250;			/* compare value for incrementing 250 milli-second */

	/* F_CPU = 1MHz, F_Timer = 976.56 Hz, T_Timer = 1.024 ms */
}

void TIMER0_setCallback(void (*ptr) (void))
{
	g_callbackPtr = ptr;
}
