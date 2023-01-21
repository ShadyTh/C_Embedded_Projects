/*
 * sh_adc.c
 *
 *  Created on: February 17, 2021
 *      Author: Shady
 */

#include "sh_adc.h"

void ADC_init()
{
	ADMUX = 0;		/* Reference voltage = AREF, ADLAR = 0, MUX channel 0*/
	/* Enable ADC, Pre-scaler F CPU /8 = 1MHz/8 = 125KHz
	 * 	ADC must operate between 50 - 200 KHz			*/
	ADCSRA = (1<<ADEN) | (1<<ADPS1) | (1<<ADPS0);
}

uint16 ADC_readChannel(uint8 channel)
{
	ADMUX = (ADMUX & 0xE0) | (channel & 0x07);	/* select channel*/
	SET_BIT(ADCSRA,ADSC);		/* Start Conversion */
	while(BIT_IS_CLEAR(ADCSRA,ADIF));	/* Polling until flag is raised */
	SET_BIT(ADCSRA,ADIF);		/* clear flag by writing 1 to it */
	return ADCW;				/* returns read value; i.e. ADCH and ADCL contents */
}
