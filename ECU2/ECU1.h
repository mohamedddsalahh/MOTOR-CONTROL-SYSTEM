#ifndef ECU1_H_
#define ECU1_H_

#include "tm4c123gh6pm.h"
#include "gpio.h"
#include "switch.h"
#include "LED.h"
#include <stdio.h>
#include "SSI.h"
#include "Schedular.h"
#include "uart.h"


/**************************************************************States Definition*******************************************************************/
typedef enum {
  STOP = 0, CW = 1, CCW = 2 , IDLE = 3
             }Status_typedef;

/******************************************************************** Functions ********************************************************************/

/******************************************************************************/
/* Function Name: Check_Switch_Task                                           */
/* Inputs: void (no input)                                                    */
/* Outputs: void (no output)                                                  */
/* Reentrancy: Non-Reentrant Function                                         */
/* Synchronous: Synchronous                                                   */	        
/* Description: This Task is responsible for checking the status of 2 switches*/           
/*               and determine which state will be executed                   */                                         
/******************************************************************************/
void Check_Switch_Task (void);

/******************************************************************************/
/* Function Name: LED_Task                                                    */
/* Inputs: void (no input)                                                    */
/* Outputs: void (no output)                                                  */
/* Reentrancy:  Reentrant                                                     */
/* Synchronous: Synchronous                                                   */	        
/* Description: TThis task is responible for turning on Red Light             */
/*               when 2 switched are pressed                                  */           
/******************************************************************************/
void LED_Task (void);

/******************************************************************************/
/* Function Name: Send_message_to_ECU2                                        */
/* Inputs: void (no input)                                                    */
/* Outputs: void (no output)                                                  */
/* Reentrancy: Non_Reentrant                                                  */
/* Synchronous: Synchronous                                                   */	        
/* Description: This task is responible for passing the status to ECU2        */           
/******************************************************************************/

void Send_message_to_ECU2 (void);

/******************************************************************************/
/* Function Name: Send_message_to_ECPC                                        */
/* Inputs: void (no input)                                                    */
/* Outputs: void (no output)                                                  */
/* Reentrancy: Non_Reentrant                                                  */
/* Synchronous: Synchronous                                                   */	        
/* Description: This task is responible for sending the status or error state */
/*               to PC by UART                                                */           
/******************************************************************************/

void Send_message_to_PC (void); 


void Receive_message_from_ECU2(void) ;

#endif