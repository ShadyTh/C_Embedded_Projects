/*
 * adc.c
 *
 *  Created on: Mar 1, 2021
 *      Author: Shady Thomas
 */

#include "adc.h"

static volatile void (*g_callbackPtr) (void) = NULL_PTR;

ISR(ADC_vect)
{
	if(g_callbackPtr != NULL_PTR)
		(*g_callbackPtr)();
}

void ADC_initINT()
{
/*	set PA0 as input */
	DDRA &= ~(1<<PA0);

	/*adc channel 0, reference is Vcc*/
	ADMUX = 0x00;

	/*Enable ADC module, set pre-scaler = 8
	 ADC_clock must be between 50KHz and 200KHz
	 with prescaler = 8, F_CPU = 1MHz,
	 ADC_clk = 125KHz
	 INTERRUPT ENABLE
	 */
	ADCSRA = (1<<ADEN) | (1<<ADPS1) | (1<<ADIE);

/*	ADLAR = 0, Disable Auto Trigger Source
	ADCSRB = 0x00;*/
}

void ADC_readValInterrupt(void)
{
	SET_BIT(ADCSRA,ADSC);				/*start conversion*/
}

void ADC_setCallback(void (*ptr) (void))
{
	g_callbackPtr = ptr;
}

void ADC_init()
{
/*	set PA0 as input */
	DDRA &= ~(1<<PA0);

	/*adc channel 0, reference is Vcc*/
	ADMUX = 0x00;

	/*Enable ADC module, set pre-scaler = 8
	 ADC_clock must be between 50KHz and 200KHz
	 with prescaler = 8, F_CPU = 1MHz,
	 ADC_clk = 125KHz
	 */
	ADCSRA = (1<<ADEN) | (1<<ADPS1);

/*	ADLAR = 0, Disable Auto Trigger Source
	ADCSRB = 0x00;*/
}

uint16 ADC_readVal(void)
{
	SET_BIT(ADCSRA,ADSC);				/*start conversion*/
	while(BIT_IS_CLEAR(ADCSRA,ADIF));	/*wait for conversion to finish*/
	SET_BIT(ADCSRA,ADIF);				/*clear flag*/
	return ADC;
}
