/*
 * timer0_pwm.h
 *
 *  Created on: Feb 18, 2021
 *      Author: Shady Thomas
 */

#ifndef TIMER0_PWM_H_
#define TIMER0_PWM_H_

#include "micro_config.h"
#include "common_macros.h"
#include "std_types.h"

/*	Function Prototypes	*/

void TIMER0_PWM_init();
void TIMER0_PWM_setDutyCycle(uint16 duty);

#endif /* TIMER0_PWM_H_ */
