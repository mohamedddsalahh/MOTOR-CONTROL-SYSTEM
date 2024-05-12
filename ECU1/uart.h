/****************************************************************************************************/
/* Module Name : Universal Asynchronous Receiver/Transmitter ( header file )                        */                                                                            
/*	 											    */
/* Author      : Team 1                                                                             */
/*												    */
/* Purpose     : This module encompasses the implementation of the Universal Asynchronous 	    */
/*		 Receiver/Transmitter (UART) communication protocol. UART is a  widely 		    */
/*		 used serial communication standard that enables the exchange of data between       */
/*		 devices. The uart.h file serves as the header file for the UART module, 	    */
/*		 containing declarations and definitions necessary for configuring, utilizing, 	    */
/*               and managing UART communication.						    */
/*				  								    */
/****************************************************************************************************/



#ifndef UART_H_
#define UART_H_

/****************************************************************************************************/

#include "tm4c123gh6pm.h"
#include "uart.h"
#include "types.h"
#include "gpio.h"

/********************************port and pin data define********************************************/


#define UART0_PORT     PORTA
#define UART1_PORT     PORTB 
#define UART2_PORT     PORTD
#define UART3_PORT     PORTC
#define UART4_PORT     PORTC
#define UART5_PORT     PORTE
#define UART6_PORT     PORTD
#define UART7_PORT     PORTE


#define UART0_RX       PIN0 
#define UART0_TX       PIN1

#define UART1_RX       PIN0
#define UART1_TX       PIN1


#define UART2_RX       PIN6
#define UART2_TX       PIN7


#define UART3_RX       PIN6
#define UART3_TX       PIN7


#define UART4_RX       PIN4
#define UART4_TX       PIN5


#define UART5_RX       PIN4
#define UART5_TX       PIN5


#define UART6_RX       PIN4
#define UART6_TX       PIN5


#define UART7_RX       PIN0
#define UART7_TX       PIN1


#define UART_ENABLE_PIN 0 
#define UART_RX_EN      8
#define UART_TX_EN      9


/******************************** Type definition***************************************************/



/****************************UART_ID**************************/
typedef enum {
  UART0 , UART1 , UART2 , UART3 , UART4 , UART5 , UART6 , UART7 
}UART_ID;

/**************************UART_BitData***********************/
typedef enum
{
  BIT_5 = 0 , BIT_6 = 0x20 , BIT_7 = 0x40  , BIT_8  = 0x60
}UART_BitData;


/**************************UART_Parity***********************/
typedef enum
{
  NO_PARITY = 0 , ODD_PARITY = 0x82 , EVEN_PARITY = 0x86
}UART_Parity;

/**************************UART_StopBit***********************/
typedef enum
{
  ONE_BIT , TWO_BIT = 0x4
}UART_StopBit;

/**************************UART_FIFO_EN***********************/
typedef enum 
{
  DISABLE_FIFO , ENABLE_FIFO = 0X10 
}UART_FIFO_EN ; 


/**************************Configration Structure***********************/
typedef struct{
  
  UART_BitData data_size  ;
  UART_Parity  parity     ;
  UART_StopBit stop_bit   ;
  UART_FIFO_EN fifo_mode  ;
  uint64       baud_rate  ;
}UART_ConfigType;



/****************************************************************************/


/*********************************************Functions***********************************************/

/*****************************************************************************************************/
/* Function Name :  uart_init									     */
/* Inputs        :  UART_ID , const UART_ConfigType                                                  */
/* Outputs       :  void ( No outputs )								     */
/* Reentrancy    :  Reentrant									     */
/* Synchronous   :  Synchronous			     						     */
/* Description   :  The uart_init function is responsible for configuring a specific UART module     */
/*	            identified by UART_ID and setting the communication baud rate to the specified   */
/*		    value (baudrate). UART communication uses two wires for data transmission:       */
/*		    one for transmitting data (TX) and one for receiving data (RX). Before data      */
/*		    exchange can occur between devices, the UART module needs to be properly         */
/*		    initialized.					                             */
/*****************************************************************************************************/
void uart_init( UART_ID UART , const UART_ConfigType * Config_Ptr ); 


/*****************************************************************************************************/
/* Function Name :  uart_transmitByte								     */
/* Inputs        :  UART_ID  , uint8 ( data )                                                        */
/* Outputs       :  void         							   	     */
/* Reentrancy    :  Reentrant			   						     */
/* Synchronous   :  Synchronous									     */
/* Description   :  The uart_transmit function facilitates the transmission of a single byte of data */
/*		    over a UART communication channel. It is essential forsending information from   */
/*		    one device to another,often used in microcontroller-based applications	     */
/*		    embedded systems, and various communication protocols.	                     */
/*****************************************************************************************************/
void uart_transmitByte ( UART_ID UART ,uint8 data );




/*****************************************************************************************************/
/* Function Name :  uart_receiveByte								     */
/* Inputs        :  UART_ID                                                                          */
/* Outputs       :  uint8 ( Data )							   	     */
/* Reentrancy    :  Reentrant									     */
/* Synchronous   :  Synchronous									     */
/* Description   :  The uart_receive function is designed to receive and return a single byte of     */
/*		    data from a specific UART communication channel identified by the UART_ID        */
/*		    parameter. UART (Universal Asynchronous Receiver/Transmitter) is a hardware      */
/*		    communication protocol used for serial data transmission between devices.	     */
/*****************************************************************************************************/
uint8 uart_receiveByte (  UART_ID UART );


/*****************************************************************************************************/
/* Function Name :  uart_transmitString								     */
/* Inputs        :  UART_ID  , const uint8 *  ( Str )                                                */
/* Outputs       :  void                  					   		     */
/* Reentrancy    :  Reentrant									     */
/* Synchronous   :  Synchronous									     */
/* Description   :	This function is designed to transmit a string of characters via a UART      */
/* 			interface. UART is a widely used communication protocol that enables         */
/* 			serial communication between devices. The function takes two                 */
/* 			parameters: UART_ID, which likely represents the specific                    */
/* 			UART module or instance, and Str, a pointer to an array of                   */
/* 			characters (string) that is intended to be transmitted.   		     */
/*****************************************************************************************************/
void uart_transmitString ( UART_ID UART , const uint8 *Str ) ;




/*****************************************************************************************************/
/* Function Name :  uart_receiveString         							     */
/* Inputs        :  UART_ID  , uint8 *  ( Str )                                                      */
/* Outputs       :  void                  					   	       	     */
/* Reentrancy    :  Reentrant									     */
/* Synchronous   :  Synchronous									     */
/* Description   :  This function is designed to receive a string of characters                      */
/*  		    from a UART (Universal Asynchronous Receiver-Transmitter) interface. UART is     */
/*  		    a common communication protocol used for serial communication between devices.   */
/*  	            The function takes two parameters: UART_ID, which likely represents the specific */
/* 	            UART module or instance, and Str, a pointer to an array of characters where the  */ 
/*  	            received string will be stored			                             */				                                              
/*****************************************************************************************************/
void uart_receiveString( UART_ID UART , uint8 *Str) ; 



#endif 
