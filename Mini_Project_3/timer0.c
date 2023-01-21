/*
 * timer0.c
 *
 *  Created on: Feb 28, 2021
 *      Author: Shady Thomas
 */

#include "timer0.h"

void TIMER0_OVF_init(void)
{
	/*		Normal Mode, Pre-scaler = 64
	 * Timer0 must not Overflow before the measuring period ends
	 * since we are measuring F = 500Hz => T = 2msec, *2 = 4msec
	 * for pre-scaler 64 and F_CPU = 1MHz,
	 * F_timer = 15.625 KHz, T_timer = 64usec
	 * T_overflow = 64usec *256 = 16.38 msec
	 */
	TCCR0 = (1<<FOC0) | (1<<CS01) | (1<<CS00);

	/*Initial Timer/Counter Value*/
	TCNT0 = 0;

	/*Enable Timer0 Overflow Interrupt*/
	//TIMSK |= (1<<TOIE0);
}

void TIMER0_clearTimerValue(void)
{
	TCNT0 = 0;
}


void TIMER0_stop(void)
{
	/*Stop Timer by disabling clock*/
	TCCR0 &= ~(1<<CS01) & ~(1<<CS00);
}

