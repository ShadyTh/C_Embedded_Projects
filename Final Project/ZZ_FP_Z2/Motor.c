/*
 * Motor.c
 *
 *  Created on: Apr 21, 2021
 *      Author: Shady Thomas
 */

#include "Motor.h"

void MOTOR_init(void)
{
	SET_BIT(MOTOR_PORT_DIR,MOTOR_PIN1);
	SET_BIT(MOTOR_PORT_DIR,MOTOR_PIN2);

	CLEAR_BIT(MOTOR_PORT,MOTOR_PIN1);
	CLEAR_BIT(MOTOR_PORT,MOTOR_PIN2);
}

void MOTOR_rotateCW(void)
{
	SET_BIT(MOTOR_PORT,MOTOR_PIN1);
	CLEAR_BIT(MOTOR_PORT,MOTOR_PIN2);
}

void MOTOR_rotateCCW(void)
{
	CLEAR_BIT(MOTOR_PORT,MOTOR_PIN1);
	SET_BIT(MOTOR_PORT,MOTOR_PIN2);
}

void MOTOR_stop(void)
{
	CLEAR_BIT(MOTOR_PORT,MOTOR_PIN1);
	CLEAR_BIT(MOTOR_PORT,MOTOR_PIN2);
}
