#ifndef LED_H_
#define LED_H_

#include "tm4c123gh6pm.h"
#include "gpio.h"

/**********************************************************Leds Pins Definition********************************************************************/
#define RedLedPin   PIN1
#define BlueLedPin  PIN2
#define GreenLedPin PIN3

/********************************************************** Ports Definition********************************************************************/
#define RedLedPort   PORTF
#define BlueLedPort  PORTF
#define GreenLedPort PORTF

/**************************************************/
/* Function Name: Led_init                        */
/* Inputs: PORT_ID & PIN_ID                       */
/* Outputs: void (no output)                      */
/* Reentrancy: Reentrant                          */
/* Synchronous: Synchronous                       */	        
/* Description: Configure the leds                */           
/**************************************************/

void Led_init( PORT_ID port, PIN_ID pin);

/****************************************************/
/* Function Name: LED_write                         */
/* Inputs: PORT_ID & PIN_ID                         */
/* Outputs: void(no output)                         */
/* Reentrancy: Reentrant                            */
/* Synchronous: Synchronous                         */	        
/* Description: Check if there is any change on Leds*/           
/****************************************************/

void LED_write ( PORT_ID port, PIN_ID pin, uint8 val); 



#endif