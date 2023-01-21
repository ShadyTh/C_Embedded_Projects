/*
 * sh_uart.h
 *
 *  Created on: Feb 25, 2021
 *      Author: Shady Thomas
 */

#ifndef SH_UART_H_
#define SH_UART_H_

#include "common_macros.h"
#include "micro_config.h"
#include "std_types.h"

#define USART_BAUDRATE 9600

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
	STOP_1BIT, STOP_2BIT
}UART_STOP_BIT_SELECT;

typedef enum
{
	DISABLE_PARTITY, EVEN_PARITY = 2, ODD_PARITY
}UART_PARITY_MODE;

/* 				UART Character Size
 * Character Size 9Bit not included to simplify init function*/
typedef enum
{
	SIZE_5BIT, SIZE_6BIT, SIZE_7BIT, SIZE_8BIT
}UART_CHARACTER_SIZE;

 /*Bit Rates for MCU 8MHz clock Datasheet Table 70, U2X = 1 */
typedef enum
{
	BIT_RATE_2400 = 416,
	BIT_RATE_4800 = 207,
	BIT_RATE_9600 = 103,
	BIT_RATE_14400 = 68,
	BIT_RATE_19200 = 51
}UART_BIT_RATE_8MHZ;

/*Bit Rates for MCU 1MHz clock Datasheet Table 68, U2X = 1 */
typedef enum
{
	BIT_RATE_2400_1MHZ  = 51,
	BIT_RATE_4800_1MHZ  = 25,
	BIT_RATE_9600_1MHZ  = 12,
	BIT_RATE_14400_1MHZ = 8,
	BIT_RATE_19200_1MHZ = 6
}UART_BIT_RATE_1MHZ;

typedef struct
{
	UART_BIT_RATE_8MHZ   bit_rate;
	UART_CHARACTER_SIZE  character_size;
	UART_PARITY_MODE     parity_mode;
	UART_STOP_BIT_SELECT stop_bit_select;
}UART_CONFIG_TYPE;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void UART_init();
void UART_sendByte(const uint8 data);
uint8 UART_receiveByte(void);
void UART_sendString(const uint8 * str);
void UART_receiveString(uint8 * str);

void UART_initConfig(const UART_CONFIG_TYPE * config_ptr);
void UART_sendPass(const uint8 * ptr);
void UART_receivePass(uint8 * ptr);

#endif /* SH_UART_H_ */
