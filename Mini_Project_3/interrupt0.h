/*
 * interrupt0.h
 *
 *  Created on: Feb 28, 2021
 *      Author: Shady Thomas
 */

#ifndef INTERRUPT0_H_
#define INTERRUPT0_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

typedef enum
{
	LOW_LEVEL, BOTH, FALLING, RISING
}INT0_Sense;

void INT0_init();
void INT0_setCallback(void (*a_ptr)(void));
void INT0_senseControl(const INT0_Sense sense);
void INT0_deinit();

#endif /* INTERRUPT0_H_ */
