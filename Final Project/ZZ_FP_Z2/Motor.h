/*
 * Motor.h
 *
 *  Created on: Apr 21, 2021
 *      Author: Shady Thomas
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include "common_macros.h"
#include "micro_config.h"
#include "std_types.h"

#define MOTOR_PORT_DIR		DDRD
#define MOTOR_PORT			PORTD
#define MOTOR_PIN1			PD3
#define MOTOR_PIN2			PD4


void MOTOR_init(void);
void MOTOR_rotateCW(void);
void MOTOR_rotateCCW(void);
void MOTOR_stop(void);

#endif /* MOTOR_H_ */
