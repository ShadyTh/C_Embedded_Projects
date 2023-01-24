/*
 * timer1.h
 *
 *  Created on: Mar 1, 2021
 *      Author: Shady Thomas
 */

#ifndef TIMER1_H_
#define TIMER1_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

void TIMER1_CTC_init(void);
void TIMER1_setCallback(void (*ptr) (void));

#endif /* TIMER1_H_ */
