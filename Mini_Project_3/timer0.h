/*
 * timer0.h
 *
 *  Created on: Feb 28, 2021
 *      Author: Shady Thomas
 */

#ifndef TIMER0_H_
#define TIMER0_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

void TIMER0_OVF_init(void);
void TIMER0_clearTimerValue(void);
void TIMER0_stop(void);

#endif /* TIMER0_H_ */
