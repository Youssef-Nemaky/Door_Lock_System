/*
 * HIM_functions.c
 *
 *  Created on: Mar 29, 2021
 *      Author: Youssef El-Nemaky
 */

#include "HIM_functions.h"

/*******************************************************************************************************************
 * Function : isPoweredDownWhileInLock
 *
 * Description : this function checks if the hacker/thieve shut down the power of the system while it was on lock down
 *
 * Parameters : None (void)
 *
 * Return Value : uint8: 1: indicating that the system was shut down in the lock down phase
 * 						 0: indicating that the system was not shut down in the lock down phase
 *******************************************************************************************************************/
uint8 isPoweredDownWhileInLock(void){
	/*asking the control unit*/
	UART_sendByte('P');
	return UART_receiveByte();
}




/*******************************************************************************************************************
 * Function : isFirstTime
 *
 * Description : used to ask the control_mcu weather this is the first time to run the system or not
 * The control_mcu would replies with a 1 or 0
 *
 * Parameters : None (void)
 *
 * Return Value : uint8 : 1 or 0 to determine weather it is the first time to run the system or not
 *******************************************************************************************************************/
uint8 isFirstTime(void){
	/* send the F (first time) command to control_mcu */
	UART_sendByte('F');

	return UART_receiveByte(); /* return the reply */
} /* end of isFirstTime() function */



/*******************************************************************************************************************
 * Function : systemFirstTimeStart
 *
 * Description : used when the system is run for the first time.
 * it would ask the user to enter the password two times and check weather the two passwords match each other or not
 * if they don't match, it asks the user to enter the two passwords again
 * this process repeats until the user finally enters two matched passwords and then save the password by sending it
 * to the control_mcu
 *
 * Parameters : None (void)
 *
 * Return Value : None (void)
 *******************************************************************************************************************/
void systemFirstTimeStart(void){
	uint8 passwordsMatch = 0;

	do{
		/* get the first password */
		LCD_clearScreen();
		LCD_displayString("Enter ur pass #1");
		getPassword(g_password1); /* get the first password and save it on the global password1 array */

		/* get the second password */
		LCD_clearScreen();
		LCD_displayString("Enter ur pass #2");
		getPassword(g_password2); /*get the second password and save it on the global password2 array*/

		/* check weather the passwords match each other or not */
		passwordsMatch = isMatch();

		/*Display error message in case passwords don't match each other */
		if(!passwordsMatch){
			LCD_clearScreen();
			LCD_displayString("Passwords");
			LCD_displayStringRowColumn("Mismatch!",1,7);
			_delay_ms(1000);
		} else {
			/* passwords match each other then save the password */
			savePassword();
		}
	} while(!passwordsMatch); /* keep doing this until the passwords match each other */

} /* end of systemFirstTimeStart() function */



/*******************************************************************************************************************
 * Function : getPassword
 *
 * Description : used to get the pressed key and save it in the corresponding index in password array
 *
 * Parameters : password : a pointer to uint8 (the address of the first element in password array)
 *
 * Return Value : None (void)
 *******************************************************************************************************************/
void getPassword(uint8 * password){
	uint8 pwCounter = 0; /* password counter */

	LCD_goToRowColumn(1,0);

	/* get the pressed key
	 * save it in the corresponding index in password array
	 * display the pressed key on the screen for 0.5 second then replace it with an asterisk */

	/*loop to get the full password */
	for(pwCounter = 0; pwCounter < pw_length;pwCounter++){
		/* get the pressed key and save it in password array */
		password[pwCounter] = KEYPAD_getPressedKey();

		/* display the pressed key for 0.5 second then replace it with an asterisk */
		/* if the pressed key is a number add it with '0' to display as a character */
		if(password[pwCounter] >= 0 && password[pwCounter] <= 9){
			LCD_displayCharacter(password[pwCounter] + '0');

		} else{
			/* else means that the pressed key is not a number and the ASCII value of the pressed key is saved in pw array so no need to add '0' */
			LCD_displayCharacter(password[pwCounter]);
		}
		_delay_ms(500);
		LCD_goToRowColumn(1,pwCounter);
		LCD_displayCharacter('*');

	} /* end of password's for loop */
} /* end of getPassword() function */




/*******************************************************************************************************************
 * Function : isMatch
 *
 * Description : used to check weather the two passwords match or not
 *
 * Parameters : None (void)
 *
 * Return Value : uint8 : 0 -> passwords don't match
 * 						  1 -> passwords match
 *******************************************************************************************************************/
uint8 isMatch(void){
	uint8 pwCounter = 0; /* password counter */

	/* loop on the password */
	for(pwCounter = 0; pwCounter < pw_length; pwCounter++){
		if(g_password1[pwCounter] != g_password2[pwCounter]){
			/* mismatch occurred */
			return 0;
		} /* end of if statement */
	} /* end of password's for loop*/

	/* if you get out of the loop without returning 0 that means there was no mismatches and the two passwords match each other */
	return 1;

} /* end of isMatch() function */



/*******************************************************************************************************************
 * Function : savePassword
 *
 * Description : used to save the password by sending it to the control_mcu
 *
 * Parameters : None (void)
 *
 * Return Value : None (void)
 *******************************************************************************************************************/
void savePassword(void){
	uint8 pwCounter = 0; /* password counter */

	/* send S (save) command to the control_mcu */
	UART_sendByte('S');

	while(UART_receiveByte() != 'R'); /* polling until the control_mcu is ready to receive the password */

	/* loop on the password number by number (byte by byte) and send it to the control_mcu */
	for(pwCounter = 0; pwCounter < pw_length; pwCounter++){
		UART_sendByte(g_password1[pwCounter] + '0');

	} /* end of password's for loop */

	/* display password saved to the user */
	LCD_clearScreen();
	LCD_displayString("Password");
	LCD_displayStringRowColumn("Saved!",1,7);
	_delay_ms(700);

} /* end of savePassword() function */



