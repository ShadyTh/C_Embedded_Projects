/*
 * uart_communication.h
 *
 *  Created on: Apr 21, 2021
 *      Author: Shady Thomas
 */

#ifndef UART_COMMUNICATION_H_
#define UART_COMMUNICATION_H_

#define MC2_READY 	0x88
#define MC1_READY 	0x77
#define ENTER 		0xEE

#define NEW_PASS 			0xA0
#define NOT_MATCHING		0XA1
#define PASS_SAVED_SUCCESS	0xA2
#define MAIN_MENU			0xA3
#define CHANGE_PASS_MODE	0xA4
#define UNLOCK_DOOR_MODE	0xA5
#define CHECK_PASS			0xA6
#define DOOR_UNLOCKING		0xA7
#define ERROR_TRIALS_LEFT	0xA8
#define ALERT				0xA9

#endif /* UART_COMMUNICATION_H_ */
