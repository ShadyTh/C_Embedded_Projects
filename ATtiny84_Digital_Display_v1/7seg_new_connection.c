/*
 * 7seg.c
 *
 *  Created on: Mar 1, 2021
 *      Author: Shady Thomas
 */

#include "7seg.h"

/**** CONNECTIONS: *****
 * ATtiny PIN --> SEGMENT
		 * PA1 -> B
		 * PA2 -> A
		 * PA3 -> LOWER_DIGIT (1)
		 * PA4 -> F
		 * PA5 -> G
		 * PA6 -> UPPER_DIGIT (2)
		 * PA7 -> E
		 * PB1 -> C
		 * PB2 -> D
 *
 *	So for PORTA, will use PA1,2,4,5,7 	*** Stored in g_segmentArrayA
 *		   PORTB, will use PB1,2		*** Stored in g_segmentArrayB
 *
 */

#define UPPER_DIGIT 6
#define LOWER_DIGIT 3

const uint8 g_segmentArrayA[10] = {
	 /*   ExGFxABx  */
		0b10010110,		/*0*/
		0b00000010,		/*1*/
		0b10100110,		/*2*/
		0b00100110,		/*3*/
		0b00110010,		/*4*/
		0b00110100,		/*5*/
		0b10110100,		/*6*/
		0b00000110,		/*7*/
		0b10110110,		/*8*/
		0b00110110,		/*9*/
};

const uint8 g_segmentArrayB[10] = {
	 /*   xxxxRDCx   R:Reset */
		0b00001110,		/*0*/
		0b00001010,		/*1*/
		0b00001100,		/*2*/
		0b00001110,		/*3*/
		0b00001010,		/*4*/
		0b00001110,		/*5*/
		0b00001110,		/*6*/
		0b00001010,		/*7*/
		0b00001110,		/*8*/
		0b00001110,		/*9*/
};

/*const uint8 g_segmentArray[10] = {
	//  0b A B C D E F G X
		0b 1 1 1 1 1 1 0 0,		0
		0b 0 1 1 0 0 0 0 0,		1
		0b 1 1 0 1 1 0 1 0,		2
		0b 1 1 1 1 0 0 1 0,		3
		0b 0 1 1 0 0 1 1 0,		4
		0b 1 0 1 1 0 1 1 0,		5
		0b 1 0 1 1 1 1 1 0,		6
		0b 1 1 1 0 0 0 0 0,		7
		0b 1 1 1 1 1 1 1 0,		8
		0b 1 1 1 1 0 1 1 0		9
};*/

void SEG_initV2(void)
{
	DDRA |= 0xFE;	/*output PA7-1*/
	DDRB |= 0x06;	/*output PB2-1*/
	PORTA &= ~0xFE;
	PORTB &= ~0x06;
}

void SEG_displayDigitV2(uint8 num)
{
	PORTA = (PORTA & 0x49) | g_segmentArrayA[num];
	PORTB = (PORTB & 0xF9) | g_segmentArrayB[num];
}

void SEG_displayUpDigitV2(uint8 num)
{
	CLEAR_BIT(PORTA,UPPER_DIGIT);
	SET_BIT(PORTA,LOWER_DIGIT);
	PORTA = (PORTA & 0x49) | g_segmentArrayA[num];
	PORTB = (PORTB & 0xF9) | g_segmentArrayB[num];
}

void SEG_displayLowDigitV2(uint8 num)
{
	CLEAR_BIT(PORTA, LOWER_DIGIT);
	SET_BIT(PORTA, UPPER_DIGIT);
	PORTA = (PORTA & 0x49) | g_segmentArrayA[num];
	PORTB = (PORTB & 0xF9) | g_segmentArrayB[num];
}

void SEG_displayNumV2(uint8 num)
{
	CLEAR_BIT(PORTA,LOWER_DIGIT);
	SET_BIT(PORTA,UPPER_DIGIT);
	PORTA = (PORTA & 0x49) | g_segmentArrayA[num%10];
	PORTB = (PORTB & 0xF9) | g_segmentArrayB[num%10];
	_delay_ms(5);
	CLEAR_BIT(PORTA,UPPER_DIGIT);
	SET_BIT(PORTA,LOWER_DIGIT);
	PORTA = (PORTA & 0x49) | g_segmentArrayA[num/10];
	PORTB = (PORTB & 0xF9) | g_segmentArrayB[num/10];
	_delay_ms(5);
}
