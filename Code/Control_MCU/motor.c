/*
 * motor.c
 *
 *  Created on: Apr 2, 2021
 *      Author: Youssef El-Nemaky
 */

#include "motor.h"


/*******************************************************************************************************************
 * Function : MOTOR_init
 *
 * Description : used to initialize motor
 *
 * Parameters : None (void)
 *
 * Return Value : None (void)
 *******************************************************************************************************************/
void MOTOR_init(void){
	/* configure motor pins to be output */
	MOTOR_DATA_DIRECTION_REGISTER |= (1<<MOTOR_PIN_NUMBER0) | (1<<MOTOR_PIN_NUMBER1);

	/* stop the motor at initialization */
	MOTOR_stop();
} /* end of MOTOR_init() function */

/*******************************************************************************************************************
 * Function : MOTOR_rotateClockWise
 *
 * Description : used to rotate the motor clockwise
 *
 * Parameters : None (void)
 *
 * Return Value : None (void)
 *******************************************************************************************************************/
void MOTOR_rotateClockWise(void){
	/* configure first pin of the motor to output 1*/
	SET_BIT(MOTOR_PORT_REGISTER, MOTOR_PIN_NUMBER0);

	/* configure second pin of the motor to output 0*/
	CLEAR_BIT(MOTOR_PORT_REGISTER, MOTOR_PIN_NUMBER1);

} /* end of MOTOR_rotateClockWise() function */


/*******************************************************************************************************************
 * Function : MOTOR_rotateAntiClockWise
 *
 * Description : used to rotate the motor anti-clockwise
 *
 * Parameters : None (void)
 *
 * Return Value : None (void)
 *******************************************************************************************************************/
void MOTOR_rotateAntiClockWise(void){
	/* configure the first pin of the motor to output 0 */
	CLEAR_BIT(MOTOR_PORT_REGISTER, MOTOR_PIN_NUMBER0);

	/* configure the second pin of the motor to output 1 */
	SET_BIT(MOTOR_PORT_REGISTER, MOTOR_PIN_NUMBER1);

} /* end of MOTOR_rotateAntiClockWise() function */


/*******************************************************************************************************************
 * Function : MOTOR_stop
 *
 * Description : used to stop the motor
 *
 * Parameters : None (void)
 *
 * Return Value : None (void)
 *******************************************************************************************************************/
void MOTOR_stop(void){
	/* output 0 on the first pin of the motor */
	CLEAR_BIT(MOTOR_PORT_REGISTER, MOTOR_PIN_NUMBER0);

	/* output 0 on the second pin of the motor */
	CLEAR_BIT(MOTOR_PORT_REGISTER, MOTOR_PIN_NUMBER1);

} /* end of MOTOR_stop() function */
