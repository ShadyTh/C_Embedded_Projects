/*
 * ATtiny84
 * After testing the PCB on a Marina 80L
 * The readings are still fluctuating
 * So "tempChangeCount" will be increased from 5 to 10
 *
 *  Created on: Mar 1, 2021
 *      Author: Shady Thomas
 */


#include "7seg.h"
#include "adc.h"
#include "timer1.h"
#include "timer0.h"
#include <math.h>

uint8 g_adc = 0;	/*flag for adc conversion to start*/

uint16 val;
float32 R;
float64 ntc;

uint8 upper_digit;
uint8 lower_digit;
uint8 ntcTemp = 0;
uint8 ntcTemp_old = 0;
uint8 tempChangeCount = 0;

uint8 g_tick_t0 = 0;
uint8 g_tick_t1 = 0;

 /*Raise ADC Flag every 0.5 second*/
void APP_raiseAdcFlag(void)
{
	SREG |= (1<<7);			/*enable nested interrupts*/

	g_tick_t0 ++;
	if(g_tick_t0 == 2)		/*after 0.5 second (2*250ms) */
		{
			g_adc = 1;
			g_tick_t0 = 0;
		}
}

 /*Multiplex the two digit with a refresh rate each 8 ms*/
void APP_multiplex(void)
{
	g_tick_t1 ++;

	if(g_tick_t1 == 1)
		SEG_displayUpDigitV2(upper_digit);
	else if(g_tick_t1 == 2)
		{
		SEG_displayLowDigitV2(lower_digit);
		g_tick_t1 = 0;
		}
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

	/*start adc conversion and store the value in val*/

	val = ADC_readVal();

	/*convert adc value to resistance by voltage divider equation*/
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
	/*first initialize all modules:
	7 segment, ADC, Timer1, Timer0
	then enable interrupts */

	SEG_initV2();
	ADC_init();

	TIMER1_CTC_init();
	TIMER1_setCallback(APP_multiplex);

	TIMER0_CTC_init();
	TIMER0_setCallback(APP_raiseAdcFlag);

    /*in the first time, read the ntcTemp and place it in both old and new variables*/
	ntcTemp = getTemp();
	ntcTemp_old = ntcTemp;

	SREG |= (1<<7);

	while(1)
	{
		/*check for ADC global flag, i.e; ADC ISR was executed*/
		if(g_adc)
		{
			/*go and getTemp i.e; execute ADC Conversion*/
			ntcTemp = getTemp();

			/*check if old and new temperature readings dont match*/
			if(ntcTemp != ntcTemp_old)
			{
				/*increment count of different temperature changes*/
				tempChangeCount ++;
				/*if count of temperature changes exceeds 5 times,
				 * i.e; the temperature has definitely changed
				 * assign new temp to old temp*/
				if(tempChangeCount < 10)
					/*if count is less than 5, keep the old temp,
					i.e; the temp is still fluctating*/
					ntcTemp = ntcTemp_old;
				else
				{
					ntcTemp_old = ntcTemp;
					tempChangeCount = 0;
				}
			}
			/*if both new and old temperature readings are equal,
			i.e; the reading is stable, make change count = zero*/
			else
			{
				tempChangeCount = 0;
			}
			/*clear adc ISR flag*/
			g_adc = 0;
		}

		/*devise the ntcTemp between the lower and upper digits*/
		upper_digit = ntcTemp/10;
		lower_digit = ntcTemp%10;

	}
}
