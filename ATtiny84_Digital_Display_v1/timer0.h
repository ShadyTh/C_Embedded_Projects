/*
 * timer1.h
 *
 *  Created on: Mar 1, 2021
 *      Author: Shady Thomas
 */

#ifndef TIMER0_H_
#define TIMER0_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

void TIMER0_CTC_init(void);
void TIMER0_setCallback(void (*ptr) (void));

#endif /* TIMER0_H_ */
