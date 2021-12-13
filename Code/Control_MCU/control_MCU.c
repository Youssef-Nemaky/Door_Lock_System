/*
 * control_MCU.c
 *
 *  Created on: Mar 31, 2021
 *      Author: Youssef El-Nemaky
 */


#include "CM_functions.h"

/*********************************
 * 		 Global Variables
 *********************************/
uint8 g_password1[pw_length];
uint8 g_password2[pw_length];
uint8 g_numberOfWrongAttempts;
uint8 g_firstTime = 1;
uint8 g_timerTicks = 0;


int main(){
	uint8 command;
	/***********************************************
	 * 				 COMMAND LIST
	 * ---------------------------------------------
	 * F: first time
	 * S: save password
	 * O: open door (verify password + open door)
	 * V: verify password
	 ***********************************************/
	USART_Config uartConfig = {_8_BITS,DISABLED,_1_BITS,9600};
	Timer_Config timerConfig = {timerInitialValue, timerCompareValue,Timer1,Compare,F_CPU_256};

	/*initialize EEPROM */
	EEPROM_init();

	/* configure I-Bit to be set to enable global interrupts */
	SREG |= (1<<7);

	/* initialize UART by sending the address of uartConfig structure */
	UART_init(&uartConfig);

	/* initialize the buzzer */
	BUZZER_init();

	/* initialize the motor */
	MOTOR_init();

	/* read the first time variable from EEPROM and update the g_firstTime with it */
	EEPROM_readByte(addressOfFirstTimeVariable, &g_firstTime);
	_delay_ms(80);

	/* read the number of wrong attempts variable from EEPROM and update the g_numberOfWrongAttempts */
	EEPROM_readByte(addressOfWrongAttempts, &g_numberOfWrongAttempts);


	/* SUPER LOOP */
	while(1){
		/* receive the command thorugh UART communication protocol */
		command = UART_receiveByte();
		switch(command){
		case 'P':
			/* is powered down while in lock down ? command */
			if(g_numberOfWrongAttempts != 0xFF && g_numberOfWrongAttempts >= maxWrongAttempts){
				/*0xFF is the initial value at first run of the system */
				UART_sendByte(1); /*indicating to the HMI that it was powered down and start locking the system again */

				lockSystem(&timerConfig); /* lock the system */

				/* reset g_numberOfWrongAttempts variable */
				g_numberOfWrongAttempts = 0;

				/* reset the number of wrong attempts stored in EEPROM after the system has returned from lock system state */
				EEPROM_writeByte(addressOfWrongAttempts, g_numberOfWrongAttempts);
			} else {
				UART_sendByte(0); /*indicating to the HMI that it wasn't powered down while in lock and everything is fine */
			}
			break;
		case 'F':
			/* first time command */
			UART_sendByte(isFirstTimeStart());
		break; /* break of switch statement */
		case 'S':
			/* save password command */
			savePassword(); //inside -> getPassword();
		break; /* break of switch statement */
		case 'O':
		case 'V':
			/* verify password command */
			if(verifyPassword()){
				/* password is correct case */

				g_numberOfWrongAttempts = 0; /* reset the g_numberOfWrongAttempts variable */
				EEPROM_writeByte(addressOfWrongAttempts, g_numberOfWrongAttempts); /*update the EEPROM with the new value*/
				UART_sendByte('C'); /* send C (correct) command through UART communication protocol to him_mcu */

				if(command == 'O'){
					/* initiate open door sequence */
					openDoor(&timerConfig);
				} /* end of if statement */

			} else {
				/* wrong password case */

				g_numberOfWrongAttempts++; /* increment the number of wrong attempts variable */

				EEPROM_writeByte(addressOfWrongAttempts, g_numberOfWrongAttempts); /* update the EEPROM with the new value */
				_delay_ms(80);

				if(g_numberOfWrongAttempts >= maxWrongAttempts){

					UART_sendByte('L'); /* send L (lock) command through UART communication protocol to him_mcu */

					lockSystem(&timerConfig); /* lock the system */

					/* reset g_numberOfWrongAttempts variable */
					g_numberOfWrongAttempts = 0;

					EEPROM_writeByte(addressOfWrongAttempts, g_numberOfWrongAttempts); /* reset the number of wrong attempts stored in EEPROM after the system has returned from lock system state */
				} else UART_sendByte('E'); /* send E (error) command */
			} /* end of else statement */
		break; /* break of switch statement */
		} /* end of switch statement */
	} /* end of super loop */
} /* end of main() function */
