/****************************************************************************************************/
/* Module Name : Universal Asynchronous Receiver/Transmitter ( source file )                        */
/*																			   						*/
/* Author      : Team 1                                                                             */
/*																									*/
/* Purpose     : This module encompasses the implementation of the Universal                        */
/* 				 Asynchronous Receiver/Transmitter (UART) communication protocol. 					*/
/*				 UART is a widely used serial communication standard that enables 					*/
/*				 the exchange of data between devices. The uart.h file serves as 					*/
/*				 the header file for the UART module, containing declarations and					*/
/*				 definitions necessary for configuring, utilizing, 									*/
/*				 and managing UART communication.		                      						*/
/*																									*/
/****************************************************************************************************/



#include "uart.h"







/*********************************************Functions***********************************************/

/*****************************************************************************************************/
/* Function Name :  uart_init									  									 */
/* Inputs        :  UART_ID , const UART_ConfigType                                                  */
/* Outputs       :  void ( No outputs )																 */
/* Reentrancy    :  Reentrant																		 */
/* Synchronous   :  Synchronous 																	 */
/* Description   :	The uart_init function is responsible for configuring a specific UART module 	 */
/*				    identified by UART_ID and setting the communication baud rate to the specified   */
/*				    value (baudrate). UART communication uses two wires for data transmission:       */
/*					one for transmitting data (TX) and one for receiving data (RX). Before data      */
/*					exchange can occur between devices, the UART module needs to be properly         */
/*					initialized.					                                                 */
/*****************************************************************************************************/
void uart_init( UART_ID UART , const UART_ConfigType * Config_Ptr ) 
{
  
  /* baudrate calculating */
  float64 baudrate = 20000000.0 / ( 16.0 * Config_Ptr->baud_rate ) ;
  
 
  switch( UART ) 
  {
/*********************************UART0***************************************/    
  case UART0 :
    
    GPIO_initDigital ( PORTA );
    
    /*   Enable clock for uart0  */ 
    SET_BIT ( SYSCTL_RCGCUART_R , UART0 );
    
    
    /* Config RX and TX pins as uart */
    SET_BIT ( GPIO_PORTA_AFSEL_R , UART0_RX );
    SET_BIT ( GPIO_PORTA_AFSEL_R , UART0_TX );
    
    /* Config RX and TX pins for 8 mA */
    SET_BIT ( GPIO_PORTA_SLR_R , UART0_RX );
    SET_BIT ( GPIO_PORTA_SLR_R , UART0_TX );
    
    
    
    
    /* Config uart for pins     */
    GPIO_PORTA_PCTL_R &=~(0x000000FF);
    SET_BIT ( GPIO_PORTA_PCTL_R , 0 );
    SET_BIT ( GPIO_PORTA_PCTL_R , 4 );
    
    /* Disable uart   */
    CLEAR_BIT ( UART0_CTL_R  , UART_ENABLE_PIN );
    
    UART0_IBRD_R = (int)baudrate;
    UART0_FBRD_R = (int)( ( baudrate - (int)(baudrate) ) * 64 + 0.5) ;

    
    /* data size transfered : Register UART Line Control         */
    /*  to set the size of the data , two bits --> bit 5,  bit 6 */
    /*  8-bit --> set bits 0x3                                   */
    /*  7-bit --> set bits 0x2                                   */
    /*  6-bit --> set bits 0x1                                   */
    /*  5-bit --> set bits 0x0                                   */
    
    UART0_LCRH_R = ( UART0_LCRH_R & 0xFFFFFF9F ) | Config_Ptr->data_size ;
    
        
    
    /* Parity Check :Register UART Line Control                                */                                                       
    /*                                                                         */
    /* Bit 1 : 0--> parity is disabled       , 1 --> parity is enabled         */
    /* Bit 2 : 0 --> Even Parity             , 1 --> Odd parity                */
    /* Bit 7 : 0 --> Stick parity diabled    , 1 --> Stick parity is disabled  */
    UART0_LCRH_R =  (UART0_LCRH_R & 0xFFFFFF79 ) | Config_Ptr->parity ; 
    
    
    /* Stop bits :Register UART Line Control             */
    /*                                                   */
    /* bit 3 = 0 -->  One stop bit , 1 --> two stop bit  */
     UART0_LCRH_R =  (UART0_LCRH_R & 0xFFFFFFFB ) | Config_Ptr->stop_bit ;
     
     
    /*FIFO Enable bit :Register UART Line Control                                    */
    /*          																	 */
    /* bit 4 = 0 --> 1-byte-deep holding registers , 1 --> FIFO buffers are enabled  */
     UART0_LCRH_R =  (UART0_LCRH_R & 0xFFFFFFEF ) | Config_Ptr->fifo_mode ; 
      
    /* set clock to system clock  */
    UART0_CC_R = 0 ;
    
    
    /* Enable uart  */  
    SET_BIT ( UART0_CTL_R  , UART_ENABLE_PIN );
    
    break;
    
/*********************************UART1***************************************/    
  case UART1 :
    
    GPIO_initDigital ( PORTB);
    
    /* Enable clock for uart1   */
    SET_BIT ( SYSCTL_RCGCUART_R , UART1 );
   
    
    /* Config RX and TX pins as uart    */
    SET_BIT ( GPIO_PORTB_AFSEL_R , UART1_RX );
    SET_BIT ( GPIO_PORTB_AFSEL_R , UART1_TX );
    
    /* Config RX and TX pins for 8 mA   */
    SET_BIT ( GPIO_PORTB_SLR_R , UART1_RX );
    SET_BIT ( GPIO_PORTB_SLR_R , UART1_TX );
    
    GPIO_PORTB_PCTL_R &=~(0x000000FF);
    SET_BIT ( GPIO_PORTB_PCTL_R , 0 );
    SET_BIT ( GPIO_PORTB_PCTL_R , 4 );
    
    /*  Disable uart   */
    CLEAR_BIT ( UART1_CTL_R  , UART_ENABLE_PIN );
    
    
    UART1_IBRD_R = (int)baudrate;
    UART1_FBRD_R = (int)( ( baudrate - (int)(baudrate) ) * 64 + 0.5)  ;
    
    /* data size transfered : Register UART Line Control            */
    /*  to set the size of the data , two bits --> bit 5,  bit 6    */
    /*  8-bit --> set bits 0x3                                      */
    /*  7-bit --> set bits 0x2										*/
    /*  6-bit --> set bits 0x1										*/
    /*  5-bit --> set bits 0x0										*/
    UART1_LCRH_R = ( UART1_LCRH_R & 0xFFFFFF9F ) | Config_Ptr->data_size ;
    
        
    
    /* Parity Check :Register UART Line Control                                 */
    /*																			*/ 
    /* Bit 1 : 0--> parity is disabled       , 1 --> parity is enabled		    */
    /* Bit 2 : 0 --> Even Parity             , 1 --> Odd parity					*/
    /* Bit 7 : 0 --> Stick parity diabled    , 1 --> Stick parity is disabled 	*/
    UART1_LCRH_R =  (UART1_LCRH_R & 0xFFFFFF79 ) | Config_Ptr->parity ; 
    
    
    /* Stop bits :Register UART Line Control             */
    /*													 */
    /* bit 3 = 0 -->  One stop bit , 1 --> two stop bit	 */
     UART1_LCRH_R =  (UART1_LCRH_R & 0xFFFFFFFB ) | Config_Ptr->stop_bit ;
     
     
    /* FIFO Enable bit :Register UART Line Control 										*/
    /*																					*/
    /* bit 4 = 0 --> 1-byte-deep holding registers , 1 --> FIFO buffers are enabled     */
    /* 																					*/
     UART1_LCRH_R =  (UART1_LCRH_R & 0xFFFFFFEF ) | Config_Ptr->fifo_mode ; 
    
    
    /*  set clock to system clock  */
    UART1_CC_R = 0 ;
    
    /*  Enable uart  */
    SET_BIT ( UART1_CTL_R  , UART_ENABLE_PIN );
    break;
    
 /*********************************UART2***************************************/   
  case UART2 :
    
    GPIO_initDigital ( PORTD );

    /* Enable clock for uart2  */
    SET_BIT ( SYSCTL_RCGCUART_R , UART2 );
    

    
    /* Config RX and TX pins as uart  */
    SET_BIT ( GPIO_PORTD_AFSEL_R , UART2_RX );
    SET_BIT ( GPIO_PORTD_AFSEL_R , UART2_TX );
    
    /* Config RX and TX pins for 8 mA */
    SET_BIT ( GPIO_PORTD_SLR_R , UART2_RX );
    SET_BIT ( GPIO_PORTD_SLR_R , UART2_TX );
    
    
    GPIO_PORTD_PCTL_R &=~(0xFF000000);
    SET_BIT ( GPIO_PORTD_PCTL_R , 24 );
    SET_BIT ( GPIO_PORTD_PCTL_R , 28 );
    
    /* Disable uart  */
    CLEAR_BIT ( UART2_CTL_R  , UART_ENABLE_PIN );
    
    UART2_IBRD_R = (int)baudrate;
    UART2_FBRD_R = (int)( ( baudrate - (int)(baudrate) ) * 64 + 0.5) ;
    
    /* data size transfered : Register UART Line Control         */
    /*  to set the size of the data , two bits --> bit 5,  bit 6 */
    /*  8-bit --> set bits 0x3                                   */
    /*  7-bit --> set bits 0x2                                   */
    /*  6-bit --> set bits 0x1                                   */
    /*  5-bit --> set bits 0x0                                   */
    UART2_LCRH_R = ( UART2_LCRH_R & 0xFFFFFF9F ) | Config_Ptr->data_size ;
    
        
    
    /* Parity Check :Register UART Line Control									*/
    /* 																			*/
    /* Bit 1 : 0--> parity is disabled       , 1 --> parity is enabled          */
    /* Bit 2 : 0 --> Even Parity             , 1 --> Odd parity					*/
    /* Bit 7 : 0 --> Stick parity diabled    , 1 --> Stick parity is disabled 	*/
    UART2_LCRH_R =  (UART2_LCRH_R & 0xFFFFFF79 ) | Config_Ptr->parity ; 
    
    
    /* Stop bits :Register UART Line Control			   */
    /*												       */
    /* bit 3 = 0 -->  One stop bit , 1 --> two stop bit    */
     UART2_LCRH_R =  (UART2_LCRH_R & 0xFFFFFFFB ) | Config_Ptr->stop_bit ;
     
    /*FIFO Enable bit :Register UART Line Control     								*/
    /*																				*/
    /* bit 4 = 0 --> 1-byte-deep holding registers , 1 --> FIFO buffers are enabled */
     UART2_LCRH_R =  (UART2_LCRH_R & 0xFFFFFFEF ) | Config_Ptr->fifo_mode ; 
    
    /* set clock to system clock   */
    UART2_CC_R = 0 ;
    
    /* Enable uart        */ 
    SET_BIT ( UART2_CTL_R  , UART_ENABLE_PIN );
    break;
    
/*********************************UART3***************************************/   
  case UART3 :
    
    GPIO_initDigital ( PORTC );
    
    /* Enable clock for uart3 */
    SET_BIT ( SYSCTL_RCGCUART_R , UART3 );
    
    
    /* Config RX and TX pins as uart */
    SET_BIT ( GPIO_PORTC_AFSEL_R , UART3_RX );
    SET_BIT ( GPIO_PORTC_AFSEL_R , UART3_TX );
    
    
    /* Config RX and TX pins for 8 mA */
    SET_BIT ( GPIO_PORTC_SLR_R , UART3_RX );
    SET_BIT ( GPIO_PORTC_SLR_R , UART3_TX );
    
    GPIO_PORTC_PCTL_R &=~(0xFF000000);
    SET_BIT ( GPIO_PORTC_PCTL_R , 24 );
    SET_BIT ( GPIO_PORTC_PCTL_R , 28 );
    
    /* Disable uart  */
    CLEAR_BIT ( UART3_CTL_R  , UART_ENABLE_PIN );
    
    UART3_IBRD_R = (int)baudrate;
    UART3_FBRD_R = (int)( ( baudrate - (int)(baudrate) ) * 64 + 0.5) ;
    
    /* data size transfered : Register UART Line Control           */
    /*  to set the size of the data , two bits --> bit 5,  bit 6   */
    /*  8-bit --> set bits 0x3                                     */
    /*  7-bit --> set bits 0x2                                     */
    /*  6-bit --> set bits 0x1                                     */
    /*  5-bit --> set bits 0x0                                     */
    UART3_LCRH_R = ( UART3_LCRH_R & 0xFFFFFF9F ) | Config_Ptr->data_size ;
    
        
    
    /*  Parity Check :Register UART Line Control								*/
    /* 																			*/
    /* Bit 1 : 0--> parity is disabled       , 1 --> parity is enabled			*/
    /* Bit 2 : 0 --> Even Parity             , 1 --> Odd parity					*/
    /* Bit 7 : 0 --> Stick parity diabled    , 1 --> Stick parity is disabled 	*/
    UART3_LCRH_R =  (UART3_LCRH_R & 0xFFFFFF79 ) | Config_Ptr->parity ; 
    
    
	/* Stop bits :Register UART Line Control			*/
    /*													*/
    /* bit 3 = 0 -->  One stop bit , 1 --> two stop bit */
     UART3_LCRH_R =  (UART3_LCRH_R & 0xFFFFFFFB ) | Config_Ptr->stop_bit ;
     
    /*FIFO Enable bit :Register UART Line Control                                   */
    /* 																				*/
    /* bit 4 = 0 --> 1-byte-deep holding registers , 1 --> FIFO buffers are enabled	*/
     UART3_LCRH_R =  (UART3_LCRH_R & 0xFFFFFFEF ) | Config_Ptr->fifo_mode ; 
    
    /*   set clock to system clock   */ 
    UART3_CC_R = 0 ;
    
    /* Enable uart  */
    SET_BIT ( UART3_CTL_R  , UART_ENABLE_PIN );
    break;
    
/*********************************UART4***************************************/   
  case UART4:
    
    GPIO_initDigital ( PORTC );
    
    /* Enable clock for uart4 */
    SET_BIT ( SYSCTL_RCGCUART_R , UART4 );
    
    /* Config RX and TX pins as uart */
    SET_BIT ( GPIO_PORTC_AFSEL_R , UART4_RX );
    SET_BIT ( GPIO_PORTC_AFSEL_R , UART4_TX );
    
    /* Config RX and TX pins for 8 mA */
    SET_BIT ( GPIO_PORTC_SLR_R , UART4_RX );
    SET_BIT ( GPIO_PORTC_SLR_R , UART4_TX );
    
    GPIO_PORTC_PCTL_R &=~(0x00FF0000);
    SET_BIT ( GPIO_PORTC_PCTL_R , 16 );
    SET_BIT ( GPIO_PORTC_PCTL_R , 20 );
    
    /* Disable uart */
    CLEAR_BIT ( UART4_CTL_R  , UART_ENABLE_PIN );
    
    UART4_IBRD_R = (int)baudrate;
    UART4_FBRD_R = (int)( ( baudrate - (int)(baudrate) ) * 64 + 0.5) ;
    
    
    
    /* data size transfered : Register UART Line Control					*/
    /*  to set the size of the data , two bits --> bit 5,  bit 6			*/
    /*  8-bit --> set bits 0x3												*/
    /*  7-bit --> set bits 0x2												*/
    /*  6-bit --> set bits 0x1												*/
    /*  5-bit --> set bits 0x0 												*/
    UART4_LCRH_R = ( UART4_LCRH_R & 0xFFFFFF9F ) | Config_Ptr->data_size ;
    
        
    
    /*Parity Check :Register UART Line Control								   */
    /*  																	   */
    /* Bit 1 : 0--> parity is disabled       , 1 --> parity is enabled		   */
    /* Bit 2 : 0 --> Even Parity             , 1 --> Odd parity				   */
    /* Bit 7 : 0 --> Stick parity diabled    , 1 --> Stick parity is disabled  */
    UART4_LCRH_R =  (UART4_LCRH_R & 0xFFFFFF79 ) | Config_Ptr->parity ; 
    
    
    /* Stop bits :Register UART Line Control                */
    /*	                                                    */
    /* bit 3 = 0 -->  One stop bit , 1 --> two stop bit     */
     UART4_LCRH_R =  (UART4_LCRH_R & 0xFFFFFFFB ) | Config_Ptr->stop_bit ;
     
     
    /*FIFO Enable bit :Register UART Line Control									 */
    /*																				 */
    /* bit 4 = 0 --> 1-byte-deep holding registers , 1 --> FIFO buffers are enabled  */
     UART4_LCRH_R =  (UART4_LCRH_R & 0xFFFFFFEF ) | Config_Ptr->fifo_mode ; 
    
    /*   set clock to system clock  */
    UART4_CC_R = 0 ;
    
    /*   Enable uart  */
    SET_BIT ( UART4_CTL_R  , UART_ENABLE_PIN );
    break;
    
/*********************************UART5***************************************/  
  case UART5 :
    
    GPIO_initDigital ( PORTE );
    
    /* Enable clock for uart5    */
    SET_BIT ( SYSCTL_RCGCUART_R , UART5 );
    
    /* Config RX and TX pins as uart   */
    SET_BIT ( GPIO_PORTE_AFSEL_R , UART5_RX );
    SET_BIT ( GPIO_PORTE_AFSEL_R , UART5_TX );
    
    /* Config RX and TX pins for 8 mA  */
    SET_BIT ( GPIO_PORTE_SLR_R , UART5_RX );
    SET_BIT ( GPIO_PORTE_SLR_R , UART5_TX );
    
    GPIO_PORTE_PCTL_R &=~(0x00FF0000);
    SET_BIT ( GPIO_PORTE_PCTL_R , 16 );
    SET_BIT ( GPIO_PORTE_PCTL_R , 20 );
    
    /* Disable uart    */
    CLEAR_BIT ( UART5_CTL_R  , UART_ENABLE_PIN );
    
    UART5_IBRD_R = (int)baudrate;
    UART5_FBRD_R = (int)( ( baudrate - (int)(baudrate) ) * 64 + 0.5) ;
    
    /* data size transfered : Register UART Line Control			*/
    /*  to set the size of the data , two bits --> bit 5,  bit 6	*/
    /*  8-bit --> set bits 0x3										*/
    /*  7-bit --> set bits 0x2										*/
    /*  6-bit --> set bits 0x1										*/
    /*  5-bit --> set bits 0x0										*/
    UART5_LCRH_R = ( UART5_LCRH_R & 0xFFFFFF9F ) | Config_Ptr->data_size ;
    
        
    
    /*Parity Check :Register UART Line Control									*/
    /* 																			*/
    /* Bit 1 : 0--> parity is disabled       , 1 --> parity is enabled			*/
    /* Bit 2 : 0 --> Even Parity             , 1 --> Odd parity					*/
    /* Bit 7 : 0 --> Stick parity diabled    , 1 --> Stick parity is disabled 	*/
    UART5_LCRH_R =  (UART5_LCRH_R & 0xFFFFFF79 ) | Config_Ptr->parity ; 		
    
    
    /* Stop bits :Register UART Line Control				*/
    /*														*/
    /* bit 3 = 0 -->  One stop bit , 1 --> two stop bit 	*/
     UART5_LCRH_R =  (UART5_LCRH_R & 0xFFFFFFFB ) | Config_Ptr->stop_bit ;
     
    /*FIFO Enable bit :Register UART Line Control										*/
    /*																					*/
    /* bit 4 = 0 --> 1-byte-deep holding registers , 1 --> FIFO buffers are enabled		*/
     UART5_LCRH_R =  (UART5_LCRH_R & 0xFFFFFFEF ) | Config_Ptr->fifo_mode ; 
    
    /* set clock to system clock    */
    UART5_CC_R = 0 ;
    
    /*  Enable uart  */ 
    SET_BIT ( UART5_CTL_R  , UART_ENABLE_PIN );
    break;
    
 /*********************************UART6***************************************/   
  case UART6 :
    
    GPIO_initDigital ( PORTD );
    
    /*      Enable clock for uart6  */
    SET_BIT ( SYSCTL_RCGCUART_R , UART6 );
    

    
    /*    Config RX and TX pins as uart   */
    SET_BIT ( GPIO_PORTD_AFSEL_R , UART6_RX );
    SET_BIT ( GPIO_PORTD_AFSEL_R , UART6_TX );
    
    /* Config RX and TX pins for 8 mA  */
    SET_BIT ( GPIO_PORTD_SLR_R , UART6_RX );
    SET_BIT ( GPIO_PORTD_SLR_R , UART6_TX );
    

    GPIO_PORTD_PCTL_R &=~(0x00FF0000);
    SET_BIT ( GPIO_PORTD_PCTL_R , 16 );
    SET_BIT ( GPIO_PORTD_PCTL_R , 20 );
    
    /*  Disable uart  */ 
    CLEAR_BIT ( UART6_CTL_R  , UART_ENABLE_PIN );
    
    UART6_IBRD_R = (int)baudrate;
    UART6_FBRD_R = (int)( ( baudrate - (int)(baudrate) ) * 64 + 0.5) ;
    
    /* data size transfered : Register UART Line Control				*/
    /*  to set the size of the data , two bits --> bit 5,  bit 6		*/
    /*  8-bit --> set bits 0x3											*/
    /*  7-bit --> set bits 0x2											*/
    /*  6-bit --> set bits 0x1											*/
    /*  5-bit --> set bits 0x0  										*/
    UART6_LCRH_R = ( UART6_LCRH_R & 0xFFFFFF9F ) | Config_Ptr->data_size ;
    
        
    
    /*Parity Check :Register UART Line Control								  */
    /* 																		  */
    /* Bit 1 : 0--> parity is disabled       , 1 --> parity is enabled		  */
    /* Bit 2 : 0 --> Even Parity             , 1 --> Odd parity				  */
    /* Bit 7 : 0 --> Stick parity diabled    , 1 --> Stick parity is disabled */
    UART6_LCRH_R =  (UART6_LCRH_R & 0xFFFFFF79 ) | Config_Ptr->parity ; 
    
    
    /*Stop bits :Register UART Line Control				 */
    /*													 */
    /* bit 3 = 0 -->  One stop bit , 1 --> two stop bit  */
     UART6_LCRH_R =  (UART6_LCRH_R & 0xFFFFFFFB ) | Config_Ptr->stop_bit ;
     
    /*FIFO Enable bit :Register UART Line Control									*/
    /*																				*/
    /* bit 4 = 0 --> 1-byte-deep holding registers , 1 --> FIFO buffers are enabled */
     UART6_LCRH_R =  (UART6_LCRH_R & 0xFFFFFFEF ) | Config_Ptr->fifo_mode ; 
    
    /* set clock to system clock	*/
    UART6_CC_R = 0 ;
    
    /*  Enable uart    */ 
    SET_BIT ( UART6_CTL_R  , UART_ENABLE_PIN );
    break;
    
/*********************************UART7***************************************/   
  case UART7 :
    
    GPIO_initDigital ( PORTE );
    
    /*  Enable clock for uart7  */
    SET_BIT ( SYSCTL_RCGCUART_R , UART7 );
    
    
    /* Config RX and TX pins as uart  */ 
    SET_BIT ( GPIO_PORTE_AFSEL_R , UART7_RX );
    SET_BIT ( GPIO_PORTE_AFSEL_R , UART7_TX );
    
    
    /* Config RX and TX pins for 8 mA */
    SET_BIT ( GPIO_PORTE_SLR_R , UART7_RX );
    SET_BIT ( GPIO_PORTE_SLR_R , UART7_TX );
    
    GPIO_PORTE_PCTL_R  &=~(0x000000FF);
    SET_BIT ( GPIO_PORTE_PCTL_R , 0 );
    SET_BIT ( GPIO_PORTE_PCTL_R , 4 );
    
    /* Disable uart  */ 
    CLEAR_BIT ( UART7_CTL_R  , UART_ENABLE_PIN );
    
    UART7_IBRD_R = (int)baudrate;
    UART7_FBRD_R = (int)( ( baudrate - (int)(baudrate) ) * 64 + 0.5) ;
    
    /* data size transfered : Register UART Line Control			*/
    /*  to set the size of the data , two bits --> bit 5,  bit 6	*/
    /*  8-bit --> set bits 0x3										*/
    /*  7-bit --> set bits 0x2										*/
    /*  6-bit --> set bits 0x1										*/
    /*  5-bit --> set bits 0x0 										*/
    UART7_LCRH_R = ( UART7_LCRH_R & 0xFFFFFF9F ) | Config_Ptr->data_size ;
    
        
    
    /*Parity Check :Register UART Line Control								   */
    /* 																		   */
    /* Bit 1 : 0--> parity is disabled       , 1 --> parity is enabled		   */
    /* Bit 2 : 0 --> Even Parity             , 1 --> Odd parity				   */
    /* Bit 7 : 0 --> Stick parity diabled    , 1 --> Stick parity is disabled  */
    UART7_LCRH_R =  (UART7_LCRH_R & 0xFFFFFF79 ) | Config_Ptr->parity ; 
    
    
    /* Stop bits :Register UART Line Control			*/
    /*													*/
    /* bit 3 = 0 -->  One stop bit , 1 --> two stop bit */
     UART7_LCRH_R =  (UART7_LCRH_R & 0xFFFFFFFB ) | Config_Ptr->stop_bit ;
    
    /*FIFO Enable bit :Register UART Line Control									*/
    /*																				*/
    /* bit 4 = 0 --> 1-byte-deep holding registers , 1 --> FIFO buffers are enabled */
     UART7_LCRH_R =  (UART7_LCRH_R & 0xFFFFFFEF ) | Config_Ptr->fifo_mode ; 
    
    /* set clock to system clock */
    UART7_CC_R = 0 ;

    /* Enable uart  */ 
    SET_BIT ( UART7_CTL_R  , UART_ENABLE_PIN );
    
    break;
  }
  
 
  
}





