/*
 * MUX_7Segment.c
 *
 *  Created on: Jan 31, 2021
 *      Author: Shady Thomas
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

unsigned char seconds = 0, minutes = 0, hours = 0;
unsigned char digit[6];

void checkClock(void);
void deviseDigits(void);

void INT0_Init(void);
void INT1_Init(void);
void INT2_Init(void);
void TIMER1_CTC_Init(void);

ISR(TIMER1_COMPA_vect)	/* Increment 1 second */
{
	seconds ++;
}

ISR(INT0_vect)		/* RESET */
{
	seconds = 0;
	minutes = 0;
	hours = 0;
}

ISR(INT1_vect)		/* PAUSE (no clock source) */
{
	TCCR1B &= ~(1<<CS10) & ~(1<<CS11);
}

ISR(INT2_vect)		/* RESUME */
{
	TCCR1B |= (1<<CS10) | (1<<CS11);
}

int main()
{
	unsigned char i;

	SREG |= (1<<7);			/* enable global interrupt */

	INT0_Init();
	INT1_Init();
	INT2_Init();

	TIMER1_CTC_Init();


	DDRA |= 0x0F; 			/* set PA3-0 as output */
	PORTA &= 0xF0;			/* initialize PA3-0 low */

	DDRC |= 0x3F;			/* set PC5-0 as output */
	PORTC &= 0xC0;			/* initialize low */

	while(1)
	{
		checkClock();
		deviseDigits();

		for(i=0; i<6; i++)
		{
			PORTC = (PORTC & 0xC0) | ~(1<<i);
			PORTA = (PORTA & 0xF0) | (digit[i] & 0x0F);
			_delay_ms(2);
		}
	}

}

/* checkClock corrects the values of sec, min, and hours
 * after the timer ISR increments the seconds variable  */
void checkClock(void)
{
	if(seconds > 59)
	{
		seconds = 0;
		minutes++;

		if(minutes > 59)
		{
			minutes = 0;
			hours++;

			if(hours > 23)
			{
				hours = 0;
			}
		}
	}
}

/* deviseDigits distributes each digit in the num
 * variables into digits for the 7 segments */
void deviseDigits(void)
{
	digit[0] = seconds%10;
	digit[1] = seconds/10;
	digit[2] = minutes%10;
	digit[3] = minutes/10;
	digit[4] = hours%10;
	digit[5] = hours/10;
}


void TIMER1_CTC_Init(void)
{
	TCCR1A = (1<<FOC1A);
	/* disable OC1A and OC1B, non-PWM mode */

	TCCR1B = (1<<WGM12) | (1<<CS11) | (1<<CS10);
	/* CTC mode 4, pre-scaler = 64 */

	TIMSK |= (1<<OCIE1A);	/* enable OC interrupt */

	TCNT1 = 0;				/* initial value of timer */

	OCR1A = 15625;			/* compare value for incrementing 1 second */

	/* F_CPU = 1MHz, F_Timer = 15.625KHz, T_Timer = 64 usec */

}

void INT0_Init(void)	/* INT0 Resets stopwatch*/
{
	DDRD &= ~(1<<PD2);
	PORTD |= (1<<PD2);		/* set PD2 as input and enable internal pullup */
	GICR |= (1<<INT0);		/* enable INT0 */
	MCUCR |= (1<<ISC01); 	/* falling edge generates the INT request */
}

void INT1_Init(void)	/* INT1 Pauses stopwatch */
{
	DDRD &= ~(1<<PD3);					/* set PD3 as input */
	GICR |= (1<<INT1);					/* enable INT1 */
	MCUCR |= (1<<ISC11) | (1<<ISC10);	/* rising edge generates the INT request */
}

void INT2_Init(void)
{
	DDRB &= ~(1<<PB2);
	PORTB |= (1<<PB2);		/* set PB2 as input and enable internal pullup */
	GICR |= (1<<INT2);		/* enable INT2 */
	MCUCSR &= ~(1<<ISC2); 	/* falling edge of INT2 generates the INT request */
}
