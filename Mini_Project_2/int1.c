/*
 * int1.c
 *
 *  Created on: Feb 18, 2021
 *      Author: Shady Thomas
 */

#include "int1.h"

ISR(INT1_vect)
{
	//reverse motor direction
	// by toggling both bits
	g_motor_direction ^= (1<<PB6) | (1<<PB7);
	PORTB = (PORTB & 0x3F) | (g_motor_direction & 0xC0);

}

void INT1_init()
{
	CLEAR_BIT(DDRD,PD3);		/* set PD3 as input */
	SET_BIT(GICR,INT1);			/* enable INT1 */
	SET_BIT(MCUCR,ISC11);
	SET_BIT(MCUCR,ISC10);		/* rising edge generates the INT request */
}
