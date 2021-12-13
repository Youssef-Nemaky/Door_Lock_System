/*
 * timer.c
 *
 *  Created on: Mar 26, 2021
 *      Author: Youssef El-Nemaky
 */

#include "timer.h"

/************************************
 * POINTERS FOR CALL BACKS FUNCTIONS
 ************************************/
static volatile void (*g_Timer0_callBackPtr)(void) = NULL_PTR;
static volatile void (*g_Timer1_callBackPtr)(void) = NULL_PTR;
static volatile void (*g_Timer2_callBackPtr)(void) = NULL_PTR;

/************************************
 * ISRs : INTERRUPTS SERVICE ROUTINES
 ************************************/


/* TIMER0 ISRs */

ISR(TIMER0_OVF_vect){
	/* TIMER0 OVERFLOW ISR */
	if(g_Timer0_callBackPtr != NULL_PTR){
		(*g_Timer0_callBackPtr)();
	}
}
ISR(TIMER0_COMP_vect){
	/* TIMER0 COMPARE MATCH ISR*/
	if(g_Timer0_callBackPtr != NULL_PTR){
		(*g_Timer0_callBackPtr)();
	}
}


/* TIMER1 ISRs */

ISR(TIMER1_OVF_vect){
	/* TIMER1 OVERFLOW ISR */
	if(g_Timer1_callBackPtr != NULL_PTR){
		(*g_Timer1_callBackPtr)();
	}
}

ISR(TIMER1_COMPA_vect){
	/* TIMER1 COMPARE MATCH ISR */
	if(g_Timer1_callBackPtr != NULL_PTR){
		(*g_Timer1_callBackPtr)();
	}
}

/* TIMER2 ISRs */

ISR(TIMER2_OVF_vect){
	/* TIMER2 OVERFLOW ISR*/
	if(g_Timer2_callBackPtr != NULL_PTR){
		(*g_Timer2_callBackPtr)();
	}
}

ISR(TIMER2_COMP_vect){
	/* TIMER2 COMPARE MATCH ISR*/
	if(g_Timer2_callBackPtr != NULL_PTR){
		(*g_Timer2_callBackPtr)();
	}
}


/*******************************************************************************************************************
 * Function : TIMER_init
 *
 * Description : used to initialize the timer with the required settings through a pointer to Timer_Config structure
 *
 * Parameters : config_ptr : pointer to Timer_Config structure which contains the settings to initialize the timer with
 *
 * Return Value : None (void)
 *******************************************************************************************************************/
