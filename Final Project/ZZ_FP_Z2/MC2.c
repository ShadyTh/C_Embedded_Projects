/*
 * MC2.c
 *
 *	Micro-controller working in background,
 *		- saves and fetches password to external EEPROM using I2C
 *		- takes all the decisions
 *		- communicates with MC1 and receives user entry through UART
 *		- controls Motor of door lock and alarm
 *
 *	Tasks TO DO:
 *		(x) Alert Function
 *		(x) Correct 1st time pass checking
 *		(x) Implement Timer Driver in Door Unlocking delay
 *		(x)  Implement Timer in Alarm
 *		(x) Modify Unlock Door Screen Message
 *		(x) Remove Test LEDs and LCD test character
 *		(x) put all hash defines in .h
 *		(x) Motor .c and .h files
 *		(x)  Buzzer
 *		(x) Fix 'Enter' Bug
 *
 *  Created on: Mar 21, 2021
 *      Author: Shady Thomas
 */

#include "MC2.h"

int main()
{
	uint8 menu_byte;
	uint8 pass_check;
	uint8 error_count = 0;

	UART_CONFIG_TYPE uart_config = {BIT_RATE_9600, SIZE_8BIT, DISABLE_PARTITY, STOP_1BIT};
	UART_initConfig(&uart_config);

	MOTOR_init();	// Initialize Motor Pins

	DDRD |= (1<<PD7);		// Buzzer Pin
	PORTD &= ~(1<<PD7);

	EEPROM_init();

	/* comment this line if u do not want to reset password
	 * each time the micro-controller is reset
	 * In actual functionality, this line should be commented	*/
	EEPROM_writeByte(PASS_EXIST_ADR, PASS_NOT_EXIST);
	_delay_ms(10);

	/*initial handshake, i.e; both MC1 and MC2 are ready*/
	while(UART_receiveByte() != MC1_READY);
	UART_sendByte(MC2_READY);

	while(1)
	{
		/*check if there is a saved password or not
		by checking on the pass_exist address*/
		EEPROM_readByte(PASS_EXIST_ADR, &pass_check);
		_delay_ms(10);

		if(pass_check == PASS_NOT_EXIST)
		{
			while(1)
			{
				if (LOCK_saveNewPass() == 1)
				{
					UART_sendByte(PASS_SAVED_SUCCESS);
					_delay_ms(100);
					DDRA  |= (1<<PA0);
					PORTA |= (1<<PA0);
					_delay_ms(2400);	//screen delay
					break;
				}
				else	// if returned Error, repeat the while loop again
				{
					UART_sendByte(NOT_MATCHING);	// display on the screen not matching
				}
			}
		}
		else if(pass_check == PASS_DOES_EXIST)
		{
			while(1)
			{
				UART_sendByte(MAIN_MENU);		// display menu screen
				menu_byte = UART_receiveByte();		// wait for user entry '+' or '-'

				if(menu_byte == CHANGE_PASS_MODE)	//Change Pass Menu
				{
					while(1)
					{
						if(LOCK_checkPass('+') == 1)
						{
							while(1)
							{
								if(LOCK_saveNewPass() == 1)
								{
									UART_sendByte(PASS_SAVED_SUCCESS);
									_delay_ms(2400);	// screen delay
									break;
								}
								else
								{
									UART_sendByte(NOT_MATCHING);
								}
							}
							error_count = 0;
							break;
						}
						else
						{
							UART_sendByte(NOT_MATCHING);
							error_count++;
							/* Starting from 3 trials left
							 * Send warning to HMI	*/
							if((error_count > (MAX_TRIALS - 3)) && (error_count < MAX_TRIALS))
							{
								UART_sendByte(ERROR_TRIALS_LEFT);
								UART_sendByte(MAX_TRIALS - error_count);
								_delay_ms(3000);
							}

							/* After exceeding maximum number of errors
							 * Enter into alert mode
							 * Then go back to main menu	*/
							else if(error_count == MAX_TRIALS)
							{
								UART_sendByte(ALERT);
								LOCK_alert();
								error_count = 0;
								break;
							}
						}
					}
				}

				else if(menu_byte == UNLOCK_DOOR_MODE) //Unlock Door
				{
					while(1)
					{
						if(LOCK_checkPass('-') == 1)
						{
							UART_sendByte(DOOR_UNLOCKING);
							LOCK_doorUnlock();
							break;
						}
						else
						{
							UART_sendByte(NOT_MATCHING);
							error_count++;
							/* Starting from 3 trials left
							 * Send warning to HMI	*/
							if((error_count > (MAX_TRIALS - 3)) && (error_count < MAX_TRIALS))
							{
								UART_sendByte(ERROR_TRIALS_LEFT);
								UART_sendByte(MAX_TRIALS - error_count);
								_delay_ms(3000);
							}

							/* After exceeding maximum number of errors
							 * Enter into alert mode
							 * Then go back to main menu	*/
							else if(error_count == MAX_TRIALS)
							{
								UART_sendByte(ALERT);
								LOCK_alert();
								error_count = 0;
								break;
							}
						}
					}
				}
				else	//Incorrect Entry
				{

				}
			}
		}
	}
}

