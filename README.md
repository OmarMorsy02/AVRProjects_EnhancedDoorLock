# AVRProjects_EnhancedDoorLock
Atmega_32_EnhancedDoorLock Project

## Project description: 
This project is made using the ATmega32 MCU using Embedded C language. It implements a password controlled door lock using EEPROM.

The componponents used were: 1-LCD as a GUI for the user telling him the steps, 2-KEYPAD for user interactions and password entry, 3-An LED and BUZZER for theft warning, 4- Two Atmega32 MCUs one for user interaction and the other to communicate with the EEPROM, and most imporatantly 4-EEPROM for password saving.

The user is first asked to choose whether it is a new or old password and to change it or only open the door. If the user enters the passowrd correctly the door will open, if not the buzzer works to warn about this theft operation. The user is allowed to enter the password incorrectly for two times the third time system lockdown happens.

After correct password entry, using UART the two MCUs communicate and send the data to the EEPROM to store it.

Attached is the .c files that contain the Embedded C codes, and the .elf files to burn on the MCUs. 

The Proteus simulation for this project is also attached to show the pins connected.
## How to compile:
For MCU1 everyfile that ends in ("1") is for MCU1, and with ("2") is for MCU2. Other files are shareable ans should be in both Projects
