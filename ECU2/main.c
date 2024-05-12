#include "tm4c123gh6pm.h"
//#include "gpio.h"
#include <stdio.h>
#include "Schedular.h"
#include "ECU2.h"
//#include "SSI.h"
#include "uart.h"
#include "GPTM.h"
#include "LED.h"
#include "switch.h"
#include "string.h"
#include "CAN.h"
#include "DIO.h"
#include "FlashM.h"
#include "adc.h"
/***************************************************************    Time Slice Variables    *****************************************************************/
//uint32  g_tick = 0; 
extern uint8 initialTime;
extern uint8 nextTime;
extern uint8 timeSlice;

extern uint8 transmitData[8] ;
extern const CAN_configType  CAN_config ;
extern uint8 receiveData[8] ;
extern CAN_msgObj msgObjReceive ;
extern const CAN_msgObj msgObjTransmit ;

extern CAN_msgObj msgObjReceiveRemote ;



/****************************************************************  Flags Initialization  ********************************************************************/
extern uint8 g_Switch_flag;
extern uint8 g_Status_flag;
extern uint32 g_State_Check;


/****************************************************************    Generic Variables   ********************************************************************/
//extern SSI_Config_t SSI_Configuration;
//extern UART_ConfigType Uart0_Config;
//extern UART_ConfigType Uart5_Config;


/**************************************************/
/* Function Name: SysTick_Handler                 */
/* Inputs: None                                   */
/* Outputs:None                                   */
/* Reentrancy:                                    */
/* Synchronous:                                   */	        
/* Description: ISR                               */           
/**************************************************/

UART_ConfigType Uart5_Configs = { BIT_8 , NO_PARITY , ONE_BIT , ENABLE_FIFO , 115200 };
UART_ConfigType Uart2_Configs = { BIT_8 , NO_PARITY , ONE_BIT , ENABLE_FIFO , 115200 };
UART_ConfigType Uart1_Configs = { BIT_8 , NO_PARITY , ONE_BIT , ENABLE_FIFO , 115200 };
UART_ConfigType Uart7_Configs = { BIT_8 , NO_PARITY , ONE_BIT , ENABLE_FIFO , 115200 };
UART_ConfigType Uart4_Configs = { BIT_8 , NO_PARITY , ONE_BIT , ENABLE_FIFO , 115200 };


void main(void)
{
  
  GPIO_initDigital ( PORTF );
  
  /**************************************************************** Setting Pins Directions ***********************************************/
  GPIO_setupPinDirection ( PORTF  , PIN0 , INPUT_PIN_DIR , USE_PULL  );
  GPIO_setupPinDirection ( PORTF  , PIN4 , INPUT_PIN_DIR , USE_PULL  );
  
  GPIO_setupPinDirection ( PORTF  , PIN1 , OUTPUT_PIN_DIR , NO_USE  );
  GPIO_setupPinDirection ( PORTF  , PIN2 , OUTPUT_PIN_DIR , NO_USE  );
  GPIO_setupPinDirection ( PORTF  , PIN3 , OUTPUT_PIN_DIR , NO_USE  );
  
  //uart_init( UART0 ,  &Uart0_Config );
  
  //SSI_Init(&SSI_Configuration);
  // CAN_init();
  //CAN_configTransmitMessageObject(0);
  uart_init( UART5 ,  &Uart5_Configs ) ;
  uart_init( UART2 ,  &Uart2_Configs ) ;
  uart_init( UART1 ,  &Uart1_Configs ) ;
  uart_init( UART7 ,  &Uart7_Configs ) ;
  uart_init( UART4 ,  &Uart4_Configs ) ;
  LM35_Init();
  REF_init();
   GPIO_initCan ();
  CAN_init ( &CAN_config ) ;
  CAN_enable ( &CAN_config ) ;
  CAN_configMsgObj(&msgObjReceive);
  //CAN_configMsgObj(&msgObjReceiveRemote); 
  // CAN_configTransmitMessageObject2(0);
  
  
  //EraseM_Block(17); 
  
 
  
  OS_Init();
  
  
 OS_CreateTask(Send_KeepAliveTask);
 OS_CreateTask(Receive_KeepAliveTask);
 
  OS_CreateTask(switchStatesTask);
 
   OS_CreateTask(Send_ADC_ReadingTask);
  OS_CreateTask(sendVoltageTask);
  OS_CreateTask(DTCTask);
  OS_CreateTask(No_CommunicationTask);
  OS_CreateTask(OverheatTask);
  // OS_CreateTask(CheckStateTask);
    
  OS_Start();
  
  while(1){    
  }
}

