/*
 * HIM_functions.h
 *
 *  Created on: Mar 29, 2021
 *      Author: Youssef El-Nemaky
 */

#ifndef HIM_FUNCTIONS_H_
#define HIM_FUNCTIONS_H_

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"
#include "lcd.h"
#include "uart.h"
#include "keypad.h"
#include "timer.h"


#define pw_length 5
#define openingDoorTime 15
#define closingDoorTime 15
#define timeToKeepDoorOpen 3
#define lockTime 60
#define timerInitialValue 0
#define timerCompareValue 31250UL


/****************************************************
 *				EXTERNAL VARIABLES
 ****************************************************/
extern uint8 g_password1[pw_length];
extern uint8 g_password2[pw_length];
extern uint8 g_timerTicks;


/****************************************************
 *				FUNCTIONS PROTOTYPES
 ****************************************************/
uint8 isFirstTime(void);
void systemFirstTimeStart(void);
void getPassword(uint8 *);
uint8 isMatch(void);
void savePassword(void);
uint8 verifyPassword(uint8);
void openDoor(Timer_Config *);
void timerTick(void);
void lockSystem(Timer_Config *);
uint8 isPoweredDownWhileInLock(void);
#endif /* HIM_FUNCTIONS_H_ */