/**********************************************************************************************************************
 * Function : verifyPassword
 *
 * Description : used to get the password and then send it to the control_mcu and return the reply from the control_mcu
 *
 * Parameters : option : 1 -> open door
 * 						 2 -> change password
 *
 * Return Value : uint8 : return the reply from the control_mcu
 *
 * control_mcu different replies on verifyPassword() function
 * C : Correct password
 * E : error - password is not correct
 * L : lock the system
 **********************************************************************************************************************/
uint8 verifyPassword(uint8 option){
	uint8 pwCounter; /* password counter */

	/* get the password from the user */
	LCD_clearScreen();
	LCD_displayString("Enter Your Pass:");
	getPassword(g_password2);

	/* send O (open) command to the control_mcu if option is 1*/
	if(option == 1){
		UART_sendByte('O');
	} else UART_sendByte('V'); /* send V (verify but don't open) command to the control_mcu if option is 2 */

	while(UART_receiveByte() != 'R'); /* polling until the control_mcu is ready to receive the password */

	/* loop on the password and send it number by number (byte by byte) */
	for(pwCounter = 0; pwCounter < pw_length; pwCounter++){
		UART_sendByte(g_password2[pwCounter] + '0');
	} /* end of password's for loop */

	return UART_receiveByte(); /* return the reply on the password from the control_mcu */
} /* end of verifyPassword() function */





/*******************************************************************************************************************
 * Function : openDoor
 *
 * Description : used to initiate open door sequence by
 * 1) display "Opening Door!"
 * 2) initialize the timer and stop the system until the door is open
 * 3) display "Door Is Open!"
 * 4) reset timer ticks
 * 5) stop the system for the amount of time required to keep the door open
 * 6) display "Closing Door!"
 * 7) reset timer ticks
 * 8) stop the system until the door is closed
 * 9) display "Door Is Closed!"
 *
 * Parameters : config_ptr : pointer to Timer_Config structure that contains the settings to initialize the timer with.
 *
 * Return Value : None (void)
 *******************************************************************************************************************/
void openDoor(Timer_Config * config_ptr){
	/*reset g_timerTicks */
	g_timerTicks = 0;

	/* display "Opening Door!" message and stop the system until the door is open */
	LCD_clearScreen();
	LCD_displayString("Opening Door!");

	/* initialize the timer */
	TIMER_init(config_ptr);

	/* set the call back function of timer 1 to simply call back timerTick() function when an ISR occurs which increments the g_timerTicks variable */
	TIMER_setCallBack(timerTick,Timer1);

	while(g_timerTicks != openingDoorTime); /* polling/stopping the system until the required opening door time has passed */

	g_timerTicks = 0; /* reset g_TimerTicks */

	/* display "Door Is Open!" and stop the system for the required time to keep the door open */
	LCD_clearScreen();
	LCD_displayString("Door Is Open!");

	while(g_timerTicks != timeToKeepDoorOpen); /* polling/stopping the system until the required time for keeping the door open has passed */

	g_timerTicks = 0; /* reset g_timerTicks */

	/* display "Closing Door!" and stop the system for the required time to close the door */
	LCD_clearScreen();
	LCD_displayString("Closing Door");

	while(g_timerTicks != closingDoorTime); /* polling/stopping the system until the required time for closing the door has passed */

	g_timerTicks = 0; /* reset g_timerTicks */

	/* display "Door Is Closed!" for 0.5 second */
	LCD_clearScreen();
	LCD_displayString("Door Is Closed");
	_delay_ms(500);

	/* timer de-init */
	TIMER_deInit(Timer1);

} /* end of openDoor() function */



/*******************************************************************************************************************
 * Function : lockSystem
 *
 * Description : used to lock the system in case of theft
 *
 * Parameters : config_ptr : pointer to Timer_Config structure which contains the setting to initialize the timer with
 *
 * Return Value : None (void)
 *******************************************************************************************************************/
void lockSystem(Timer_Config * config_ptr){
	/* reset g_timerTicks */
	g_timerTicks = 0;

	/* initialize the timer with config_ptr */
	TIMER_init(config_ptr);

	/* set the call back function of timer 1 to simply call back timerTick() function when an ISR occurs which increments the g_timerTicks variable */
	TIMER_setCallBack(timerTick,Timer1);

	/* display the lock system message and keep the system locked for the required lock time */
	LCD_clearScreen();
	LCD_displayStringRowColumn("Calling",0,4);
	LCD_displayStringRowColumn("911",1,6);

	while(g_timerTicks != lockTime); /* polling/stopping/locking the system until the lock time has passed */

	/* reset g_timerTicks */
	g_timerTicks = 0;

	/* timer de-init */
	TIMER_deInit(Timer1);
} /* end of lockSystem() function */


/**********************************************************************************************************************************************
 * Function : timerTick
 *
 * Description : used to increment the g_timerTicks variable to show there has been a timer tick (in this project a tick is equal to 1 second)
 * this function is called back by the timer when its ISR is being handled
 *
 * Parameters : config_ptr : pointer to Timer_Config structure which contains the setting to initialize the timer with
 *
 * Return Value : None (void)
 ***********************************************************************************************************************************************/
void timerTick(){
	/* increment g_timerTicks variable */
	g_timerTicks++;

} /* end of timerTick() function*/





