#ifndef ECU1_H_
#define ECU1_H_

#include "tm4c123gh6pm.h"
#include "gpio.h"
#include "switch.h"
#include "LED.h"
#include <stdio.h>
//#include "SSI.h"
#include "Schedular.h"
#include "uart.h"
#include "CAN.h"
#include "FlashM.h"

#define S_KEEP_ALIVE 10
#define R_KEEP_ALIVE 20
#define MIN_TEMP 10
#define MAX_TEMP 25
#define COMMUNICATION_TIMEOUT 51
#define KNOWN_VOLTAGE 5
#define READY 1
/**************************************************************States Definition*******************************************************************/
typedef enum {
  STOP = 0,
  CW = 1, 
  CCW = 2 , 
  IDLE = 3
  }Status_typedef;


typedef enum {
  Pad,
  WHITE,
  RED,
  GREEN,
  BLUE
} State;

/******************************************************************** Functions ********************************************************************/

void switchStates(void);

void DTC_Task (void);
void Process_ADC_Reading (void);
void No_Communication_Task (void);
void Overheat_Task(void);

/******************************************************************************/
/* Function Name: Check_Switch_Task                                           */
/* Inputs: void (no input)                                                    */
/* Outputs: void (no output)                                                  */
/* Reentrancy: Non-Reentrant Function                                         */
/* Synchronous: Synchronous                                                   */	        
/* Description: This Task is responsible for checking the status of 2 switches*/           
/*               and determine which state will be executed                   */                                         
/******************************************************************************/
void Send_KeepAlive_Task (void);

/******************************************************************************/
/* Function Name: Check_Switch_Task                                           */
/* Inputs: void (no input)                                                    */
/* Outputs: void (no output)                                                  */
/* Reentrancy: Non-Reentrant Function                                         */
/* Synchronous: Synchronous                                                   */	        
/* Description: This Task is responsible for checking the status of 2 switches*/           
/*               and determine which state will be executed                   */                                         
/******************************************************************************/
void Receive_KeepAlive_Task (void);


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
//void SendState_Task (void);

#endif