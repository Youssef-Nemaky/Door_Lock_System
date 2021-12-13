/*
 * him.c Human Interface Micro-controller
 *
 *  Created on: Mar 27, 2021
 *      Author: Youssef El-Nemaky
 */
#include "HIM_functions.h"

/*********************************
 * 		 Global Variables
 *********************************/
uint8 g_password1[pw_length];
uint8 g_password2[pw_length];
uint8 g_firstTime = 1;
uint8 g_timerTicks = 0;



int main(){
	/* option  1 -> open door and 2 -> change password */
	/* replyOnThePassword when him_mcu sends the password to the control_mcu the reply would be saved in replyOnThePassword variable
	 * C : correct password
	 * E : error - password is not correct
	 * L : lock the system */
	uint8 option, replyOnThePassword, systemPoweredOffWhileInLock;

	/* uartConfig structure and timerConfig structure which will be used to initialize UART and TIMER*/
	USART_Config uartConfig = {_8_BITS,DISABLED,_1_BITS,9600};
	Timer_Config timerConfig = {timerInitialValue, timerCompareValue,Timer1,Compare,F_CPU_256};

	/* configure I-Bit to be set to enable global interrupts */
	SREG |= (1<<7);

	/* initialize UART by sending the address of uartConfig structure */
	UART_init(&uartConfig);

	/* initialize LCD */
	LCD_init();

	if(isPoweredDownWhileInLock()){
		lockSystem(&timerConfig);
	}
	/* SUPER LOOP */
	while(1){
		g_firstTime = isFirstTime(); /* check weather this is the first time to run the system or not */

		if(g_firstTime == 1){
			/* in case this is the first time to run the system, call systemFirstTimeStart() function which will keep asking the user to
			 * enter two matched passwords */

			systemFirstTimeStart();
		} else {
			/* Main Screen */
			LCD_clearScreen();
			LCD_displayString("1: Open Door!");
			LCD_displayStringRowColumn("2: Change PW!", 1, 0);

			/* keep looping until the pressed key is either 1 or 2 */
			do{
				option = KEYPAD_getPressedKey();
				_delay_ms(300);
			} while(option != 1 && option != 2);

			/* keep sending the password to the control_mcu until it is correct or lock the system if it is wrong more than certain number of times */
			do{
				replyOnThePassword = verifyPassword(option);

				if(replyOnThePassword == 'E'){
					/* display error message */
					LCD_clearScreen();
					LCD_displayString("Wrong Password!");
					_delay_ms(500);
				}
			} while(replyOnThePassword != 'C' && replyOnThePassword != 'L');

			if(replyOnThePassword == 'C'){
				/* the password is correct then open door or change the password */
				switch(option){
					case 1:
						openDoor(&timerConfig);
					break;
					case 2: systemFirstTimeStart();
					break;
				} /* end of switch statement */

			} else {
				/* the password was incorrect more than number of certain times so lock the system */
				lockSystem(&timerConfig);
			} /* end of else*/

		} /* end of outter else */

	} /* end of super loop*/

} /* end of main() function*/

