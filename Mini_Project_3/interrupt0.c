/*
 * interrupt0.c
 *
 *  Created on: Feb 28, 2021
 *      Author: Shady Thomas
 */

#include "interrupt0.h"

static volatile void (*g_callbackPtr) (void) = NULL_PTR;

ISR(INT0_vect)
{
	if(*g_callbackPtr != NULL_PTR)
		(*g_callbackPtr)();
}

void INT0_init()
{
	/*set PD2 as input*/
	CLEAR_BIT(DDRD,PD2);

	/*generate interrupt on rising edge*/
	MCUCR |= (1<<ISC01) | (1<<ISC00);

	/*enable interrupt0 module*/
	SET_BIT(GICR,INT0);
}

void INT0_setCallback(void (*a_ptr)(void))
{
	g_callbackPtr = a_ptr;
}

void INT0_senseControl(const INT0_Sense sense)
{
	MCUCR = (MCUCR & 0xFC) | (0x03 & sense);
}

void INT0_deinit()
{
	/*disable interrupt0 module*/
	CLEAR_BIT(GICR,INT0);
}
