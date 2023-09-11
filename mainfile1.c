/*
 * mainfile.c
 *
 *  Created on: Mar 25, 2023
 *      Author: amor2
 */

#include"keypad.h"
#include"lcd.h"
#include"uart.h"

/*
 * Project Description: this project implemnts a smart door lock system
 * It uses the following components: external EEPROM, Motor(door), LED(alert), two AVR ATmega, Keyapad, LCD
 * The extenal eeprom is used for safe reserve of the password
 * The first MCU does the following: -Gets pass from user
 * -Sends pass to second MCU for eeprom storage
 * -opens the door for the user
 * -turns on the led as an alert(can use buzzer)
 * The second MCU does the following:-Recicves pass from first MCU
 * -forwards the pass to the eeprom
 */




uint8 pass[5];
uint8 eeprom_pass[5];

void LCD_begin();
void writePass();
void eepromPass();
void main() {
	/*
	 * needed initializations for the system
	 */
	UART_init();
	LCD_init();
	LCD_begin();
	LCD_goToRowColumn(1, 0);
	DDRB |= (1 << PB3);
	PORTB &= ~(1 << PB3);
	uint8 str;
	uint8 status;
	uint8 rep_pass[5];
	uint8 wrongPass = 0;
	uint8 firstTime = 0;
	while (1) {
		/*
		 * firstTime is created to indicate if it is the first time the system is running
		 * or if the user requested a new change in the password
		 */
		if (firstTime == 0) {
			writePass();
			eepromPass();
			firstTime++;
		} else {
			/*
			 * after getting the main pass we tell the user to give the status either
			 * open the door, or reenter pass
			 * The number of wrong passwords is remaining the same even if the user
			 * changed the status to increase the security of the system
			 */
			LCD_clearScreen();
			LCD_goToRowColumn(0, 0);
			str = "door press 1";
			LCD_displayString(str);
			LCD_goToRowColumn(1, 0);
			str = "reenter pass 2";
			LCD_displayString(str);
			status = KeyPad_getPressedKey();
			uint8 flag = 0;

			/*
			 * if the user chose to open the door: we tell him to reconfirm the password
			 * if correct a motor implementing the door moves to open and close
			 * if wrong the user has two more iterations before system lockdown
			 */
			if (status == 1) {
				LCD_clearScreen();
				LCD_goToRowColumn(0, 0);
				str = "pass again";
				LCD_displayString(str);
				LCD_goToRowColumn(1, 0);
				for (uint8 i = 0; i < 5; i++) {
					rep_pass[i] = KeyPad_getPressedKey();
					LCD_intgerToString((int) rep_pass[i]);
					_delay_ms(500);
				}
				for (uint8 i = 0; i < 5; i++) {
					if (rep_pass[i] != eeprom_pass[i])
						flag = 1;
				}
				if (flag == 0) {
					SET_BIT(DDRB, PB0);
					SET_BIT(DDRB, PB1);
					SET_BIT(DDRB, PB2);
					CLEAR_BIT(PORTB, PB0);
					SET_BIT(PORTB, PB1);
					SET_BIT(PORTB, PB2);
					_delay_ms(500);
					CLEAR_BIT(PORTB, PB1);
					SET_BIT(PORTB, PB0);
					SET_BIT(PORTB, PB2);
					_delay_ms(500);
					CLEAR_BIT(PORTB, PB2);
				} else {
					LCD_clearScreen();
					LCD_goToRowColumn(0, 0);
					str = "wrong pass";
					LCD_displayString(str);
					_delay_ms(100);
					wrongPass++;
					if (wrongPass == 2) {
						LCD_clearScreen();
						LCD_goToRowColumn(0, 0);
						str = "thief!!";
						LCD_displayString(str);
						PORTB |= (1 << PB3);
						while (1)
							; //to halt the system until a reset
					}

				}
			}
			/*
			 * if the user chose to change the password we tell him to
			 * enter the original password first
			 * if correct: it syncs with the second MCU to change the pass existing in the eeprom
			 * if wrong same scenario happens with the door (only two more iterations)
			 */
			else if (status == 2) {
				LCD_clearScreen();
				LCD_goToRowColumn(0, 0);
				str = "original pass:";
				LCD_displayString(str);
				LCD_goToRowColumn(1, 0);
				for (uint8 i = 0; i < 5; i++) {
					rep_pass[i] = KeyPad_getPressedKey();
					LCD_intgerToString((int) rep_pass[i]);
					_delay_ms(500);
				}
				for (uint8 i = 0; i < 5; i++) {
					if (rep_pass[i] != eeprom_pass[i])
						flag = 1;
				}
				if (flag == 0) {
					LCD_clearScreen();
					LCD_goToRowColumn(0, 0);
					str = "new pass: ";
					LCD_displayString(str);
					LCD_goToRowColumn(1, 0);
					firstTime = 0;
				} else {
					LCD_clearScreen();
					LCD_goToRowColumn(0, 0);
					str = "wrong pass";
					LCD_displayString(str);
					_delay_ms(100);
					wrongPass++;
					if (wrongPass == 2) {
						LCD_clearScreen();
						LCD_goToRowColumn(0, 0);
						str = "thief!!";
						LCD_displayString(str);
						PORTB |= (1 << PB3);
						while (1)
							; //to halt the system until a reset
					}

				}

			}
		}
	}
}

void LCD_begin() {
	/*
	 * this the function to welcome the user and tell him to enter the 5 letter pass
	 */
	uint8 str = "Hi,enter 5Lpass:";
	LCD_displayString(str);
}
void writePass() {
	/*
	 * this function changes the original pass written in the eeprom
	 */
	for (uint8 i = 0; i < 5; i++) {
		pass[i] = KeyPad_getPressedKey();
		LCD_intgerToString((int) pass[i]);
		_delay_ms(500);
		UART_sendByte(pass[i]);

	}
}
void eepromPass() {
	/*
	 * this function reads the pass from the eeprom
	 */
	for (uint8 i = 0; i < 5; i++) {
		eeprom_pass[i] = UART_recieveByte();
	}
}