/*****************************************************************************************************/
/* Function Name :  uart_transmitByte																 */
/* Inputs        :  UART_ID  , uint8 ( data )                                                        */
/* Outputs       :  uint8 ( Data )							   			         					 */
/* Reentrancy    :  Reentrant																		 */
/* Synchronous   :  Synchronous																		 */
/* Description   :	The uart_transmit function facilitates the transmission of a single byte of data */
/*					over a UART communication channel. It is essential for 							 */
/*					sending information from one device to another,									 */
/*					often used in microcontroller-based applications,								 */
/*					embedded systems, and various communication protocols.	   						 */	                                        
/*****************************************************************************************************/
void uart_transmitByte ( UART_ID UART , uint8 data ) 
{
  switch( UART )
  {
  case UART0:
  	/*  Open Transmit pin  */
    SET_BIT ( UART0_CTL_R  , UART_TX_EN );
    /* Send data */
    UART0_DR_R = data;
    break;
  case UART1:
  	/*  Open Transmit pin  */
    SET_BIT ( UART1_CTL_R  , UART_TX_EN );
    /* Send data */
    UART1_DR_R = data;
    break;
  case UART2:
  	/*  Open Transmit pin  */
    SET_BIT ( UART2_CTL_R  , UART_TX_EN );
    /* Send data */
    UART2_DR_R = data;
    break;
  case UART3:
  	/*  Open Transmit pin  */
    SET_BIT ( UART3_CTL_R  , UART_TX_EN );
    /* Send data */
    UART3_DR_R = data;
    break;
  case UART4:
  	/*  Open Transmit pin  */
    SET_BIT ( UART4_CTL_R  , UART_TX_EN );
    /* Send data */
    UART4_DR_R = data;
    break;
  case UART5:
  	/*  Open Transmit pin  */
    SET_BIT ( UART5_CTL_R  , UART_TX_EN );
    /* Send data */
    UART5_DR_R = data ; 
    break;
  case UART6:
  	/*  Open Transmit pin  */
    SET_BIT ( UART6_CTL_R  , UART_TX_EN );
    /* Send data */
    UART6_DR_R = data;
    break;
  case UART7:
  	/*  Open Transmit pin  */
    SET_BIT ( UART7_CTL_R  , UART_TX_EN );
    /* Send data */
    UART7_DR_R = data;
    break;
  }
  
}



