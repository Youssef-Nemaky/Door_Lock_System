/*
 * uart.h
 *
 *  Created on: Mar 28, 2021
 *      Author: Youssef El-Nemaky
 */

#ifndef UART_H_
#define UART_H_

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

/* USART_MODE can be either
 * 0 -> Asynchronous Normal Mode (U2X = 0)
 * 1 -> Asynchronous Double Speed Mode (U2X = 1)
 * 2 -> Synchronous Master Mode */

#define USART_MODE 0
#define USART_BAUD_RATE 9600


#define S_MODE 0 /* no synchronous mode */
#define DOUBLE_SPEED_MODE 1

#if (USART_MODE == 0)

	#define UBRR_VALUE ((F_CPU/ (16 * USART_BAUD_RATE)) - 1)

#elif (USART_MODE == 1)
	#define UBRR_VALUE ((F_CPU/ (8 * USART_BAUD_RATE)) - 1)
	#undef DOUBLE_SPEED_MODE
	#define DOUBLE_SPEED_MODE 0
#elif (USART_MODE == 2)
	#define UBRR_VALUE ((F_CPU/ (2 * USART_BAUD_RATE)) - 1)
    #undef S_MODE
    #define S_MODE 1
	#undef DOUBLE_SPEED_MODE
	#define DOUBLE_SPEED_MODE 0
#endif


/* USART Registers*/
#define USART_DATA_REGISTER UDR
#define USART_CONTROL_AND_STATUS_REGISTER_A UCSRA
#define USART_CONTROL_AND_STATUS_REGISTER_B UCSRB
#define USART_CONTROL_AND_STATUS_REGISTER_C UCSRC
#define USART_BAUD_RATE_REGISTER_LOW  UBRRL
#define USART_BAUD_RATE_REGISTER_HIGH UBRRH

/* UCSRA BITS : USART CONTROL AND STATUS REGISTER A BITS */
#define USART_RECEIVE_COMPLETE_FLAG_BIT  RXC
#define USART_TRANSMIT_COMPLETE_FLAG_BIT TXC
#define USART_DATA_REGISTER_EMPTY_FLAG_BIT UDRE
#define USART_FRAME_ERROR_FLAG_BIT FE
#define USART_DATA_OVERRUN_FLAG_BIT DOR
#define USART_PARITY_ERROR_FLAG_BIT PE
#define USART_DOUBLE_TRANSMISSION_SPEED_BIT U2X
#define USART_MULTI_PROCESSOR_COMMUNICATION_MODE_BIT MPCM

/* UCSRB BITS : USART CONTROL AND STATUS REGISTER B BITS */
#define USART_RX_COMPLETE_INTERRUPT_ENABLE_BIT RXCIE
#define USART_TX_COMPLETE_INTERRUPT_ENABLE_BIT TXCIE
#define USART_DATA_REGISTER_EMPTY_INTERRUPT_ENABLE_BIT UDRIE
#define USART_RECEIVER_ENABLE_BIT RXEN
#define USART_TRANSMITTER_ENABLE_BIT TXEN
#define USART_CHARACTER_SIZE_BIT2 UCSZ2
#define USART_RECIVE_DATA_BIT_8 RXB8

/* UCSRC BITS : USART CONTROL AND STATUS REGISTER C BITS */
#define USART_REGISER_SELECT_BIT URSEL
#define USART_MODE_SELECT_BIT UMSEL
#define USART_PARITY_MODE_BIT0 UPM0
#define USART_PARITY_MODE_BIT1 UPM1
#define USART_STOP_BIT_SELECT USBS
#define USART_CHRACTER_SIZE_BIT0  UCSZ0
#define USART_CHARACTER_SIZE_BIT1 UCSZ1


typedef enum{
	_5_BITS, _6_BITS, _7_BITS, _8_BITS, _9_BITS = 7
}data_bits_number;

typedef enum{
	DISABLED, EVEN = 2, ODD
}parity_mode;

typedef enum{
	_1_BITS, _2_BITS
}stop_bits_number;

typedef struct{
	data_bits_number numberOfDataBits;
	parity_mode parityMode;
	stop_bits_number stopBitsNumber;
	uint16 baudRate;
}USART_Config;


/**************************************************************
 *				FUNCTIONS PROTOTYPES
 **************************************************************/
void UART_init(const USART_Config * config_ptr);
void UART_sendByte(uint8);
uint8 UART_receiveByte(void);
void UART_sendString(const uint8 *);
void UART_receiveString(uint8 *);



#endif /* UART_H_ */
