/*
 * Z_max7219_Temp.c
 *
 *  Created on: Feb 10, 2021
 *      Author: Shady Thomas
 *
 *      This is a functional Venus Control Board Prototype
 *      EWH Control Board
 *      -	using max7219 to multiplex 8 7-SEG Displays, communcation using SPI
 *      -	2 push buttons to increment/decrement set temperature
 *      -	1 10K NTC sensor, read using ADC and voltage divider and converted to deg. Celsius
 *      -	Using a relay on PD2 to compare between set Temp and actual Temp.
 *
 *      Future Improvements:
 *      	- Buzzer
 *      	- IR Remote Control
 *      	- Hysteresis to control Relay
 *      	- User Set Timer
 *      	- Display Current Time
 *      	- Fluctuation Cancellation on Display (check out ATtiny84 Temp Display Project)
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <math.h>
#include "max7219.h"

#define SIZE 13

	unsigned char digitsInUse = 4;
	unsigned char setTemp = 50;
    unsigned char ntcTemp;

    unsigned char f_Increment = 0;
    unsigned char f_Decrement = 0;
    unsigned char f_ADC = 0;


void spiSendByte (char databyte)
{
    // Copy data into the SPI data register
    SPDR = databyte;
    // Wait until transfer is complete
    while (!(SPSR & (1 << SPIF)));
}

void MAX7219_writeData(char data_register, char data)
{
    MAX7219_LOAD0;
        // Send the register where the data will be stored
        spiSendByte(data_register);
        // Send the data to be stored
        spiSendByte(data);
    MAX7219_LOAD1;
}

void MAX7219_clearDisplay()
{
    char i = digitsInUse;
    // Loop until 0, but don't run for zero
    do {
        // Set each display in use to blank
        MAX7219_writeData(i, MAX7219_CHAR_BLANK);
    } while (--i);
}

void MAX7219_displayNumber(volatile long number)
{

    // Initialization to 0 required in this case,
    // does not work without it. Not sure why.
    char i = 0;

    // Loop until number is 0.
    do {
        MAX7219_writeData(++i, number % 10);
        // Actually divide by 10 now.
        number /= 10;
    } while (number);
}

void MAX7219_display_setTemp(volatile char number)
{
	MAX7219_writeData(MAX7219_DIGIT0, number % 10);
	MAX7219_writeData(MAX7219_DIGIT1, (number/10) % 10);
}

void MAX7219_display_ntcTemp(volatile char number)
{
	MAX7219_writeData(MAX7219_DIGIT2, number % 10);
	MAX7219_writeData(MAX7219_DIGIT3, (number/10) % 10);
}

void MAX7219_Init()
{
	// SCK MOSI CS/LOAD/SS
	    DDRB |= (1 << PIN_SCK) | (1 << PIN_MOSI) | (1 << PIN_SS);

	    // SPI Enable, Master mode
	    SPCR |= (1 << SPE) | (1 << MSTR);// | (1<<SPR1);

	    // SPI2x
	    SPSR |= (1<<SPI2X);

	    // Decode mode to "Font Code-B"
	    MAX7219_writeData(MAX7219_MODE_DECODE, 0xFF);

	    // Scan limit runs from 0.
	    MAX7219_writeData(MAX7219_MODE_SCAN_LIMIT, digitsInUse - 1);
	    MAX7219_writeData(MAX7219_MODE_INTENSITY, 10);
	    MAX7219_writeData(MAX7219_MODE_POWER, ON);
}

/********************************************/
/*
 * 			    ADC Functions
 */
/********************************************/

int val;
long R;
double ntc;

