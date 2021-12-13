/*
 * buzzer.c
 *
 *  Created on: Mar 31, 2021
 *      Author: Youssef El-Nemaky
 */

#include "buzzer.h"

/*******************************************************************************************************************
 * Function : BUZZER_init
 *
 * Description : used to initialize buzzer
 *
 * Parameters : None (void)
 *
 * Return Value : None (void)
 *******************************************************************************************************************/
void BUZZER_init(void){
	/* make buzzer pin output */
	SET_BIT(BUZZER_DATA_DIRECTION_REGISTER, BUZZER_PIN_NUMBER);

	/* turn off the buzzer at initialization */
	BUZZER_off();

} /* end of BUZZER_init() function */


/*******************************************************************************************************************
 * Function : BUZZER_off
 *
 * Description : used to turn off the buzzer
 *
 * Parameters : None (void)
 *
 * Return Value : None (void)
 *******************************************************************************************************************/
void BUZZER_off(void){
	/* output 0 on buzzer pin by clearing buzzer pin in port register */
	CLEAR_BIT(BUZZER_PORT_REGISTER, BUZZER_PIN_NUMBER);

} /* end of BUZZER_off() function*/


/*******************************************************************************************************************
 * Function : BUZZER_on
 *
 * Description : used to turn on the buzzer
 *
 * Parameters : None (void)
 *
 * Return Value : None (void)
 *******************************************************************************************************************/
void BUZZER_on(void){
	/* output 1 on buzzer pin by setting buzzer pin in port register */
	SET_BIT(BUZZER_PORT_REGISTER, BUZZER_PIN_NUMBER);

} /* end of BUZZER_on() function */
