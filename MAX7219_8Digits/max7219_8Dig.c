/*
 * 8DIG_Test.c
 *
 *  Created on: Feb 10, 2021
 *      Author: Shady Thomas
 *
 *      Testing MAX7219 with SPI
 */
/*
 * max7219.c
 *
 *  Created on: Feb 4, 2021
 *      Author: Shady Thomas
 */


/* MAX7219 Interaction Code
 * ---------------------------
 * For more information see
 * http://www.adnbr.co.uk/articles/max7219-and-7-segment-displays
 *
 * 668 bytes - ATmega168 - 16MHz
 */

// 16MHz clock
//#define F_CPU 16000000UL

// Outputs, pin definitions
/*original version
#define PIN_SCK                   PORTB5
#define PIN_MOSI                  PORTB3
#define PIN_SS                    PORTB2
*/

/*Shady's version*/
#define PIN_SCK			PORTB7
#define PIN_MOSI		PORTB5
#define PIN_SS	 		PORTB4

#define ON                        1
#define OFF                       0

#define MAX7219_LOAD1             PORTB |= (1<<PIN_SS)
#define MAX7219_LOAD0             PORTB &= ~(1<<PIN_SS)

#define MAX7219_MODE_DECODE       0x09
#define MAX7219_MODE_INTENSITY    0x0A
#define MAX7219_MODE_SCAN_LIMIT   0x0B
#define MAX7219_MODE_POWER        0x0C
#define MAX7219_MODE_TEST         0x0F
#define MAX7219_MODE_NOOP         0x00

// I only have 3 digits, no point having the
// rest. You could use more though.
#define MAX7219_DIGIT0            0x01
#define MAX7219_DIGIT1            0x02
#define MAX7219_DIGIT2            0x03
#define MAX7219_DIGIT3			  0X04
#define MAX7219_DIGIT7			  0X08

#define MAX7219_CHAR_BLANK        0xF
#define MAX7219_CHAR_NEGATIVE     0xA

#include <avr/io.h>
#include <util/delay.h>

char digitsInUse = 8;

void spiSendByte (char databyte)
{
    // Copy data into the SPI data register
    SPDR = databyte;
    // Wait until transfer is complete
    while (!(SPSR & (1 << SPIF)));
}

void MAX7219_writeData(char data_register, char data)
{
    MAX7219_LOAD0;
        // Send the register where the data will be stored
        spiSendByte(data_register);
        // Send the data to be stored
        spiSendByte(data);
    MAX7219_LOAD1;
}

void MAX7219_clearDisplay()
{
    char i = digitsInUse;
    // Loop until 0, but don't run for zero
    do {
        // Set each display in use to blank
        MAX7219_writeData(i, MAX7219_CHAR_BLANK);
    } while (--i);
}

void MAX7219_displayNumber(volatile long number)
{
    char negative = 0;

    // Convert negative to positive.
    // Keep a record that it was negative so we can
    // sign it again on the display.
    if (number < 0) {
        negative = 1;
        number *= -1;
    }

    //MAX7219_clearDisplay();
    /*No need to use clear Display, it reduces intensity
     * And in my application it's not needed, since we have 4 digits for the time and 4 Digits for Temp.
     */

    // If number = 0, only show one zero then exit
    if (number == 0) {
        MAX7219_writeData(MAX7219_DIGIT0, 0);
        return;
    }

    // Initialization to 0 required in this case,
    // does not work without it. Not sure why.
    char i = 0;

    // Loop until number is 0.
    do {
        MAX7219_writeData(++i, number % 10);
        // Actually divide by 10 now.
        number /= 10;
    } while (number);

    // Bear in mind that if you only have three digits, and
    // try to display something like "-256" all that will display
    // will be "256" because it needs an extra fourth digit to
    // display the sign.
    if (negative) {
        MAX7219_writeData(i, MAX7219_CHAR_NEGATIVE);
    }
}

int main(void)
{
    // SCK MOSI CS/LOAD/SS
    DDRB |= (1 << PIN_SCK) | (1 << PIN_MOSI) | (1 << PIN_SS);

    // SPI Enable, Master mode
    SPCR |= (1 << SPE) | (1 << MSTR);// | (1<<SPR1);

    // SPI2x

    SPSR |= (1<<SPI2X);

    // Decode mode to "Font Code-B"
    MAX7219_writeData(MAX7219_MODE_DECODE, 0xFF);

    // Scan limit runs from 0.
    MAX7219_writeData(MAX7219_MODE_SCAN_LIMIT, digitsInUse - 1);
    MAX7219_writeData(MAX7219_MODE_INTENSITY, 12);
    MAX7219_writeData(MAX7219_MODE_POWER, ON);

    unsigned long i = 99999999;
    while(1)
    {
        MAX7219_displayNumber(--i);
        _delay_ms(2);

        if (i == 0) {
            i = 99999999;
        }
    }
}