void ADC_Init()
{
	DDRA &= ~(1<<PA0);			/* set PA0 as input*/
	/*	Enable ADC, Pre-scaler = 128*/
	ADCSRA |= (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
	/* 	Reference is AVcc and MUX channel 0*/
	ADMUX |= (1<<REFS0);
}

int ADC_val()
{
	ADCSRA |= (1<<ADSC);	/* Start ADC Conversion*/
	while( (ADCSRA & (1<<ADIF)) == 0);	/* wait for conversion to finish (Polling)*/
	ADCSRA |= (1<<ADIF);		/* Clear the flag bit by writing a 1 to it */
	return((int)ADCW);
}

double getTemp()
{
	unsigned char i = 0;
	unsigned long ARR_ADC_val[SIZE];
	unsigned long long sum = 0;
	for(i=0;i<SIZE;i++)
	{
		ARR_ADC_val[i] = ADC_val();
		sum+= ARR_ADC_val[i];
	}
	val = sum/SIZE;

//	val = ADC_val();

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

void INT0_Init(void)	/* INT0 Decreases Set Temp */
{
	DDRD &= ~(1<<PD2);
	PORTD |= (1<<PD2);		/* set PD2 as input and enable internal pullup */
	GICR |= (1<<INT0);		/* enable INT0 */
	MCUCR |= (1<<ISC01); 	/* falling edge generates the INT request */
}

void INT1_Init(void)	/* INT1 Increases Set Temp */
{
	DDRD &= ~(1<<PD3);
	PORTD |= (1<<PD3);		/* set PD3 as input and enable internal pullup */
	GICR |= (1<<INT1);		/* enable INT1 */
	MCUCR |= (1<<ISC10);	/* falling edge generates the INT request */
}

ISR(INT1_vect)	/* Increment Set Temp */
{
	f_Increment = 1;

}

ISR(INT0_vect)	/* Decrement Set Temp */
{
	f_Decrement = 1;
}

void TIMER1_CTC_Init(void)
{
	TCCR1A = (1<<FOC1A);
	/* disable OC1A and OC1B, non-PWM mode */

	TCCR1B = (1<<WGM12) | (1<<CS11) | (1<<CS10);
	/* CTC mode 4, pre-scaler = 64 */

	TIMSK |= (1<<OCIE1A);	/* enable OC interrupt */

	TCNT1 = 0;				/* initial value of timer */

	OCR1A = 15625;			/* compare value for incrementing 1 second * 2 */

	/* F_CPU = 1MHz, F_Timer = 15.625KHz, T_Timer = 64 usec */

}

ISR(TIMER1_COMPA_vect)	/* Increment 1 second */
{
	f_ADC = 1;
}

int main(void)
{

	/*static unsigned char error_counter = 0;
	unsigned char prev_ntcTemp;
*/
	MAX7219_Init();
    ADC_Init();
    INT1_Init();
    INT0_Init();
    TIMER1_CTC_Init();

    DDRA |= (1<<PD2);


    SREG |= (1<<7);

    //prev_ntcTemp = getTemp();

    while(1)
    {
    	if(f_Increment)
    	{
    		setTemp++;
    		if(setTemp > 80)
    				setTemp = 80;
    		f_Increment = 0;
    	}

    	if(f_Decrement)
    	{
    		setTemp --;
			if(setTemp < 40)
				setTemp = 40;
			f_Decrement = 0;
    	}

    	if(f_ADC)
    	{
    		ntcTemp = getTemp();
    		f_ADC = 0;
    	}

    	if(ntcTemp < setTemp)	//actual temp is lower, so turn on heater
    	{
    		PORTA |= (1<<PD2);
    	}
    	else					//actual temp is higher, so turn it off
    	{
    		PORTA &= ~(1<<PD2);
    	}

        MAX7219_display_setTemp(setTemp);
        MAX7219_display_ntcTemp(ntcTemp);

   /*     if((error_counter == 5) && (prev_ntcTemp != ntcTemp))
        {
        	error_counter = 0;
        	prev_ntcTemp = ntcTemp;
        }

        if(prev_ntcTemp == ntcTemp)
        {
        	MAX7219_display_ntcTemp(ntcTemp);
        }
        else if(prev_ntcTemp != ntcTemp)
        {
        	MAX7219_display_ntcTemp(prev_ntcTemp);
        	error_counter++;
        }*/

    }
}
