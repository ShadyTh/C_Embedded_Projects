/*
 * sh_lcd.h
 *
 *  Created on: Feb 14, 2021
 *      Author: Shady Thomas
 */

#ifndef SH_LCD_H_
#define SH_LCD_H_

/*		Library Includes		*/
#include "common_macros.h"
#include "micro_config.h"
#include "std_types.h"

/*	Configure for 4bit Mode		*/
#define DATA_BITS_MODE		8

#if DATA_BITS_MODE == 4
#undef UPPER_PORT_PINS	/*Undef to use lower 4bits*/
#endif

/*		Pin Definitions		*/
#define RS 	PB0
#define RW 	PB1
#define E 	PB2
#define LCD_CTRL_PORT			PORTB
#define LCD_CTRL_PORT_DIR		DDRB

#define LCD_DATA_PORT		PORTA
#define LCD_DATA_PORT_DIR	DDRA

/*		LCD Commands		*/
#define CLEAR_COMMAND			0x01
#define FOUR_BITS_DATA_MODE 	0X02
#define TWO_LINE_LCD_Four_BIT_MODE		0x28
#define TWO_LINE_LCD_Eight_BIT_MODE		0x38
#define	CURSOR_OFF				0x0C
#define CURSOR_ON				0x0E
#define CURSOR_BLINK			0x0F
#define SET_CURSOR_LOCATION		0x80
#define SHIFT_LEFT 				0x18


/*		Function Prototypes		*/
void LCD_sendCommand(uint8 command);
void LCD_displayCharacter(uint8 data);
void LCD_init(void);
void LCD_displayString(const char * Str);
void LCD_goToRowColumn(uint8 row, uint8 col);
void LCD_displayStringRowColumn(uint8 row ,uint8 col, const char * Str);
void LCD_clearScreen(void);
void LCD_displayInteger(sint16 num);

void LCD_displayString100Delay(const char  * Str);
void LCD_displayStringDelay(const char  * Str, uint16 delay);
void LCD_shiftLeft(uint8 loops, uint16 delay);
void LCD_displayStringDelayShiftL(const char  * Str, uint16 delay);

#endif /* SH_LCD_H_ */
