#ifndef SWITCH_H_
#define SWITCH_H_

#include "tm4c123gh6pm.h"
#include "gpio.h"

/*************************************************************** Pins Definition ************************************************************/
#define Switch1_pin PIN0
#define Switch2_pin PIN4

/*************************************************************** Ports Definition ************************************************************/
#define Switch1_port PORTF
#define Switch2_port PORTF

/******************************************************************************/
/* Function Name: Switch_init                                                 */
/* Inputs: PORT_ID & PIN_ID                                                   */
/* Outputs: void (no output)                                                  */
/* Reentrancy: Reentrant Function                                             */
/* Synchronous: Synchronous                                                   */	        
/* Description: Configure the switches                                        */                               
/******************************************************************************/
void Switch_init( PORT_ID port, PIN_ID pin );

/******************************************************************************/
/* Function Name: SwitchIsPressed                                             */
/* Inputs: PORT_ID & PIN_ID                                                   */
/* Outputs: uint8 (GPIO GET_BIT                                               */
/* Reentrancy: Reentrant Function                                             */
/* Synchronous: Synchronous                                                   */	        
/* Description: Check if there is any change on pins                          */                                         
/******************************************************************************/
uint8 SwitchIsPressed (PORT_ID port , PIN_ID pin);


#endif