/****************************************************************************************************/
/* Module Name : General-Purpose Input/Outputs  ( source file )                                     */
/*																									*/
/* Author      : Team 1                                                                             */
/*																									*/
/* Purpose     : This header file defines function prototypes for configuring and controlling 		*/
/*               GPIO pins on a microcontroller. Users can include this file in their projects		*/
/*				 to interact with GPIO pins based on their specific requirements. It declares		*/
/*				 functions for initializing GPIO pins, setting pin modes (input/output), reading	*/	
/*				 and writing pin values, and enabling/ disabling internal pull-up or pull-down 		*/
/*				 resistors.					 														*/
/*																									*/
/****************************************************************************************************/
#ifndef GPIO_H_
#define GPIO_H_


#include "types.h"
#include "tm4c123gh6pm.h"


#define LOGIC_HIGH       1

#define LOGIC_LOW        0

#define UNLOCK           0x4C4F434B 

#define ALLOW_MODIFY     0xFF 

#define PORT_DIGITAL     0xFF

/*************************Bit Operation************************************/

#define SET_BIT( REG , BIT ) ( REG |=  ( 1 << BIT ) )
#define CLEAR_BIT( REG , BIT ) ( REG &= ~( 1 << BIT ) )
#define GET_BIT( REG , BIT ) ( ( REG & ( 1 << BIT ) ) >> BIT ) 
#define TOGGLE_BIT( REG , BIT ) ( REG ^= ( 1 << BIT ) )

/****************************************************************************/

/**************************port and pin data define*********************/

typedef enum {
  PORTA , PORTB , PORTC , PORTD , PORTE , PORTF
}PORT_ID;

typedef enum {
  PIN0 , PIN1 , PIN2 ,  PIN3 , PIN4 , PIN5 , PIN6 , PIN7 
}PIN_ID;


typedef enum {
  INPUT_DIR , OUTPUT_DIR = 0xFF 
}PORT_MODE ;

typedef enum {
  INPUT_PUR , OUTPUT_PUR = 0xFF 
}PORT_PULL_UP ;


typedef enum {
  INPUT_PIN_DIR , OUTPUT_PIN_DIR = 1
}PIN_MODE ;


typedef enum {
  NO_USE , USE_PULL = 1
}PIN_PULL_UP ;

/****************************************************************************/


/*********************************************Functions***********************************************/

/*****************************************************************************************************/
/* Function Name :  GPIO_initDigital																 */
/* Inputs        :  PORT_ID ( PORT  )                                                                */
/* Outputs       :  void ( No outputs )																 */
/* Reentrancy    :  Reentrant																		 */
/* Synchronous   :  Synchronous  																	 */
/* Description   :	This function is used to initialize the port. it takes the requried port and 	 */
/*					open its clock and unlock the port. it also open the digital i/o for the port.   */
/*					and open the modification on the direction , pullup and the alternative function */
/*					options					 														 */
/*****************************************************************************************************/
void GPIO_initDigital ( PORT_ID PORT );


/*****************************************************************************************************/
/* Function Name :  GPIO_setupPortDirection															 */
/* Inputs        :  PORT_ID ( PORT  )  , PORT_MODE (dir ) , PORT_PULL_UP ( useInternalPullUp )       */
/* Outputs       :  void ( No outputs )																 */
/* Reentrancy    :  Reentrant																		 */
/* Synchronous   :  Synchronous																     	 */
/* Description   :	This function is used to define the port direction and wether to use the internal*/ 
/*					pull up resistance or not				 										 */
/*****************************************************************************************************/
void GPIO_setupPortDirection ( PORT_ID PORT  , PORT_MODE dir , PORT_PULL_UP useInternalPullUp  );


/****************************************************************************************************************/
/* Function Name :  GPIO_setupPinDirection															 			*/
/* Inputs        :  PORT_ID ( PORT  )  , PIN_ID PIN,PORT_MODE (dir ) , PORT_PULL_UP ( useInternalPullUp )       */
/* Outputs       :  void ( No outputs )																            */
/* Reentrancy    :  Reentrant																		            */
/* Synchronous   :  Synchronous																     	            */
/* Description   :	This function is used to define the PIN direction and wether to use the internal pull up    */
/*					resistance or not			 										                        */
/****************************************************************************************************************/
void GPIO_setupPinDirection ( PORT_ID PORT  , PIN_ID PIN , PIN_MODE dir , PIN_PULL_UP useInternalPullUp  );


/****************************************************************************************************************/
/* Function Name :  GPIO_writeBit															 	        		*/
/* Inputs        :  PORT_ID ( PORT  )  , PIN_ID PIN, uint8 (val	)										        */
/* Outputs       :  void ( No outputs )																            */
/* Reentrancy    :  Reentrant																		            */
/* Synchronous   :  Synchronous																     	            */
/* Description   :	This function is used to set a high or low for a specific bit in specific port				*/	
/****************************************************************************************************************/
void GPIO_writeBit ( PORT_ID PORT  ,  PIN_ID PIN , uint8 val ) ;


/****************************************************************************************************************/
/* Function Name :  GPIO_readBit															 	        		*/
/* Inputs        :  PORT_ID ( PORT  )  , PIN_ID PIN 													        */
/* Outputs       :  uint8           																            */
/* Reentrancy    :  Reentrant																		            */
/* Synchronous   :  Synchronous																     	            */
/* Description   :	This function is used to red  a specific bit in specific port								*/
/****************************************************************************************************************/
uint8 GPIO_readBit ( PORT_ID PORT  ,  PIN_ID PIN ) ;


/****************************************************************************************************************/
/* Function Name :  GPIO_writePort															 	        		*/
/* Inputs        :  PORT_ID ( PORT  )  , uint8 val  													        */
/* Outputs       :  void	           																            */
/* Reentrancy    :  Reentrant																		            */
/* Synchronous   :  Synchronous																     	            */
/* Description   :	This function is used write  8 bit value for the entire port								*/
/****************************************************************************************************************/
void GPIO_writePORT( PORT_ID PORT , uint8 val );


/****************************************************************************************************************/
/* Function Name :  GPIO_readPort															 	        		*/
/* Inputs        :  PORT_ID ( PORT  )  , uint8 val  													        */
/* Outputs       :  uint8	           																            */
/* Reentrancy    :  Reentrant																		            */
/* Synchronous   :  Synchronous																     	            */
/* Description   :	This function is used read  8 bit value for the entire port     							*/	 							
/****************************************************************************************************************/
uint8 GPIO_readPORT( PORT_ID PORT , uint8 val ) ; 



#endif 
