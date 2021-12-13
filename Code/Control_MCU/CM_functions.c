/*
 * CM_functions.c
 *
 *  Created on: Mar 31, 2021
 *      Author: Youssef El-Nemaky
 */

#include "CM_functions.h"

/*******************************************************************************************************************
 * Function : getPassword
 *
 * Description : used to get the password from him_mcu through UART communication protocol
 *
 * Parameters : None (void)
 *
 * Return Value : None (void)
 *******************************************************************************************************************/
void getPassword(void){
	uint8 pwCounter; /* password counter */
	UART_sendByte('R');  /* send ready to receive the password to him_mcu */

	/* loop to get the password number by number (byte by byte) through UART communication protocol */
	for(pwCounter = 0; pwCounter < pw_length; pwCounter++){
		g_password2[pwCounter] = UART_receiveByte(); /* get the number (byte) and save it in g_passwrod2 array */

	} /* end of password's loop */

} /* end of getPassword() function */


/*******************************************************************************************************************
 * Function : savePassword
 *
 * Description : used to save the password in g_password2 array to EEPROM after getting it through getPassword()
 * function through UART communication protocol
 *
 * Parameters : None (void)
 *
 * Return Value : None (void)
 *******************************************************************************************************************/
void savePassword(void){
	uint8 pwCounter;
	getPassword(); /* get the password through UART communication protocol and save it g_password2 */


	if(g_firstTime == 0xFF){
		/* change the status of g_firstTime variable as it will not be the first time to run the system anymore */
		g_firstTime = 0;

		/* change the status of firstTimeVariable in EEPROM */
		EEPROM_writeByte(addressOfFirstTimeVariable,g_firstTime);
		_delay_ms(80);
	}

	/* loop through the password and save it in EEPROM byte by byte */
	for(pwCounter = 0; pwCounter < pw_length; pwCounter++){
		g_password1[pwCounter] = g_password2[pwCounter]; /* update the value of g_password1 as it would be faster to work on it than EEPROM later on */

		EEPROM_writeByte((addressOfPassword + pwCounter),g_password2[pwCounter]); /* save the password in EEPROM */
		_delay_ms(100);
	} /* end of password's loop */
} /* end of savePassword() function */



/*******************************************************************************************************************
 * Function : isFirstTimeStart
 *
 * Description : used to check weather it is first time to run the system or not by checking on the
 * g_firstTime variable which is updated through at the begining of the run of the program from EEPROM
 *
 * Parameters : None (void)
 *
 * Return Value : uint8 : return 1 -> first time
 *  							 0 -> not first time
 *******************************************************************************************************************/
uint8 isFirstTimeStart(void){

	if(g_firstTime == 0xFF){ /* initial value of EEPROM is 0xFF */
		/* first time */
		return 1;
	} else {
		/* not first time */
		return 0;
	} /* end of else statement*/
} /* end of isFirstTimeStart() function */


/*******************************************************************************************************************
 * Function : verifyPassword
 *
 * Description : used to get the password from him_mcu and compare it with the saved password in EEPROM
 *
 * Parameters : None (void)
 *
 * Return Value : uint8 : return 1 -> they match
 * 		  						 0 -> they don't match
 *******************************************************************************************************************/
uint8 verifyPassword(void){
	uint8 pwCounter; /* password counter */

	getPassword(); /* get the password from him_mcu */

	/* read the password from EEPROM number by number (byte by byte) and save it in g_password1 */
	for(pwCounter = 0; pwCounter < pw_length; pwCounter++){
		EEPROM_readByte((addressOfPassword + pwCounter),(&g_password1[0] + pwCounter)); /* read the password from EEPROM and save it in g_password1 */
		_delay_ms(80);


		if(g_password1[pwCounter] != g_password2[pwCounter]){
			/*the number (byte) read from EEPROM does not match the number (byte) received from him */
			return 0;
		} /* end of if statement */
	} /* end of reading password from EEPROM loop */

	/* if there was no return 0 at all that means both passwords match each other */
	return 1;
} /* end of verifyPassword() function */



/*******************************************************************************************************************
 * Function : lockSystem
 *
 * Description : used to lock the system by
 * 1) turn on the buzzer
 * 2) initializing the timer and pause the system for the required lock time
 * 3) turn off the buzzer and unlock the system
 *
 * Parameters : None (void)
 *
 * Return Value : uint8 : return 1 -> they match
 * 		  						 0 -> they don't match
 *******************************************************************************************************************/
void lockSystem(Timer_Config * config_ptr){
	/* reset g_timerTicks variable */
	g_timerTicks = 0;

	/* turn on the buzzer */
	BUZZER_on();
	/* initialize the timer with config_ptr */
	TIMER_init(config_ptr);

	/* set the call back function to simply call timerTick() function which increments the g_timerTicks by 1 which means 1 second has passed when an ISR is being handled */
	TIMER_setCallBack(timerTick,Timer1);


	while(g_timerTicks != lockTime); /* polling and lock the system until the lock time has passed */

	/* turn off the buzzer */
	BUZZER_off();
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
void timerTick(void){
	/* increment g_timerTicks variable */
	g_timerTicks++;
} /* end of timerTick() function */



/*******************************************************************************************************************
 * Function : openDoor
 *
 * Description : used to initiate open door sequence by
 * 1) rotate the motor clock wise (open door)
 * 2) initialize the timer and keep rotating the motor until the door is open
 * 3) stop the motor
 * 4) reset g_timerTicks variable
 * 5) stop the system for the amount of time required to keep the door open
 * 6) rotate the motor anti clock wise (close door)
 * 7) reset g_timerTicks
 * 8) keep rotating the motor until the door is closed
 * 9) stop the motor (door is now open)
 *
 * Parameters : config_ptr : pointer to Timer_Config structure that contains the settings to initialize the timer with.
 *
 * Return Value : None (void)
 *******************************************************************************************************************/
void openDoor(Timer_Config * config_ptr){
	/* rotate the motor clock wise */
	MOTOR_rotateClockWise();
	/* keep rotating the motor clock wise until the the time required to open the door has passed */
	g_timerTicks = 0;
	TIMER_init(config_ptr);
	TIMER_setCallBack(timerTick,Timer1);
	while(g_timerTicks != openingDoorTime);

	/* stop the motor for the amount of time required to keep the door open */
	MOTOR_stop();
	g_timerTicks = 0;
	TIMER_init(config_ptr);
	TIMER_setCallBack(timerTick,Timer1);
	while(g_timerTicks != timeToKeepDoorOpen);

	/* keep rotating the motor anti clock wise until the time required to close the door has passed */
	MOTOR_rotateAntiClockWise();
	g_timerTicks = 0;
	TIMER_init(config_ptr);
	TIMER_setCallBack(timerTick,Timer1);
	while(g_timerTicks != openingDoorTime);

	/*stop rotating the motor as the door has been closed */
	MOTOR_stop();
} /* end of openDoor() function */


