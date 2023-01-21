/*
 * sh_lcd.c
 *
 *  Created on: Feb 15, 2021
 *      Author: Shady Thomas
 */

#include "sh_lcd.h"
#include <stdlib.h>

void LCD_sendCommand(uint8 command)
{
	CLEAR_BIT(LCD_CTRL_PORT,RS);
	CLEAR_BIT(LCD_CTRL_PORT,RW);
	_delay_ms(1);
	SET_BIT(LCD_CTRL_PORT,E);
	_delay_ms(1);

#if DATA_BITS_MODE == 4

	/* 1. Sends the higher 4 bits of the command
	 *  2. Clears and Sets Enable
	 *  3. Sends the lower 4 bits of the command*/

#ifdef UPPER_PORT_PINS
	LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | (command & 0xF0);
#else
	LCD_DATA_PORT = (LCD_DATA_PORT & 0xF0) | ((command & 0xF0) >> 4);
#endif

	_delay_ms(1);
	CLEAR_BIT(LCD_CTRL_PORT,E);
	_delay_ms(1);
	SET_BIT(LCD_CTRL_PORT,E);
	_delay_ms(1);

#ifdef UPPER_PORT_PINS
	LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | ((command & 0x0F) << 4);
#else
	LCD_DATA_PORT = (LCD_DATA_PORT & 0xF0) | (command & 0x0F);
#endif

#elif (DATA_BITS_MODE == 8)
	LCD_DATA_PORT = command;
#endif

	_delay_ms(1);
	CLEAR_BIT(LCD_CTRL_PORT,E);
	_delay_ms(1);
}

void LCD_displayCharacter(uint8 command)
{
	SET_BIT(LCD_CTRL_PORT,RS);
	CLEAR_BIT(LCD_CTRL_PORT,RW);
	_delay_ms(1);
	SET_BIT(LCD_CTRL_PORT,E);
	_delay_ms(1);

#if DATA_BITS_MODE == 4

	/* 1. Sends the higher 4 bits of the command
	 *  2. Clears and Sets Enable
	 *  3. Sends the lower 4 bits of the command*/

#ifdef UPPER_PORT_PINS
	LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | (command & 0xF0);
#else
	LCD_DATA_PORT = (LCD_DATA_PORT & 0xF0) | ((command & 0xF0) >> 4);
#endif

	_delay_ms(1);
	CLEAR_BIT(LCD_CTRL_PORT,E);
	_delay_ms(1);
	SET_BIT(LCD_CTRL_PORT,E);
	_delay_ms(1);

#ifdef UPPER_PORT_PINS
	LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | ((command & 0x0F) << 4);
#else
	LCD_DATA_PORT = (LCD_DATA_PORT & 0xF0) | (command & 0x0F);
#endif

#elif (DATA_BITS_MODE == 8)
	LCD_DATA_PORT = command;
#endif

	_delay_ms(1);
	CLEAR_BIT(LCD_CTRL_PORT,E);
	_delay_ms(1);
}

void LCD_init(void)
{
	LCD_CTRL_PORT_DIR |= (1<<RS) | (1<<RW) | (1<<E);		/* CTRL Pins are output */

#if DATA_BITS_MODE == 4
	#ifdef UPPER_PORT_PINS
		LCD_DATA_PORT_DIR |= 0xF0;
	#else
		LCD_DATA_PORT_DIR |= 0x0F;
	#endif
	LCD_sendCommand(FOUR_BITS_DATA_MODE);				/* init LCD in 4-bit mode	*/
	LCD_sendCommand(TWO_LINE_LCD_Four_BIT_MODE);	/*	use 2 line lcd, 4bit mode*/

#elif DATA_BITS_MODE == 8
	LCD_DATA_PORT_DIR = 0xFF;		/*	Data Port is output*/
	LCD_sendCommand(TWO_LINE_LCD_Eight_BIT_MODE);	/*	use 2 line lcd, 8bit mode*/
#endif

	LCD_sendCommand(CURSOR_OFF);
	LCD_sendCommand(CLEAR_COMMAND);
}

void LCD_displayString(const char  * Str)
{
	while((*Str) != '\0')
	{
		LCD_displayCharacter(*Str);
		Str++;
	}
}

void LCD_displayString100Delay(const char  * Str)
{
	while((*Str) != '\0')
	{
		LCD_displayCharacter(*Str);
		Str++;
		_delay_ms(100);
	}
}

void LCD_displayStringDelay(const char  * Str, uint16 delay)
{
	while((*Str) != '\0')
	{
		LCD_displayCharacter(*Str);
		Str++;
		_delay_ms(delay);
	}
}

void LCD_shiftLeft(uint8 loops, uint16 delay)
{
	while(loops>0)
	{
		LCD_sendCommand(SHIFT_LEFT);
		loops--;
		_delay_ms(delay);
	}
}

void LCD_displayStringDelayShiftL(const char  * Str, uint16 delay)
{
	while((*Str) != '\0')
	{
		LCD_displayCharacter(*Str);
		Str++;
		LCD_sendCommand(SHIFT_LEFT);
		_delay_ms(delay);
	}
}

void LCD_goToRowColumn(uint8 row, uint8 col)
{
	uint8 address = 0;
	switch (row)
	{
		case 0:	address = col;
			break;
		case 1:	address = 0x40 + col;
			break;
		case 2:	address = 0x10 + col;
			break;
		case 3:	address = 0x50 + col;
			break;
	}

	LCD_sendCommand(address | SET_CURSOR_LOCATION);
}

void LCD_displayStringRowColumn(uint8 row ,uint8 col, const char * Str)
{
	LCD_goToRowColumn(row, col);
	LCD_displayString(Str);
}

void LCD_clearScreen(void)
{
	LCD_sendCommand(CLEAR_COMMAND);
}

void LCD_displayInteger(sint16 num)
{
	char buff[16];
	itoa(num,buff,10);
	LCD_displayString(buff);
}
