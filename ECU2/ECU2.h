#ifndef ECU2_H_
#define ECU2_H_


#include <stdio.h>
//#include <stdint.h>
//#include <stdbool.h>
#include "gpio.h"
#include "systick.h"
#include "tm4c123gh6pm.h"
#include "bitwise_operation.h"
#include "types.h"
#include "Schedular.h"
//#include "SSI.h"
#include "uart.h"
//#include "GPTM.h"
#include "switch.h"
#include "LED.h"
#include "CAN.h"
#include "FlashM.h"
#include "adc.h"

#define S_KEEP_ALIVE 20
#define R_KEEP_ALIVE 10
#define COMMUNICATION_TIMEOUT 51
#define NORMAL_TEMPERATURE 25
#define OVERHEAT 50
#define KNOWN_VOLTAGE 3.3
#define REMOTE_FRAME 8
#define READY 1
typedef enum {
  Pad,
  WHITE,
  RED,
  GREEN,
  BLUE
} State;

typedef enum {
  STOP,
  CLOCKWISE,
  ANTI_CLOCKWISE,
  IDLE
} Direction;


void Send_KeepAlive_Task(void);
void Receive_KeepAlive_Task(void);
void Send_ADC_Reading(void);
void DTC_Task (void);
void No_Communication_Task (void);
void Overheat_Task (void);
/******************************************************************************/
/* Function Name: switchStates                                                */
/* Inputs: void (no inputs)                                                   */
/* Outputs: void (no outputs)                                                 */
/* Reentrancy: Non Reentrant                                                  */
/* Synchronous: Synchronous                                                   */	        
/* Description: Check the current state and acts accordingly                  */           
/******************************************************************************/
void switchStates (void);

/******************************************************************************/
/* Function Name: SPI_GetMessage                                              */
/* Inputs: void (no inputs)                                                   */
/* Outputs: void (no outputs)                                                 */
/* Reentrancy: Non Reentrant                                                  */
/* Synchronous: Synchronous                                                   */	        
/* Description: Read message that contains the state from SPI                 */           
/******************************************************************************/
void Can_GetMessage (void);

/******************************************************************************/
/* Function Name: Check_GPTM                                                  */
/* Inputs: void (no inputs)                                                   */
/* Outputs: void (no outputs)                                                 */
/* Reentrancy: Non Reentrant                                                  */
/* Synchronous: Synchronous                                                   */	        
/* Description: this task is responsible for starting timer                   */
/*                if any error occurs while receiving from SPI                */           
/******************************************************************************/
//void Check_GPTM (void);
void sendVoltage (void);

/******************************************************************************/
/* Function Name: SendToEcu1                                                  */
/* Inputs: void (no inputs)                                                   */
/* Outputs: void (no outputs)                                                 */
/* Reentrancy: Non Reentrant                                                  */
/* Synchronous: Synchronous                                                   */	        
/* Description: Send the operating state to ECU1                              */           
/******************************************************************************/
void sendToEcu1(void);

void CheckState_Task (void);

#endif