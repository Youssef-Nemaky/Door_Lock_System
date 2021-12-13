/*
 * CM_functions.h
 *
 *  Created on: Mar 31, 2021
 *      Author: Youssef El-Nemaky
 */

#ifndef CM_FUNCTIONS_H_
#define CM_FUNCTIONS_H_

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"
#include "timer.h"
#include "uart.h"
#include "external_eeprom.h"
#include "buzzer.h"
#include "motor.h"

#define pw_length 5
#define maxWrongAttempts 3
#define timerInitialValue 0
#define timerCompareValue 31250UL
#define openingDoorTime 15
#define closingDoorTime 15
#define timeToKeepDoorOpen 3
#define lockTime 60

/****************************************************
 *				EEPROM ADDRESSES
 ****************************************************/
#define addressOfFirstTimeVariable 0x0311
#define addressOfWrongAttempts 0x0312
#define addressOfPassword 0x0319 /* base address */

/****************************************************
 *				EXTERNAL VARIABLES
 ****************************************************/
extern uint8 g_password1[pw_length];
extern uint8 g_password2[pw_length];
extern uint8 g_numberOfWrongAttempts;
extern uint8 g_firstTime;
extern uint8 g_timerTicks;


/****************************************************
 *				FUNCTIONS PROTOTYPES
 ****************************************************/
void getPassword(void);
void savePassword(void);
uint8 isFirstTimeStart();
uint8 verifyPassword();
void lockSystem(Timer_Config *);
void openDoor(Timer_Config *);
void timerTick();


#endif /* CM_FUNCTIONS_H_ */
