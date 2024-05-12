#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "gpio.h"
#include "systick.h"
#include "tm4c123gh6pm.h"
#include "bitwise_operation.h"
#include "types.h"
#include "ECU1.h"
#include "uart.h"
#include "CAN.h"
#include "DIO.h"
#include "FlashM.h"
/****************************************************************** extern variables ****************************************************/
extern uint8 ECU1_flag;
extern uint8 State_flag;
extern uint8 Get_Message_flag;


extern const CAN_configType  CAN_config ;
extern uint8 transmitData[8] ;
extern uint8 receiveData[8] ;
extern const CAN_msgObj msgObjTransmit ;

extern CAN_msgObj msgObjReceive ;


extern const CAN_msgObj msgObjTransmitRemote ;

//extern uint8 timeSlice;
//extern uint8 initialTime;
//extern uint8 nextTime;
UART_ConfigType Uart5_Configs = { BIT_8 , NO_PARITY , ONE_BIT , ENABLE_FIFO , 115200 };
UART_ConfigType Uart2_Configs = { BIT_8 , NO_PARITY , ONE_BIT , ENABLE_FIFO , 115200 };
UART_ConfigType Uart1_Configs = { BIT_8 , NO_PARITY , ONE_BIT , ENABLE_FIFO , 115200 };
UART_ConfigType Uart7_Configs = { BIT_8 , NO_PARITY , ONE_BIT , ENABLE_FIFO , 115200 };
UART_ConfigType Uart4_Configs = { BIT_8 , NO_PARITY , ONE_BIT , ENABLE_FIFO , 115200 };
UART_ConfigType Uart0_Configs = { BIT_8 , NO_PARITY , ONE_BIT , ENABLE_FIFO , 12006 };

//uint32 DTC;



//extern uint32 g_tick;
//extern SSI_Config_t SSI_Configuration;
//extern UART_ConfigType Uart5_Config;
//extern Timer_ConfigType Timer0A;

/*
void SysTick_Handler(void)
{
  g_tick++;
  if(g_tick == 1001){
    g_tick = 0;
  }

  nextTime = g_tick;
  timeSlice = nextTime - initialTime;
  if(timeSlice == 4){
    initialTime = g_tick;
  }
  State_flag = 0;
  ECU1_flag  = 0;
  Get_Message_flag = 0;
}
*/

 int main (){
   
  
  
  
 
  
   GPIO_initDigital ( PORTF );
   
  /**************************************************************** Setting Pins Directions ***********************************************/
  GPIO_setupPinDirection ( PORTF  , PIN0 , INPUT_PIN_DIR , USE_PULL  );
  GPIO_setupPinDirection ( PORTF  , PIN4 , INPUT_PIN_DIR , USE_PULL  );
  
  GPIO_setupPinDirection ( PORTF  , PIN1 , OUTPUT_PIN_DIR , NO_USE  );
  GPIO_setupPinDirection ( PORTF  , PIN2 , OUTPUT_PIN_DIR , NO_USE  );
  GPIO_setupPinDirection ( PORTF  , PIN3 , OUTPUT_PIN_DIR , NO_USE  );
  LED_write(RedLedPort, RedLedPin, LOGIC_LOW);

  
   GPIO_initCan ();
  CAN_init ( &CAN_config ) ;
  CAN_enable ( &CAN_config ) ;
  CAN_configMsgObj(&msgObjReceive);
  
   uart_init( UART0 ,  &Uart0_Configs ) ;
   uart_init( UART5 ,  &Uart5_Configs ) ;
   uart_init( UART2 ,  &Uart2_Configs ) ;
   uart_init( UART1 ,  &Uart1_Configs ) ;
   uart_init( UART7 ,  &Uart7_Configs ) ;
   uart_init( UART4 ,  &Uart4_Configs ) ;
   
   
   
    //EraseM_Block(19); 
  

  
  OS_Init();
  
  
  OS_CreateTask(Send_KeepAliveTask);
  OS_CreateTask(Receive_KeepAliveTask);
  
  OS_CreateTask(switchStatesTask);
  
  OS_CreateTask(Process_ADC_ReadingTask);
  OS_CreateTask(DTCTask);
  OS_CreateTask(No_CommunicationTask);
  OS_CreateTask(OverheatTask);
  OS_CreateTask(SendToPCTask);
 
  OS_Start();
  
  
while(1)
  {
    
  }
  
  
}       