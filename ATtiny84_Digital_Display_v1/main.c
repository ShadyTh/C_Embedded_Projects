/*
 * main.c
 *
 *  Created on: Mar 14, 2021
 *      Author: Shady Thomas
 */


/*
 * main.c
 *
 *  Created on: Mar 7, 2021
 *      Author: Shady Thomas
 */

#include "7seg.h"
#include "timer1.h"

uint8 g_num = 0;

void INC_NUM(void)
{
	g_num += 1;
	if(g_num > 99)
		g_num = 0;
}

int main()
{
	SEG_initV2();

	TIMER1_setCallback(INC_NUM);
	TIMER1_CTC_init();

	SREG |= (1<<7);

	while(1)
	{
		SEG_displayUpDigitV2(g_num/10);
		_delay_ms(5);
		SEG_displayLowDigitV2(g_num%10);
		_delay_ms(5);
	}
}
