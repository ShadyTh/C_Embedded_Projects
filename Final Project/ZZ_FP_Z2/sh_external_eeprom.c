/*
 * external_eeprom.c
 *
 *  Created on: Feb 27, 2021
 *      Author: Shady Thomas
 */

#include "sh_i2c.h"
#include "sh_external_eeprom.h"

void EEPROM_init(void)
{
	TWI_init();
}
uint8 EEPROM_writePass(uint16 u16addr, uint8 * pass)
{
	uint8 i;
	/* Send the start bit */
	TWI_start();
	if(TWI_getStatus() != TW_START)
		return ERROR;

    /* Send the device address, we need to get A8 A9 A10 address bits from the
     * memory location address and R/W=0 (write) */
	TWI_write((uint8) (0xA0 | ((u16addr & 0x700)>>7)));
    if (TWI_getStatus() != TW_MT_SLA_W_ACK)
        return ERROR;

    /* Send the required memory location address */
    TWI_write((uint8)(u16addr));
    if (TWI_getStatus() != TW_MT_DATA_ACK)
		return ERROR;

    for(i=0; i<5; i++)
    {
		/* write byte to eeprom */
		TWI_write(*pass);
		pass++;
		if (TWI_getStatus() != TW_MT_DATA_ACK)
			return ERROR;
		_delay_ms(10);
    }

	/* Send the Stop Bit */
	TWI_stop();

	return SUCCESS;
}

uint8 EEPROM_readPass(uint16 u16addr, uint8 *pass)
{
	uint8 i;

	/* Send the Start Bit */
    TWI_start();
    if (TWI_getStatus() != TW_START)
        return ERROR;

    /* Send the device address, we need to get A8 A9 A10 address bits from the
     * memory location address and R/W=0 (write) */
    TWI_write((uint8)((0xA0) | ((u16addr & 0x0700)>>7)));
    if (TWI_getStatus() != TW_MT_SLA_W_ACK)
        return ERROR;

    /* Send the required memory location address */
    TWI_write((uint8)(u16addr));
    if (TWI_getStatus() != TW_MT_DATA_ACK)
        return ERROR;

    /* Send the Repeated Start Bit */
    TWI_start();
    if (TWI_getStatus() != TW_REP_START)
        return ERROR;

    /* Send the device address, we need to get A8 A9 A10 address bits from the
     * memory location address and R/W=1 (Read) */
    TWI_write((uint8)((0xA0) | ((u16addr & 0x0700)>>7) | 1));
    if (TWI_getStatus() != TW_MT_SLA_R_ACK)
        return ERROR;

    for(i=0; i<4; i++)
    {
		/* Read Byte from Memory with sending ACK */
		*pass = TWI_readWithACK();
		pass++;
		if (TWI_getStatus() != TW_MR_DATA_ACK)
			return ERROR;
		_delay_ms(10);
    }

    /* Read Byte from Memory without send ACK */
	*pass = TWI_readWithNACK();
	if (TWI_getStatus() != TW_MR_DATA_NACK)
		return ERROR;

    /* Send the Stop Bit */
    TWI_stop();
    return SUCCESS;
}

uint8 EEPROM_writeByte(uint16 u16addr, uint8 u8data)
{
	/* Send the start bit */
	TWI_start();
	if(TWI_getStatus() != TW_START)
		return ERROR;

    /* Send the device address, we need to get A8 A9 A10 address bits from the
     * memory location address and R/W=0 (write) */
	TWI_write((uint8) (0xA0 | ((u16addr & 0x700)>>7)));
    if (TWI_getStatus() != TW_MT_SLA_W_ACK)
        return ERROR;

    /* Send the required memory location address */
    TWI_write((uint8)(u16addr));
    if (TWI_getStatus() != TW_MT_DATA_ACK)
		return ERROR;

	/* write byte to eeprom */
	TWI_write(u8data);
	if (TWI_getStatus() != TW_MT_DATA_ACK)
		return ERROR;

	/* Send the Stop Bit */
	TWI_stop();

	return SUCCESS;
}

uint8 EEPROM_readByte(uint16 u16addr, uint8 *u8data)
{
	/* Send the Start Bit */
    TWI_start();
    if (TWI_getStatus() != TW_START)
        return ERROR;

    /* Send the device address, we need to get A8 A9 A10 address bits from the
     * memory location address and R/W=0 (write) */
    TWI_write((uint8)((0xA0) | ((u16addr & 0x0700)>>7)));
    if (TWI_getStatus() != TW_MT_SLA_W_ACK)
        return ERROR;

    /* Send the required memory location address */
    TWI_write((uint8)(u16addr));
    if (TWI_getStatus() != TW_MT_DATA_ACK)
        return ERROR;

    /* Send the Repeated Start Bit */
    TWI_start();
    if (TWI_getStatus() != TW_REP_START)
        return ERROR;

    /* Send the device address, we need to get A8 A9 A10 address bits from the
     * memory location address and R/W=1 (Read) */
    TWI_write((uint8)((0xA0) | ((u16addr & 0x0700)>>7) | 1));
    if (TWI_getStatus() != TW_MT_SLA_R_ACK)
        return ERROR;

    /* Read Byte from Memory without send ACK */
    *u8data = TWI_readWithNACK();
    if (TWI_getStatus() != TW_MR_DATA_NACK)
        return ERROR;

    /* Send the Stop Bit */
    TWI_stop();
    return SUCCESS;
}
