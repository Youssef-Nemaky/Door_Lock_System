/*
 * motor.h
 *
 *  Created on: Apr 2, 2021
 *      Author: Youssef El-Nemaky
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

#define MOTOR_DATA_DIRECTION_REGISTER DDRD
#define MOTOR_PORT_REGISTER PORTD


#define MOTOR_PIN_NUMBER0 PD6
#define MOTOR_PIN_NUMBER1 PD7

/**************************************************************
 *				FUNCTIONS PROTOTYPES
 **************************************************************/
void MOTOR_init();
void MOTOR_rotateClockWise();
void MOTOR_rotateAntiClockWise();
void MOTOR_stop();

#endif /* MOTOR_H_ */
