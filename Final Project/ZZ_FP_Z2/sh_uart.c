/*
 * sh_uart.c
 *
 *  Created on: Feb 25, 2021
 *      Author: Shady Thomas
 */


#include "sh_uart.h"

#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 8UL))) - 1)

void UART_initConfig(const UART_CONFIG_TYPE * config_ptr)
{
	/* U2X to double transmission speed*/
	UCSRA = (1<<U2X);

	/*UCSRB Register Description:
	1. RXCIE, TXCIE, UDRIE all = 0. Disable all interrupts
	2. TXEN, RXEN = 1. Enable Receiver and Transmitter.
	3. UCSZ2 = 0. will use 8bit data. RXB8,TXB8 not used either.*/

	UCSRB = (1<<RXEN) | (1<<TXEN);

	/*UCSRC Register Description:
	1. URSEL = 1 To write in UCSRC.
	2. UMSEL = 0 for asynchronous
	3. UPM1-0 = UART_CONFIG_TYPE -> UART_PARITY_MODE
	4. USBS   = UART_CONFIG_TYPE -> UART_STOP_BIT
	5. UCPOL  = 0 synchronous mode not used.
	6. UCSZ1-0 = UART_CONFIG_TYPE -> UART_CHARACTER_SIZE */

	UCSRC = (1<<URSEL) | ((config_ptr -> parity_mode) << 4) | ((config_ptr -> stop_bit_select) << 3) | ((config_ptr -> character_size) << 1);

/*********************************************************************
 * ********     This method doesn't work  ****************************
 * *****  The register has to be written in one instruction,**********

  	UCSRC = (1<<URSEL) | (UCSRC & 0xCF) | (config_ptr -> parity_mode);
	UCSRC = (1<<URSEL) | (UCSRC & 0xF7) | (config_ptr -> stop_bit_select);
	UCSRC = (1<<URSEL) | (UCSRC & 0xF9) | (config_ptr -> character_size);

**********************************************************************/

	UBRRH = ( (config_ptr -> bit_rate) >> 8);
	UBRRL = (config_ptr -> bit_rate);
}

void UART_sendPass(const uint8 * ptr)
{
	uint8 i;
	for(i=0; i<5; i++)
	{
		UART_sendByte(*ptr);
		ptr++;
	}
}

void UART_receivePass(uint8 * ptr)
{
	uint8 i;
	for(i=0; i<5; i++)
	{
		*ptr = UART_receiveByte();
		ptr++;
	}
}


void UART_init()
{
	UCSRA = (1<<U2X);

	/*UCSRB Register Description:
	1. RXCIE, TXCIE, UDRIE all = 0. Disable all interrupts
	2. TXEN, RXEN = 1. Enable Reciever and Transmitter.
	3. UCSZ2 = 0. will use 8bit data. RXB8,TXB8 not used either.*/

	UCSRB = (1<<RXEN) | (1<<TXEN);

	/*UCSRC Register Description:
	1. URSEL = 1. To write in UCSRC.
	2. UMSEL =0 for asynchronous
	3. UPM1-0 = 0. Disable Parity
	4. USBS = 0. 1 stop bit
	5. UCPOL = 0. synch. mode not used.
	6. UCSZ1-0 = 11 for 8bit data mode*/

	UCSRC = (1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0);


	UBRRH = (BAUD_PRESCALE>>8);
	UBRRL = BAUD_PRESCALE;
}

void UART_sendByte(const uint8 data)
{
	//wait for send buffer to be empty
	while(BIT_IS_CLEAR(UCSRA,UDRE));
	UDR = data;

	/***	Another method: ***
	 * UDR = data;
	 * while(BIT_IS_CLEAR(UCSRA,TXC);	//wait until transmission is complete, TXC = 1
	 * SET_BIT(UCSRA,TXC);				// clear txc flag by writing 1 to it
	 * */
}

uint8 UART_receiveByte(void)
{
	while(BIT_IS_CLEAR(UCSRA,RXC));		//wait till receive is complete, RXC = 1
	return UDR;							// return read data
}

void UART_sendString(const uint8 * str)
{
	while(*str != '\0')
	{
		UART_sendByte(*str);
		str++;
	}
}

void UART_receiveString(uint8 * str)
{
		*str = UART_receiveByte();
		while(*str != '#')
		{
			str++;
			*str = UART_receiveByte();
		}
	*str = '\0';
}
