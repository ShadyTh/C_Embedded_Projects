/*
 * mini_project_2.c
 *
 *  Created on: Feb 18, 2021
 *      Author: Shady Thomas
 */

#include "sh_lcd.h"
#include "sh_adc.h"
#include "timer0_pwm.h"
#include "int1.h"

uint8 g_motor_direction = 0x40;

int main()
{
	uint16 duty = 0;
	uint16 prev_duty = 0;		/* Variable to prevent overlapping of LCD Values */

	ADC_init();
	LCD_init();
	LCD_displayString("ADC value: ");
	LCD_displayStringRowColumn(1,0,"Shady Thomas");
	TIMER0_PWM_init();

	DDRB |= (1<<PB6) | (1<<PB7);									/*set motor pins direction to be output*/
	PORTB = (PORTB & 0x3F) | (g_motor_direction & 0xC0);			/*set motor direction to CW*/
	INT1_init();

	SREG |= (1<<7);		/* Enable Global Interrupt */

	while(1)
	{
		LCD_goToRowColumn(0,11);
		duty = ADC_readChannel(0);		/* Record ADC Value of Potentiometer */

		/* To prevent numbers from overlapping on LCD */
		if(prev_duty != duty)
		{
			LCD_displayString("    ");   /* Display Spaces/Blank to overwrite extra digits */
			prev_duty = duty;
			LCD_goToRowColumn(0,11);
		}

		LCD_displayInteger(duty);
		TIMER0_PWM_setDutyCycle(duty/4);  /* Output PWM with duty cycle value */
	}
}
