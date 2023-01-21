/*
 * main.c
 *
 *  Created on: Apr 1, 2021
 *      Author: Shady Thomas
 */

#include "sh_timer.h"

uint8 g_t0_tick = 0;

void APP_toggleLED(void)
{
	if(g_t0_tick < 4)
	{
		g_t0_tick++;
	}
	else
	{
		PORTA ^= 0x01;
		g_t0_tick = 0;
	}
}

int main()
{
	TIMER0_CONFIG t0config = {CTC, TOGGLE_OC0_RESERVED, F_CPU_1024, 0, 30, OCIE_ENABLE, TOIE_DISABLE};

	TIMER1_CONFIG t1config = {0};

	TIMER2_CONFIG t2config = {T2_CTC, TOGGLE_OC2_RESERVED, T2_F_CPU_1024, 0, 30, OCIE2_ENABLE, TOIE2_DISABLE};

	TIMER_CONFIG timer_config = {TIMER0, &t0config, &t1config, &t2config};

	TIMER_CONFIG timer_config2 = {TIMER2, &t0config, &t1config, &t2config};

	TIMER0_CTC_setCallBack(APP_toggleLED);

	TIMER_init(&timer_config);
	TIMER_init(&timer_config2);

	DDRA  |= 0x01;
	PORTA |= 0x00;

	SET_BIT(SREG,7);

	while(1)
	{
	}
}
