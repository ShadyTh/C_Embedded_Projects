/*
 * keypad.h
 *
 *  Created on: Feb 14, 2021
 *      Author: Shady Thomas
 */

#ifndef SH_KEYPAD_H_
#define SH_KEYPAD_H_

#include "std_types.h"
#include "micro_config.h"
#include "common_macros.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/*************************
 * 		  Defines
 *************************/

#define N_ROW	4
#define N_COL	4

#define KEYPAD_PORT_DIR	DDRC
#define	KEYPAD_PORT_OUT	PORTC
#define	KEYPAD_PORT_IN	PINC

uint8 KEYPAD_getPressedKey();

#endif /* SH_KEYPAD_H_ */
