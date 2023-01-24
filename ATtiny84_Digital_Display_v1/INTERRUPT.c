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

void APP_raiseAdcFlag(void)
{
	ADC_readValInterrupt();
}

void APP_adcFinished(void)
{
	val = ADC;
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
	uint8 upper_digit;
	uint8 lower_digit;
	uint8 ntcTemp = 0;

	SEG_initV2();

	ADC_initINT();
	ADC_setCallback(APP_adcFinished);

	TIMER1_CTC_init();
	TIMER1_setCallback(APP_raiseAdcFlag);

	ntcTemp = getTemp();

	SREG |= (1<<7);

	while(1)
	{
		ntcTemp = getTemp();

		upper_digit = ntcTemp/10;
		lower_digit = ntcTemp%10;

		SEG_displayLowDigitV2(lower_digit);
		_delay_ms(5);
		SEG_displayUpDigitV2(upper_digit);
		_delay_ms(3);
	}
}
