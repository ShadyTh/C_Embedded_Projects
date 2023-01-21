/*
 * sh_i2c.c
 *
 *  Created on: Feb 27, 2021
 *      Author: Shady Thomas
 */


#include "sh_i2c.h"

void TWI_initConfig(const I2C_CONFIG_TYPE * config_ptr)
{
	/* always using zero pre-scaler TWPS = 00, FCPU = 8MHz
	 * TWBR = ((F_CPU / i2c_bit_rate) - 16) / 2
	 */

	TWBR = ((F_CPU /(config_ptr -> i2c_bit_rate)) - 16) / 2;
	TWSR = 0x00;

	/* My address */
	TWAR = ((config_ptr -> slave_address)<<1);

	/* Enable TWI at the end, after all configurations have been made */
	TWCR = (1<<TWEN);
}

void TWI_init(void)
{
	/* 				Default (Normal) Initialization
	 * Bit Rate = 400kbps, using zero pre-scaler TWPS = 00, FCPU = 8MHz */
	TWBR = 0x02;
	TWSR = 0x00;

	/* My address */
	TWAR = 0b00000010;

	/* Enable TWI at the end, after all configurations have been made */
	TWCR = (1<<TWEN);
}

void TWI_start(void)
{
	/*
	 * Enable the TWI every time, since we are overwriting the whole register TWEN = 1
	 * clear the flag at the start, before waiting for transfer to start TWINT = 1
	 * send start bit TWSTA = 1
	 */
	TWCR = (1<<TWEN) | (1<<TWSTA) | (1<<TWINT);

	/* wait for bit to get sent (Flag is set)*/
	while(BIT_IS_CLEAR(TWCR,TWINT));
}

void TWI_stop(void)
{
	/*
	 * Enable TWI, TWEN = 1
	 * Send Stop bit, TWSTO = 1
	 * Clear the flag, TWINT = 1
	 */

	TWCR = (1<<TWEN) | (1<<TWSTO) | (1<<TWINT);
}

void TWI_write(uint8 data)
{
	/*Put data on TWI Register*/
	TWDR = data;
	/*
	 * Clear the flag before sending data TWINT = 1
	 *  Enable TWI Module TWEN = 1
	 */
	TWCR = (1<<TWEN) | (1<<TWINT);

	while(BIT_IS_CLEAR(TWCR,TWINT));
}

uint8 TWI_readWithACK(void)
{
	/*
	 * Clear the flag bit before receiving TWINT = 1
	 * Enable the TWI Module TWEN = 1
	 * Enable sending an ACK bit after reading TWEA = 1
	 */
	TWCR = (1<<TWEN) | (1<<TWEA) | (1<<TWINT);
	/* Wait for TWINT flag set in TWCR Register (data received successfully) */
	while(BIT_IS_CLEAR(TWCR,TWINT));
	/* read the data and return it from function */
	return TWDR;
}

uint8 TWI_readWithNACK(void)
{
	/*
	 * Clear the flag bit before receiving TWINT = 1
	 * Enable the TWI Module TWEN = 1
	 * Not enable the sending of ACK bit after reading, means it will send a NACK (or 1) auto after reading
	 */
	TWCR = (1<<TWEN) | (1<<TWINT);
	/* wait till data is received */
	while(BIT_IS_CLEAR(TWCR,TWINT));
	/*return data from TWI data register*/
	return TWDR;
}

uint8 TWI_getStatus(void)
{
	uint8 status;
	/* masking to remove the first 3 bits and get the last 5 bits TWS7-TWS3 */
	status = TWSR & 0xF8;
	return status;
}
