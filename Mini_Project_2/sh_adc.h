/*
 * sh_adc.h
 *
 *  Created on: Feb 17, 2021
 *      Author: Shady Thomas
 */

#ifndef SH_ADC_H_
#define SH_ADC_H_

#include "micro_config.h"
#include "common_macros.h"
#include "std_types.h"

/*		Function Prototypes		*/

void ADC_init();
uint16 ADC_readChannel(uint8 channel);


#endif /* SH_ADC_H_ */
