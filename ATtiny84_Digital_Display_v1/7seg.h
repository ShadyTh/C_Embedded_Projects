/*
 * 7seg.h
 *
 *  Created on: Mar 1, 2021
 *      Author: Shady Thomas
 */

#ifndef SEG_H_
#define SEG_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

void SEG_displayDigit(uint8 num);
void SEG_displayLowDigit(uint8 num);
void SEG_displayUpDigit(uint8 num);
void SEG_displayNum(uint8 num);

void SEG_initV2(void);
void SEG_displayDigitV2(uint8 num);
void SEG_displayLowDigitV2(uint8 num);
void SEG_displayUpDigitV2(uint8 num);
void SEG_displayNumV2(uint8 num);

#endif /* 7SEG_H_ */
