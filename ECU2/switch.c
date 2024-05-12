#include "switch.h"


/******************************************************************************/
/* Function Name: Switch_init                                                 */
/* Inputs: PORT_ID & PIN_ID                                                   */
/* Outputs: void (no output)                                                  */
/* Reentrancy: Reentrant Function                                             */
/* Synchronous: Synchronous                                                   */	        
/* Description: Configure the switches                                        */                               
/******************************************************************************/

void Switch_init( PORT_ID port , PIN_ID pin ){
  GPIO_initDigital ( port );
  GPIO_setupPinDirection (port, pin, INPUT_PIN_DIR, USE_PULL );
}

/******************************************************************************/
/* Function Name: SwitchIsPressed                                             */
/* Inputs: PORT_ID & PIN_ID                                                   */
/* Outputs: uint8 (GPIO GET_BIT                                               */
/* Reentrancy: Reentrant Function                                             */
/* Synchronous: Synchronous                                                   */	        
/* Description: Check if there is any change on pins                          */                                         
/******************************************************************************/

uint8 SwitchIsPressed (PORT_ID port, PIN_ID pin){
  return GPIO_readBit (port, pin ) ;
}