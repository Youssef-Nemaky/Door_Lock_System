/*
 * buzzer.h
 *
 *  Created on: Mar 31, 2021
 *      Author: Youssef El-Nemaky
 */

#ifndef BUZZER_H_
#define BUZZER_H_

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

#define BUZZER_DATA_DIRECTION_REGISTER DDRD
#define BUZZER_PORT_REGISTER PORTD
#define BUZZER_PIN_REGISTER PIND

#define BUZZER_PIN_NUMBER PD5

/**************************************************************
 *				FUNCTIONS PROTOTYPES
 **************************************************************/
void BUZZER_init(void);
void BUZZER_off(void);
void BUZZER_on(void);

#endif /* BUZZER_H_ */