/*****************************************************************************************************/
/* Function Name :  uart_receiveByte																 */
/* Inputs        :  UART_ID                                                                          */
/* Outputs       :  uint8 ( Data )							   			         					 */
/* Reentrancy    :  Reentrant																		 */
/* Synchronous   :  Synchronous																		 */
/* Description   :	The uart_receive function is designed to receive and return a single byte of     */
/*					data from a specific UART communication channel identified by the UART_ID        */
/*					parameter. UART (Universal Asynchronous Receiver/Transmitter) is a hardware      */
/*					communication protocol used for serial data transmission between devices.	     */				                                       
/*****************************************************************************************************/
uint8 uart_receiveByte (  UART_ID UART ) 
{
  uint8 data = 0 ;
  switch( UART )
  {
  case UART0:
  	/* Open Receive Pin */
    SET_BIT ( UART0_CTL_R  , UART_RX_EN );
    /* polling for receive */
    while (UART0_FR_R & (1 << 4)) {}
    data = (UART0_DR_R & 0xFF) ; 
    break;
  case UART1:
  	/* Open Receive Pin */
    SET_BIT ( UART1_CTL_R  , UART_RX_EN );
    /* polling for receive */
    //while (UART1_FR_R & (1 << 4)) {}
    data = (UART1_DR_R & 0xFF) ; 
    break;
  case UART2:
  	/* Open Receive Pin */
    SET_BIT ( UART2_CTL_R  , UART_RX_EN );
    /* polling for receive */
    //while (UART2_FR_R & (1 << 4)) {}
    data = (UART2_DR_R & 0xFF) ; 
    break;
  case UART3:
  	/* Open Receive Pin */
    SET_BIT ( UART3_CTL_R  , UART_RX_EN );
    /* polling for receive */
   // while (UART3_FR_R & (1 << 4)) {}
    data = (UART3_DR_R & 0xFF) ; 
    break;
  case UART4:
  	/* Open Receive Pin */
    SET_BIT ( UART4_CTL_R  , UART_RX_EN );
    /* polling for receive */
  //  while (UART4_FR_R & (1 << 4)) {}
    data = (UART4_DR_R & 0xFF) ; 
    break;
  case UART5:
  	/* Open Receive Pin */
    SET_BIT ( UART5_CTL_R  , UART_RX_EN );
    /* polling for receive */
  //  while (UART5_FR_R & (1 << 4)) {}
    data = (UART5_DR_R & 0xFF) ; 
    break;
  case UART6:
  	/* Open Receive Pin */
    SET_BIT ( UART6_CTL_R  , UART_RX_EN );
    /* polling for receive */
    while (UART6_FR_R & (1 << 4)) {}
    data = (UART6_DR_R & 0xFF) ; 
    break;
  case UART7:
  	/* Open Receive Pin */
    SET_BIT ( UART7_CTL_R  , UART_RX_EN );
    /* polling for receive */
   // while (UART7_FR_R & (1 << 4)) {}
    data = (UART7_DR_R & 0xFF) ; 
    break;    
  }
    return data ; 

}




