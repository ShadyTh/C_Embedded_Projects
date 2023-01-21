/*
 * MC2.h
 *
 *  Created on: Apr 21, 2021
 *      Author: Shady Thomas
 */

#ifndef MC2_H_
#define MC2_H_

#include "sh_uart.h"
#include "sh_external_eeprom.h"
#include "uart_communication.h"
#include "Motor.h"
#include "sh_timer.h"

/*address to check if first time to enter password or note
 	 if 0 then first time
 	 if 1 then there is a password saved*/
#define PASS_EXIST_ADR 	0x0133
#define PASS_DOES_EXIST	0x41	//ASCII for 'A'
#define PASS_NOT_EXIST 	0x55	//ASCII for 'U'
#define PASS_ADR		0x00BB

#define MAX_TRIALS		3 /* Number of maximum error when inputting password */

/********************************
 * 		Function Prototypes
 ********************************/
uint8 LOCK_saveNewPass(void);
uint8 LOCK_checkPass(uint8 screen);
void LOCK_doorUnlock(void);
void LOCK_alert(void);


#endif /* MC2_H_ */
