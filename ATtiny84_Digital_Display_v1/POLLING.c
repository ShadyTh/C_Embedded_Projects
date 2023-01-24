/*
 * test.c
 *
 *  Created on: Mar 1, 2021
 *      Author: Shady Thomas
 */


#include "7seg.h"
#include "adc.h"
#include "timer1.h"
#include <math.h>

uint8 g_adc = 0;
uint16 val;
float32 R;
float64 ntc;

uint8 upper_digit;
uint8 lower_digit;
uint8 ntcTemp = 0;

void APP_raiseAdcFlag(void)
{
	g_adc = 1;
}

uint8 getTemp()
{
/*
	unsigned char i = 0;
	unsigned long ARR_ADC_val[SIZE];
	unsigned long long sum = 0;
	for(i=0;i<SIZE;i++)
	{
		ARR_ADC_val[i] = ADC_val();
		sum+= ARR_ADC_val[i];
	}
	val = sum/SIZE;
*/

	val = ADC_readVal();

	R = ((10230000/val) - 10000);
	ntc = log(R);


	/* Steinhart_Hart Thermistor Equation
	 * Temp. (Kelvin) = 1 / (A + B [ln(R)] + C [ln(R)]^3)
	 * where A = 0.001129148 ,B = 0.000234125, C = 8.76741*10^-8
*/
	ntc = 1 / (0.001129148 + (0.000234125 * ntc) + (0.0000000876741 * ntc * ntc * ntc));
	ntc = ntc - 273.15;		/* convert Kelvin to Celsius*/

	return(ntc);
}

int main()
{
	SEG_initV2();
	ADC_init();

	TIMER1_CTC_init();
	TIMER1_setCallback(APP_raiseAdcFlag);

	ntcTemp = getTemp();

	SREG |= (1<<7);

	while(1)
	{
		if(g_adc)
		{
			ntcTemp = getTemp();
			g_adc = 0;
		}

		upper_digit = ntcTemp/10;
		lower_digit = ntcTemp%10;

		SEG_displayUpDigitV2(upper_digit);
		_delay_ms(2);
		SEG_displayLowDigitV2(lower_digit);
		_delay_ms(1.25);
	}
}

/***********************************
 *
 *
 * How to knwo stack size:
 * 1st flash this code to paint the stack
	 while(1)
	 {
	  uint8*ptr = (uint8 *) 0x100;
	  *ptr = 0xEE;

	  ptr++;
	  }
 *
 * 2nd flash your own code
 *
 * 3rd flash a code which increments everybyte till you reACH 0xEE
 *
 */