void TIMER_init(const Timer_Config * config_ptr){
	switch(config_ptr->timer_no){
	case Timer0:

		/* RESET TIMER0 SETTINGS */
		TIMER0_CONTROL_REGISTER         = 0;
		TIMER0_INITIAL_VALUE_REGISTER    = 0;
		TIMER0_OUTPUT_COMPARE_REGISTER  = 0;
		CLEAR_BIT(TIMER_INTERRUPT_MASK_REGISTER, TIMER0_OVERFLOW_INTERRUPT_ENABLE_BIT);
		CLEAR_BIT(TIMER_INTERRUPT_MASK_REGISTER, TIMER0_OUTPUT_COMPARE_MATCH_INTERRUPT_ENABLE_BIT);

		/* Load TCNT0 with the timer_initialValue */
		TIMER0_INITIAL_VALUE_REGISTER = config_ptr->timer_initialValue;

		/* Load OCR with timer_compareMatchValue*/
		TIMER0_OUTPUT_COMPARE_REGISTER = config_ptr->timer_compareMatchValue;



		/* MODE SELECTION */

		if(config_ptr->timer_mode == Overflow){
			/* configure force output compare bit to be active as it is non-pwm mode */
			SET_BIT(TIMER0_CONTROL_REGISTER, TIMER0_FORCE_OUTPUT_COMPARE_BIT);

			/* configure waveform generation mode to work on normal mode (overflow) by setting
			 * WGM00 -> 0
			 * WGM01 -> 0 */

			/* clear WGM00 */
			CLEAR_BIT(TIMER0_CONTROL_REGISTER, WGM00);

			/* clear WGM01 */
			CLEAR_BIT(TIMER0_CONTROL_REGISTER, WGM01);


			/* configure TIMSK to enable overflow interrupt for timer 0 */
			SET_BIT(TIMER_INTERRUPT_MASK_REGISTER, TIMER0_OVERFLOW_INTERRUPT_ENABLE_BIT);

			/* configure TIMSK to disable output compare match interrupt for timer 0 */
			CLEAR_BIT(TIMER_INTERRUPT_MASK_REGISTER, TIMER0_OUTPUT_COMPARE_MATCH_INTERRUPT_ENABLE_BIT);

		} else if(config_ptr->timer_mode == Compare){
			/* compare match mode*/

			/* configure force output compare bit to be active as it is non-pwm mode */
			SET_BIT(TIMER0_CONTROL_REGISTER, FOC0);

			/* Configure waveform generation mode to work on compare match mode by setting
			 * WGM00 -> 0
			 * WGM01 -> 1 */
			CLEAR_BIT(TIMER0_CONTROL_REGISTER, WGM00);
			SET_BIT(TIMER0_CONTROL_REGISTER, WGM01);

			/* configure TIMSK to disable overflow interrupt for timer 0 */
			SET_BIT(TIMER_INTERRUPT_MASK_REGISTER, TIMER0_OUTPUT_COMPARE_MATCH_INTERRUPT_ENABLE_BIT);

			/* configure TISMK to enable output compare match interrupt for timer 0 */
			CLEAR_BIT(TIMER_INTERRUPT_MASK_REGISTER, TIMER0_OVERFLOW_INTERRUPT_ENABLE_BIT);
		} /* end of else if statement */

		/* configure the timer to work on the timer_clock setting */
		/* reset the 3 bits of clock selection (CS00 , CS01 , CS02) and then put the setting of the timer_clock setting in TCCR0 register */
		TIMER0_CONTROL_REGISTER = (TIMER0_CONTROL_REGISTER & 0xf8) | config_ptr->timer_clock;

	break; /* break out of switch statement (timer0 settings)*/

	case Timer1:
		/* RESET TIMER1 SETTINGS */
		TIMER1_CONTROL_REGISTER_A = 0;
		TIMER1_CONTROL_REGISTER_B = 0;
		TIMER1_INITIAL_VALUE_REGISTER = 0;
		TIMER1_OUTPUT_COMPARE_MATCH_REGISTER_A = 0;
		TIMER1_OUTPUT_COMPARE_MATCH_REGISTER_B = 0;
		CLEAR_BIT(TIMER_INTERRUPT_MASK_REGISTER, TIMER1_OVERFLOW_INTERRUPT_ENABLE_BIT);
		CLEAR_BIT(TIMER_INTERRUPT_MASK_REGISTER, TIMER1_OUTPUT_COMPARE_MATCH_A_INTERRUPT_ENABLE_BIT);

		/* load TCNT1 with timer_initialValue */
		TIMER1_INITIAL_VALUE_REGISTER = config_ptr->timer_initialValue;

		/* load OCR1A with timer_compareMatchValue */
		TIMER1_OUTPUT_COMPARE_MATCH_REGISTER_A = config_ptr->timer_compareMatchValue;

		/* MODE SELECTION */

		if(config_ptr->timer_mode == Overflow){

			/* configure waveform generation mode to work on overflow mode by setting
			 * WGM10 -> 0 (in TCCR1A)
			 * WGM11 -> 0 (in TCCR1A)
			 * WGM12 -> 0 (in TCCR1B)
			 * WGM13 -> 0 (in TCCR1B) */

			/* clear WGM10 */
			CLEAR_BIT(TIMER1_CONTROL_REGISTER_A, TIMER1_WAVEFORM_GENERATION_MODE_BIT0);

			/* clear WGM11*/
			CLEAR_BIT(TIMER1_CONTROL_REGISTER_A, TIMER1_WAVEFORM_GENERATION_MODE_BIT1);

			/* clear WGM12*/
			CLEAR_BIT(TIMER1_CONTROL_REGISTER_B, TIMER1_WAVEFORM_GENERATION_MODE_BIT2);

			/* clear WGM13*/
			CLEAR_BIT(TIMER1_CONTROL_REGISTER_B, TIMER1_WAVEFORM_GENERATION_MODE_BIT3);



			/* configure TIMSK to enable overflow interrupt for timer 1 */
			SET_BIT(TIMER_INTERRUPT_MASK_REGISTER, TIMER1_OVERFLOW_INTERRUPT_ENABLE_BIT);

			/* configure TISMK to disable output compare match interrupt for timer 1 */
			CLEAR_BIT(TIMER_INTERRUPT_MASK_REGISTER, TIMER1_OUTPUT_COMPARE_MATCH_A_INTERRUPT_ENABLE_BIT);

		} else if(config_ptr->timer_mode == Compare){

			/* configure waveform generation mode to work on compare match mode by setting
			 * WGM10 -> 0 (in TCCR1A)
			 * WGM11 -> 0 (in TCCR1A)
			 * WGM12 -> 1 (in TCCR1B)
			 * WGM13 -> 0 (in TCCR1B) */

			/* clear WGM10 */
			CLEAR_BIT(TIMER1_CONTROL_REGISTER_A, TIMER1_WAVEFORM_GENERATION_MODE_BIT0);

			/*clear WGM11 */
			CLEAR_BIT(TIMER1_CONTROL_REGISTER_A, TIMER1_WAVEFORM_GENERATION_MODE_BIT1);

			/* set WGM12 */
			SET_BIT(TIMER1_CONTROL_REGISTER_B, TIMER1_WAVEFORM_GENERATION_MODE_BIT2);

			/* clear WGM13 */
			CLEAR_BIT(TIMER1_CONTROL_REGISTER_B, TIMER1_WAVEFORM_GENERATION_MODE_BIT3);

			/* configure TIMSK to disable overflow interrupt for timer 1 */
			SET_BIT(TIMER_INTERRUPT_MASK_REGISTER, TIMER1_OUTPUT_COMPARE_MATCH_A_INTERRUPT_ENABLE_BIT);

			/* configure TISMK to enable output compare match interrupt for timer 1 */
			CLEAR_BIT(TIMER_INTERRUPT_MASK_REGISTER, TIMER1_OVERFLOW_INTERRUPT_ENABLE_BIT);
		} /* end of else if statement */

		/* configure the timer to work on the timer_clock setting */
		/* reset the 3 bits of clock selection (CS00 , CS01 , CS02) and then put the setting of the timer_clock setting in TCCR0 register */
		TIMER1_CONTROL_REGISTER_B = (TIMER1_CONTROL_REGISTER_B & 0xf8) | config_ptr->timer_clock;

	break; /*break out of switch statement (timer1 settings) */


	case Timer2:

		/* RESET TIMER2 SETTINGS */
		TIMER2_ASYNCHRONOUS_STATUS_REGSITER = 0;
		TIMER2_CONTROL_REGISTER = 0;
		TIMER2_INITIAL_VALUE_REGISTER  = 0;
		TIMER2_OUTPUT_COMPARE_REGISTER = 0;
		CLEAR_BIT(TIMER_INTERRUPT_MASK_REGISTER, TIMER2_OVERFLOW_INTERRUPT_ENABLE_BIT);
		CLEAR_BIT(TIMER_INTERRUPT_MASK_REGISTER, TIMER2_OUTPUT_COMPARE_MATCH_INTERRUPT_ENABLE_BIT);

		/* load TCNT2 with timer_initialValue */
		TIMER2_INITIAL_VALUE_REGISTER = config_ptr->timer_initialValue;

		/* load OCR2 with timer_compareMatchValue */
		TIMER2_OUTPUT_COMPARE_REGISTER = config_ptr->timer_compareMatchValue;

		/* MODE SELECTION */

		if(config_ptr->timer_mode == Overflow){
			/* configure force output compare to be active as it is a non-pwm mode*/
			SET_BIT(TIMER0_CONTROL_REGISTER, FOC2);
			/* configure waveform generation to work on overflow mode by setting
			 * WGM20 -> 0
			 * WGM21 -> 0*/

			/* clear WGM20 */
			CLEAR_BIT(TIMER2_CONTROL_REGISTER, WGM20);

			/* clear WGM21 */
			CLEAR_BIT(TIMER2_CONTROL_REGISTER, WGM21);

			/* configure TIMSK to enable overflow interrupt for timer2 */
			SET_BIT(TIMER_INTERRUPT_MASK_REGISTER, TIMER2_OVERFLOW_INTERRUPT_ENABLE_BIT);

			/* configure TIMSK to disable output compare match interrupt for timer2 */
			CLEAR_BIT(TIMER_INTERRUPT_MASK_REGISTER, TIMER2_OUTPUT_COMPARE_MATCH_INTERRUPT_ENABLE_BIT);

		} else if(config_ptr->timer_mode == Compare){
			/* configure force output compare to be active as it is a non-pwm mode */
			SET_BIT(TIMER0_CONTROL_REGISTER, FOC2);
			/* configure waveform generation mode to work on compare match mode by setting
			 * WGM20 -> 0
			 * WGM21 -> 1*/

			/* clear WGM20 */
			CLEAR_BIT(TIMER2_CONTROL_REGISTER, WGM20);

			/* set WGM21*/
			SET_BIT(TIMER2_CONTROL_REGISTER, WGM21);

			/* configure TIMSK to disable overflow interrupt for timer2 */
			CLEAR_BIT(TIMER_INTERRUPT_MASK_REGISTER, TIMER2_OVERFLOW_INTERRUPT_ENABLE_BIT);

			/* configure TIMSK to enable output compare match interrupt for timer2 */
			SET_BIT(TIMER_INTERRUPT_MASK_REGISTER, TIMER2_OUTPUT_COMPARE_MATCH_INTERRUPT_ENABLE_BIT);
		} /* end of else if statement */

		/* configure the timer to work on the timer_clock setting */
		/* reset the 3 bits of clock selection (CS00 , CS01 , CS02) and then put the setting of the timer_clock setting in TCCR0 register */
		TIMER2_CONTROL_REGISTER = (TIMER2_CONTROL_REGISTER & 0xf8) | config_ptr->timer_clock;

	break; /*break out of switch statement (timer2 settings) */
	} /* end of switch statement */
} /* end of TIMER_init() function */



