/*
 * ShadyThomas_Mini_Project_3.c
 *
 *  Created on: Feb 28, 2021
 *      Author: Shady Thomas
 *
 *      Description: Exercise 1 ICU: Measuring Duty Cycle of an input square wave
 *      But with using Timer0 and INT0 to record time instead of ICU.
 *
 *      Tried implementing Layered Architecture despite application being simple
 *      	- added INT0 Driver with callback and struct for Interrupt Sense Control
 *      	- added TIMER0 Normal Mode driver
 */

#include "lcd.h"
#include "timer0.h"
#include "interrupt0.h"

uint8 g_count = 0;
uint8 g_t1HIGH = 0;
uint8 g_t2PERIOD = 0;
uint8 g_t3PERIODwHIGH = 0;

void APP_getDutyCycle()
{
	g_count++;
	if(g_count == 1)
	{
		TIMER0_clearTimerValue();
		INT0_senseControl(FALLING);
	}
	if(g_count == 2)
	{
		g_t1HIGH = TCNT0;
		INT0_senseControl(RISING);
	}
	if(g_count == 3)
	{
		g_t2PERIOD = TCNT0;
		INT0_senseControl(FALLING);
	}
	if(g_count == 4)
	{
		g_t3PERIODwHIGH = TCNT0;
	}
}

int main()
{
	uint16 duty;

	/*initialize Timer0*/
	TIMER0_OVF_init();
	/*initialize LCD and prepare it*/
	LCD_init();
	LCD_displayString("Duty Cycle: ");
	/*prepare interrupt module and ISR*/
	INT0_setCallback(APP_getDutyCycle);
	INT0_init();
	/*enable global interrupt*/
	SET_BIT(SREG,7);

	while(1)
	{
		if(g_count == 4)
		{
			g_count = 0;
			TIMER0_stop();
			INT0_deinit();
			duty = ((float)(g_t3PERIODwHIGH - g_t2PERIOD) / (g_t3PERIODwHIGH - g_t1HIGH)) * 100;
			LCD_intgerToString(duty);
			LCD_displayCharacter('%');
		}
	}

}
