/*
 * uart.c
 *
 *  Created on: Mar 28, 2021
 *      Author: Youssef El-Nemaky
 */
#include "uart.h"

/*******************************************************************************************************************
 * Function : UART_init
 *
 * Description : used to initialize UART
 *
 * Parameters : config_ptr : a pointer to USART_Config structure that contains the parameters to initialize UART
 *
 * Return Value : None (void)
 *******************************************************************************************************************/
void UART_init(const USART_Config * config_ptr){
	/* Receiver Enable + Transmitter Enable*/
	USART_CONTROL_AND_STATUS_REGISTER_B = (1<<USART_RECEIVER_ENABLE_BIT) | (1<<USART_TRANSMITTER_ENABLE_BIT);

	/* configure the number of data bits*/
	USART_CONTROL_AND_STATUS_REGISTER_B = (USART_CONTROL_AND_STATUS_REGISTER_B & 0xfb) | (config_ptr->numberOfDataBits & 0x04);

	/* set URSEL as we will be writing in UCSRC register */
	SET_BIT(USART_CONTROL_AND_STATUS_REGISTER_C, USART_REGISER_SELECT_BIT);

	/* configure UMLSEC to be either 0 or 1 depending on the static configuration of S_MODE */
	USART_CONTROL_AND_STATUS_REGISTER_C = (USART_CONTROL_AND_STATUS_REGISTER_C & 0xbf) | (S_MODE<<6);

	/* configure UPM bits with the value of config_ptr->parityMode */
	USART_CONTROL_AND_STATUS_REGISTER_C = (USART_CONTROL_AND_STATUS_REGISTER_C & 0xcf) | ((config_ptr->parityMode)<<4);

	/* configure the number of stop bits */
	USART_CONTROL_AND_STATUS_REGISTER_C = (USART_CONTROL_AND_STATUS_REGISTER_C & 0xf7) | ((config_ptr->stopBitsNumber)<<3);

	/* configure the number of data bits */
	USART_CONTROL_AND_STATUS_REGISTER_C = (USART_CONTROL_AND_STATUS_REGISTER_C & 0xf9) | (((config_ptr->numberOfDataBits) & 0x03)<<1);

	/*configure double speed mode bit */
	USART_CONTROL_AND_STATUS_REGISTER_A = (USART_CONTROL_AND_STATUS_REGISTER_A & 0xfd) | (DOUBLE_SPEED_MODE<<USART_DOUBLE_TRANSMISSION_SPEED_BIT);


	/* setting baud rate settings depending on the static configuration of the required baud rate */
	UBRRL = UBRR_VALUE;
	UBRRH = (UBRR_VALUE>>8);
} /* end of UART_init() function */



/*******************************************************************************************************************
 * Function : UART_sendByte
 *
 * Description : used to send a byte through UART communication protocol
 *
 * Parameters : data : the byte that will be sent through UART protocol
 *
 * Return Value : None (void)
 *******************************************************************************************************************/
void UART_sendByte(uint8 data)
{
	while (BIT_IS_CLEAR(UCSRA ,UDRE)); /* polling until the flag is set to 1 which means that the buffer is empty and ready to transmit a new byte */

	/* put the required data in the UDR register and it also clear the UDRE flag as
	 * the UDR register is not empty now */
	UDR = data;
} /* end of UART_sendByte() function */


/*******************************************************************************************************************
 * Function : UART_recieveByte
 *
 * Description : used to receive a byte through UART communication protocol
 *
 * Parameters : None (void)
 *
 * Return Value : uint8 : the byte received in UART buffer.
 *******************************************************************************************************************/
uint8 UART_receiveByte(void)
{
	while (BIT_IS_CLEAR(UCSRA ,RXC)); /* polling until the flag is set to 1 which means that reception of the byte has been completed */
	/* Read the received data from the Rx buffer (UDR) and the RXC flag
	   will be cleared after read this data */
	return UDR;
} /* end of UART_receiveByte() function */


/*******************************************************************************************************************
 * Function : UART_sendString
 *
 * Description : used to send a string through UART communication protocol
 *
 * Parameters : str : a pointer to const uint8 (address of the first element of the string) that you want to send
 *
 * Return Value : None (void)
 *******************************************************************************************************************/
void UART_sendString(const uint8 *str)
{
	uint8 i = 0; /* string counter */

	/* keep looping on the string until you reach the end (\0) */
	while (str[i] != '\0')
	{
		/*send each element of the string (byte after byte)*/
		UART_sendByte(str[i]);
		i++;
	}
} /* end of UART_sendString() function */


/*******************************************************************************************************************
 * Function : UART_receiveString
 *
 * Description : used to receive a string through UART communication protocol
 *
 * Parameters : str : a pointer to uint8 (address of the first element of the string to copy the data into)
 *
 * Return Value : None (void)
 *******************************************************************************************************************/
void UART_receiveString(uint8 *str)
{
	uint8 i = 0; /* string counter */

	str[i] = UART_receiveByte(); /* receive the first byte through UART and put it into the array */

	/* keep receiving byte after byte until the end key which in this case is '#'*/
	while (str[i] != '#')
	{
		i++;
		str[i] = UART_receiveByte();
	}
	/* After the loop, the end key must has been received and now we should put the traditional end key of any string which is '\0' */
	str[i] = '\0';
} /* end of UART_receiveString() function*/