/*******************************************************************************************************************
 * Function : TIMER_deInit
 *
 * Description : used to de-initialize the timer and reset it to the default settings
 *
 * Parameters : timer_no : to choose which timer to de-init
 *
 * Return Value : None (void)
 *******************************************************************************************************************/
void TIMER_deInit(Timer_Number timer_no){
	switch(timer_no)
	{
	case Timer0:

		/* reset  all registers in Timer0 */
		TIMER0_CONTROL_REGISTER         = 0;
		TIMER0_INITIAL_VALUE_REGISTER    = 0;
		TIMER0_OUTPUT_COMPARE_REGISTER  = 0;
		CLEAR_BIT(TIMER_INTERRUPT_MASK_REGISTER,TIMER0_OVERFLOW_INTERRUPT_ENABLE_BIT);
		CLEAR_BIT(TIMER_INTERRUPT_MASK_REGISTER,TIMER0_OUTPUT_COMPARE_MATCH_INTERRUPT_ENABLE_BIT);
		break; /* break out of switch statement */

	case Timer1:

		/* reset all registers in Timer1 */
		TIMER1_CONTROL_REGISTER_A         		 = 0;
		TIMER1_CONTROL_REGISTER_B         		 = 0;
		TIMER1_INITIAL_VALUE_REGISTER     		 = 0;
		TIMER1_OUTPUT_COMPARE_MATCH_REGISTER_A   = 0;
		TIMER1_OUTPUT_COMPARE_MATCH_REGISTER_B   = 0;
		CLEAR_BIT(TIMER_INTERRUPT_MASK_REGISTER,TIMER1_OVERFLOW_INTERRUPT_ENABLE_BIT);
		CLEAR_BIT(TIMER_INTERRUPT_MASK_REGISTER,TIMER1_OUTPUT_COMPARE_MATCH_A_INTERRUPT_ENABLE_BIT);
		break; /* break out of swtich statement */


	case Timer2:

		/* reset all registers in Timer2 */
		TIMER2_CONTROL_REGISTER         = 0;
		TIMER2_INITIAL_VALUE_REGISTER   = 0;
		TIMER2_OUTPUT_COMPARE_REGISTER  = 0;
		CLEAR_BIT(TIMER_INTERRUPT_MASK_REGISTER,TIMER2_OVERFLOW_INTERRUPT_ENABLE_BIT);
		CLEAR_BIT(TIMER_INTERRUPT_MASK_REGISTER,TIMER2_OVERFLOW_INTERRUPT_ENABLE_BIT);
		break; /* break out of switch statement */

	} /* end of the switch statement */

}/* end of TIMER_deInit() function*/



/*******************************************************************************************************************
 * Function : TIMER_setCallBack
 *
 * Description : used to initialize the call back pointers with the address passed to its parameter
 * the call back pointers would contain the address of the function that will be called when an ISR is being handled
 *
 * Parameters : 1) a_ptr : pointer to function that will have the address of the function that will be called back when
 * an ISR occurs
 *				2) timer_no : chooses which timer to save the address of the function to its call back pointer
 *
 * Return Value : None (void)
 *******************************************************************************************************************/
void TIMER_setCallBack(void (*a_ptr)(void), Timer_Number timer_no){
	switch(timer_no){
	case Timer0:
		g_Timer0_callBackPtr = a_ptr;
	break;
	case Timer1:
		g_Timer1_callBackPtr = a_ptr;
	break;
	case Timer2:
		g_Timer2_callBackPtr = a_ptr;
	break;
	} /* end of switch statement */
} /* end of TIMER_setCallBack() function */