/*****************************************************************************************************/
/* Function Name :  uart_transmitString																 */
/* Inputs        :  UART_ID  , const uint8 *  ( Str )                                                */
/* Outputs       :  void                  					   			         					 */
/* Reentrancy    :  Reentrant																		 */
/* Synchronous   :  Synchronous																		 */
/* Description   :	This function is designed to transmit a string of characters via a UART          */
/* 					interface. UART is a widely used communication protocol that enables             */
/* 					serial communication between devices. The function takes two                     */
/* 					parameters: UART_ID, which likely represents the specific                        */
/* 					UART module or instance, and Str, a pointer to an array of                       */
/* 					characters (string) that is intended to be transmitted.   						 */
/*****************************************************************************************************/
void uart_transmitString ( UART_ID UART , const uint8 *Str ) 
{
    uint8 i = 0;

    /* Send the whole string */
    while(Str[i] != '\0')
    {
      uart_transmitByte( UART , Str[i]);
      i++;
    }
}




/*
*
*  Discrpition : This function is designed to receive a string of characters
*  from a UART (Universal Asynchronous Receiver-Transmitter) interface. UART is
*  a common communication protocol used for serial communication between devices.
*  The function takes two parameters: UART_ID, which likely represents the specific
*  UART module or instance, and Str, a pointer to an array of characters where the
*  received string will be stored
*
*/


/*****************************************************************************************************/
/* Function Name :  uart_receiveString																 */
/* Inputs        :  UART_ID  , uint8 *  ( Str )                                                      */
/* Outputs       :  void                  					   			         					 */
/* Reentrancy    :  Reentrant																		 */
/* Synchronous   :  Synchronous																		 */
/* Description   :	This function is designed to receive a string of characters                      */
/*  				from a UART (Universal Asynchronous Receiver-Transmitter) interface. UART is     */
/*  				a common communication protocol used for serial communication between devices.   */
/*  				The function takes two parameters: UART_ID, which likely represents the specific */
/* 					UART module or instance, and Str, a pointer to an array of characters where the  */ 
/*  				received string will be stored			                            			 */				                                              
/*****************************************************************************************************/
void uart_receiveString( UART_ID UART , uint8 *Str) 
{
  uint8 i = 0;

  /* Receive the first byte */
  Str[i] = uart_receiveByte( UART );

  /* Receive the whole string until the '#' */
  while(Str[i] != '#')
  {
    i++;
    Str[i] = uart_receiveByte( UART );
  }
 
  /* After receiving the whole string plus the '#', replace the '#' with '\0' */
  Str[i] = '\0';
}




















