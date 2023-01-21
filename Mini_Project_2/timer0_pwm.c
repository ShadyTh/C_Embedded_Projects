/*
 * timer0_pwm.c
 *
 *  Created on: Feb 18, 2021
 *      Author: Shady Thomas
 */


#include "timer0_pwm.h"

void TIMER0_PWM_init()
{
	TCNT0 = 0;		/* Timer starts counting from zero */
	/*	*/
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS01);
	DDRB  = DDRB | (1<<PB3);		/* Enable OC0 (PB3) pin as output */
	OCR0  = 0;		/*initially OFF*/
}

void TIMER0_PWM_setDutyCycle(uint16 duty)
{
	OCR0 = duty;
}
