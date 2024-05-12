#include "LED.h"

/**************************************************/
/* Function Name: Led_init                        */
/* Inputs: PORT_ID & PIN_ID                       */
/* Outputs: void (no output)                      */
/* Reentrancy: Reentrant                          */
/* Synchronous: Synchronous                       */	        
/* Description: Configure the leds                */           
/**************************************************/

void Led_init( PORT_ID port, PIN_ID pin){
  
   GPIO_initDigital ( port );
   GPIO_setupPinDirection (port  , pin , OUTPUT_PIN_DIR , NO_USE );

}

/****************************************************/
/* Function Name: LED_write                         */
/* Inputs: PORT_ID & PIN_ID                         */
/* Outputs: void(no output)                         */
/* Reentrancy: Reentrant                            */
/* Synchronous: Synchronous                         */	        
/* Description: Check if there is any change on Leds*/           
/****************************************************/

void LED_write ( PORT_ID port, PIN_ID pin, uint8 val){

 GPIO_writeBit ( port  ,  pin , val );
 
}