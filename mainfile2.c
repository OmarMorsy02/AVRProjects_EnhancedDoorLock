/*
 * mainfile.c
 *
 *  Created on: Mar 25, 2023
 *      Author: amor2
 */

#include"i2c.h"
#include"external_eeprom.h"
#include"uart.h"

uint16 address = 0x0311;
uint8 pass[5] = { 0, 0, 0, 0, 0 };
void passRecieve();
void passResend();
void main() {

	EEPROM_init();
	TWI_init();
	UART_init();

	uint8 rep_pass[5] = { 0, 0, 0, 0, 0 };
	uint8 flag = 0;
	while (1) {
		passRecieve();
		passResend();
	}
}

void passRecieve() {

	/*
	 * this is the function that recieves the pass and writes it in the eeprom
	 */
	for (uint8 i = 0; i < 5; i++) {
		pass[i] = UART_recieveByte();
		_delay_ms(10);
		EEPROM_writeByte(address + i, pass[i]);
		_delay_ms(10);
	}

}

void passResend() {
	/*
	 * this function sends the pass from the eeprom to the second mcu for processing
	 */
	uint8 send[5];
	for (uint8 i = 0; i < 5; i++) {
		EEPROM_readByte(address + i, &send[i]);
		UART_sendByte(send[i]);
		_delay_ms(10);
	}
}
