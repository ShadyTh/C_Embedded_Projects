/*
 * sh_timer.h
 *
 * A simpler timer driver used for delays
 *
 *  Created on: Apr 21, 2021
 *      Author: Shady Thomas
 */

#ifndef SH_TIMER_H_
#define SH_TIMER_H_

#include "common_macros.h"
#include "micro_config.h"
#include "std_types.h"

/**************************************
			Extern Variable
**************************************/

extern void (*const TIMER_delay[3])(uint16);

/**************************************
			Function Prototypes
**************************************/

void TIMER0_delay(uint16 millis);
void TIMER1_delay(uint16 millis);
void TIMER2_delay(uint16 ten_millis);

void TIMER0_PWM_init(void);
void TIMER0_deinit(void);

#endif /* SH_TIMER_H_ */
