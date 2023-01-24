/*
 * 7seg.c
 *
 *  Created on: Mar 1, 2021
 *      Author: Shady Thomas
 */

#include "7seg.h"

/**** CONNECTIONS: *****
 * ATtiny PIN --> SEGMENT
		 * PA1 -> A
		 * PA2 -> B
		 * PA3 -> C
		 * PA4 -> D
		 * PA5 -> E
		 * PA6 -> F
		 * PA7 -> G
		 */

const uint8 g_segmentArray[10] = {
		0b11111100,		/*0*/
		0b01100000,		/*1*/
		0b11011010,		/*2*/
		0b11110010,		/*3*/
		0b01100110,		/*4*/
		0b10110110,		/*5*/
		0b10111110,		/*6*/
		0b11100000,		/*7*/
		0b11111110,		/*8*/
		0b11110110		/*9*/
};

void SEG_displayDigit(uint8 num)
{
	PORTA = (PORTA & 0x01) | (g_segmentArray[num] & 0xFE);
}

void SEG_displayUpDigit(uint8 num)
{
	PORTB ^= 0xFF;
	PORTA = (PORTA & 0x01) | (g_segmentArray[num] & 0xFE);
}

void SEG_displayLowDigit(uint8 num)
{
	PORTB ^= 0xFF;
	PORTA = (PORTA & 0x01) | (g_segmentArray[num] & 0xFE);
}

void SEG_displayNum(uint8 num)
{
	PORTB = (PORTB & 0xFC) | (0x01);
	PORTA = (PORTA & 0x01) | (g_segmentArray[(num%10)] & 0xFE);
	_delay_ms(5);
	PORTB = (PORTB & 0xFC) | (0x02);
	PORTA = (PORTA & 0x01) | (g_segmentArray[(num/10)] & 0xFE);
	_delay_ms(5);
}
