/*
 * adc.h
 *
 *  Created on: Mar 1, 2021
 *      Author: Shady Thomas
 */

#ifndef ADC_H_
#define ADC_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

void ADC_init();
uint16 ADC_readVal(void);

void ADC_initINT();
void ADC_setCallback(void (*ptr) (void));
void ADC_readValInterrupt(void);

#endif /* ADC_H_ */
