/*
 * sh_external_eeprom.h
 *
 *  Created on: Feb 27, 2021
 *      Author: Shady Thomas
 */

#ifndef SH_EXTERNAL_EEPROM_H_
#define SH_EXTERNAL_EEPROM_H_

#include "std_types.h"

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/
#define ERROR 0
#define SUCCESS 1

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void EEPROM_init(void);
uint8 EEPROM_writeByte(uint16 u16addr,uint8 u8data);
uint8 EEPROM_readByte(uint16 u16addr,uint8 *u8data);

uint8 EEPROM_writePass(uint16 u16addr, uint8 * pass);
uint8 EEPROM_readPass(uint16 u16addr, uint8 *pass);

#endif /* SH_EXTERNAL_EEPROM_H_ */
