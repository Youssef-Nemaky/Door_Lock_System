/*
 * keypad.h
 *
 *  Created on: Feb 15, 2021
 *      Author: Youssef
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

#define N_rows 4
#define N_cols 4

#define KEYPAD_PORT_DIR DDRC
#define KEYPAD_PORT_IN  PINC
#define KEYPAD_PORT_OUT	PORTC

uint8 KEYPAD_getPressedKey(void);

#endif /* KEYPAD_H_ */