/********************************
 * 		Function Declarations
 ********************************/

uint8 LOCK_saveNewPass(void)
{
	uint8 pass1[16], pass2[16], i;
	uint8 * pass1_ptr = pass1;
	uint8 * pass2_ptr = pass2;
	uint8 rec_byte1, rec_byte2, pass1_length = 0, pass2_length = 0;

	UART_sendByte(NEW_PASS);

	while(1)	//loop to save password for 1st time
	{
		rec_byte1 = UART_receiveByte();
		if(rec_byte1 != ENTER)
		{
			*pass1_ptr = rec_byte1;		//save character
			pass1_ptr ++;				//increment pointer position (index)
			pass1_length ++;			//increment password length
		}
		else
		{
			break;			// when enter is pressed, confirm password
		}
	}

	while(1)	// loop to confirm password
	{
		rec_byte2 = UART_receiveByte();
		if(rec_byte2 != ENTER)
		{
			*pass2_ptr = rec_byte2;
			pass2_ptr ++;
			pass2_length++;
		}
		else	//when enter is pressed, compare the 2 passwords
		{
			if(pass1_length != pass2_length)	// compare their lengths
			{
				return ERROR;
			}
			else		// then compare each character
			{
				for(i = 0; i<pass1_length; i++)
				{
					if(pass1[i] != pass2[i])
						return ERROR;
				}

				// if no return error,
				// continue to write each character to eeprom
				for(i = 0; i<pass1_length; i++)
				{
					EEPROM_writeByte((PASS_ADR + i),pass1[i]);
					_delay_ms(10);
				}

				// write to eeprom that password exists
				EEPROM_writeByte(PASS_EXIST_ADR, PASS_DOES_EXIST);
				_delay_ms(10);
				return SUCCESS;
			}
		}
	}
}

uint8 LOCK_checkPass(uint8 screen)
{
	uint8 pass1[16], pass2[16], i;
	uint8 * pass1_ptr = pass1;
	uint8 rec_byte1 , pass1_length = 0;

	UART_sendByte(CHECK_PASS);
	UART_sendByte(screen);
	while(1)
	{
		rec_byte1 = UART_receiveByte();
		if(rec_byte1 != ENTER)
		{
			*pass1_ptr = rec_byte1;
			pass1_ptr ++;
			pass1_length ++;
		}
		else
		{
			break;
		}
	}
	for(i = 0; i < pass1_length; i++)
	{
		EEPROM_readByte((PASS_ADR + i), &pass2[i]);
		_delay_ms(10);
	}

	for(i = 0; i < pass1_length; i++)
	{
		if(pass1[i] != pass2[i])
			return ERROR;
	}
	return SUCCESS;
}

void LOCK_doorUnlock(void)
{
	MOTOR_rotateCW();	// turn motor in on direction
	TIMER0_delay(2000);
	MOTOR_stop();		// hold motor
	TIMER0_delay(500);
	MOTOR_rotateCCW();	// turn motor in other direction
	TIMER0_delay(2000);
	MOTOR_stop();		// turn off motor
}

void LOCK_alert(void)
{
		TIMER0_PWM_init();		// Buzzer On
		TIMER1_delay(3000);
		TIMER0_deinit();		// Buzzer Off
		CLEAR_BIT(PORTB, PB3);
}
