/*
 * keypad.c
 *
 *  Created on: Feb 14, 2021
 *      Author: Shady Thomas
 */

#include "sh_keypad.h"

#if N_COL == 3

static uint8 KEYPAD_4x3_adjustKeypadNumber(uint8 number);

#elif N_COL == 4

static uint8 KEYPAD_4x4_adjustKeypadNumber(uint8 number);

#endif


uint8 KEYPAD_getPressedKey(void)
{
	while(1)
	{
		uint8 row, col;

		for(col = 0; col < N_COL ; col++)
		{
			KEYPAD_PORT_DIR = (0x10 << col);
			KEYPAD_PORT_OUT = (~(0x10<<col));

			for(row = 0; row < N_ROW ; row++)
			{
				if(BIT_IS_CLEAR(KEYPAD_PORT_IN,row))
				{
					#if N_COL == 3
						return KEYPAD_4x3_adjustKeypadNumber((row*N_COL)+col+1);
					#elif N_COL == 4
						return KEYPAD_4x4_adjustKeypadNumber((row*N_COL)+col+1);
					#endif
				}
			}
		}
	}
}

#if N_COL == 3

static uint8 KEYPAD_4x3_adjustKeypadNumber(uint8 number)
{
	switch(number)
	{
		case 10:		return '*';
				break;
		case 11:		return 0;
				break;
		case 12:		return '#';
				break;
		default: return number;
	}
}

#elif N_COL == 4

static uint8 KEYPAD_4x4_adjustKeypadNumber(uint8 number)
{
	switch(number)
	{
		case 1: return 7;
				break;
		case 2: return 8;
				break;
		case 3: return 9;
				break;
		case 4: return '/';
				break;
		case 5: return 4;
				break;
		case 6: return 5;
				break;
		case 7: return 6;
				break;
		case 8: return '*';
				break;
		case 9: return 1;
				break;
		case 10: return 2;
				break;
		case 11: return 3;
				break;
		case 12: return '-';
				break;
		case 13: return 13;
				break;
		case 14: return 0;
				break;
		case 15: return '=';
				break;
		case 16: return '+';
				break;
		default: return number;
	}
}
#endif
