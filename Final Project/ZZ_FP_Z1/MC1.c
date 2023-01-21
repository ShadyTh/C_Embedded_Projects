/*
 * MC1.c
 *
 *	Micro-controller acting as Human Machine Interface
 *	and sends whatever input the user is sending
 *	to the other Micro-controller
 *	via UART communication protocol
 *
 *  Created on: Mar 21, 2021
 *      Author: Shady Thomas
 */

#include "HMI.h"

int main()
{
	uint8 mode;

	LCD_init();

	UART_CONFIG_TYPE uart_config = {BIT_RATE_9600, SIZE_8BIT, DISABLE_PARTITY, STOP_1BIT};
	UART_initConfig(&uart_config);

	/*initial handshake, i.e; both MC1 and MC2 are ready*/
	UART_sendByte(MC1_READY);
	while(UART_receiveByte() != MC2_READY);

	while(1)
	{
		mode = UART_receiveByte();
		switch (mode)
		{
			case NEW_PASS:
					newPassScreen();
					break;

			case NOT_MATCHING:
					notMatchingPass();
					break;

			case PASS_SAVED_SUCCESS:
					passSavedSuccess();
					break;

			case MAIN_MENU:
					mainMenu();
					break;

			case CHECK_PASS:
					checkPassScreen(UART_receiveByte());
					break;

			case DOOR_UNLOCKING:
					doorUnlocking();
					break;

			case ERROR_TRIALS_LEFT:
					errorTrialsLeft();
					break;

			case ALERT:
					intruderAlert();
					break;
		}
	}
}

void newPassScreen(void)
{
	LCD_clearScreen();
	LCD_displayString("Enter Password:");
	LCD_goToRowColumn(1,0);

	enterPass();

	LCD_clearScreen();
	LCD_displayString("Confirm Password");
	LCD_goToRowColumn(1,0);

	enterPass();
}

void notMatchingPass(void)
{
	LCD_clearScreen();
	LCD_displayString("Not Matching");
	_delay_ms(400);
	LCD_goToRowColumn(1,0);
	LCD_displayString("Please Try Again");
	_delay_ms(2000);
}

void passSavedSuccess(void)
{
	LCD_clearScreen();
	LCD_displayString("Password Saved");
	_delay_ms(400);
	LCD_goToRowColumn(1,0);
	LCD_displayString("Successfully");
	_delay_ms(2000);
}

void mainMenu(void)
{
	uint8 key;
	LCD_clearScreen();
	LCD_displayString("+: Change Pass");
	LCD_displayStringRowColumn(1,0,"-: Open Door");
	while(1)
	{
		key = KEYPAD_getPressedKey();
		if(key == '+')
		{
			UART_sendByte(CHANGE_PASS_MODE);
			break;
		}
		else if(key == '-')
		{
			UART_sendByte(UNLOCK_DOOR_MODE);
			break;
		}
		_delay_ms(400);	//delay for keypad
	}
}

void checkPassScreen(uint8 screen)
{
	LCD_clearScreen();
	if(screen == '+')
	{
		LCD_displayString("Enter Old Pass");
		LCD_displayStringRowColumn(1,0,"To Change");
		_delay_ms(1500);
		LCD_clearScreen();
		LCD_displayString("Old Pass:");
		LCD_goToRowColumn(1,0);
	}
	else if(screen == '-')
	{
		LCD_displayString("Enter Password");
		LCD_displayStringRowColumn(1,0,"To Unlock Door");
		_delay_ms(1500);
		LCD_clearScreen();
		LCD_displayString("Password:");
		LCD_goToRowColumn(1,0);
	}

	enterPass();
}

void doorUnlocking(void)
{
	uint8 i = 3;
	while(i>0)
	{
		LCD_clearScreen();
		_delay_ms(200);
		LCD_displayString("Door");
		LCD_displayStringRowColumn(1,0,"Unlocking");
		_delay_ms(800);

		i--;
	}
}

void errorTrialsLeft(void)
{
	uint8 trials;
	trials = UART_receiveByte();
	LCD_clearScreen();
	LCD_displayString("Warning!");
	_delay_ms(1000);
	LCD_clearScreen();
	LCD_displayString("You have ");
	LCD_goToRowColumn(1,0);
	LCD_displayInteger(trials);
	LCD_displayString(" trials left");
	_delay_ms(2000);
}

void intruderAlert(void)
{
	uint8 i = 3;
	LCD_clearScreen();
	while(i>0)
	{
		LCD_displayString("Intruder Alert!");
		_delay_ms(1000);
		LCD_clearScreen();
		_delay_ms(500);

		i--;
	}
}

void enterPass(void)
{
	uint8 key;
	/* First Ensure that the user enters at least 1 character */
	while(1)
	{
		key = KEYPAD_getPressedKey();
		if((key <= 9) && (key >= 0))
		{
			LCD_displayCharacter('*');
			UART_sendByte(key);
			_delay_ms(400);
			break;
		}
	}
	while(1)
	{
		key = KEYPAD_getPressedKey();
		/* when enter is pressed, send Pass to MC2 */
		if(key == 13)
		{
			UART_sendByte(ENTER);
			/* this delay is a must */
			_delay_ms(400);
			break;
		}
		else if((key <= 9) && (key >= 0))
		{
			LCD_displayCharacter('*');
			UART_sendByte(key);
			key = 0;
		}
		_delay_ms(400);
	}
}
