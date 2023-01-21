/*
 * HMI.h
 *
 *  Created on: Apr 21, 2021
 *      Author: Shady Thomas
 */

#ifndef HMI_H_
#define HMI_H_

#include "sh_keypad.h"
#include "sh_lcd.h"
#include "sh_uart.h"
#include "uart_communication.h"

void newPassScreen(void);
void notMatchingPass(void);
void passSavedSuccess(void);
void mainMenu(void);
void checkPassScreen(uint8 screen);
void doorUnlocking(void);
void errorTrialsLeft(void);
void intruderAlert(void);
void enterPass(void);

#endif /* HMI_H_ */
