#include "ECU1.h"


extern uint32 g_tick ;
Status_typedef g_status = STOP;
uint8 g_Switch_flag=0;
uint8 g_Status_flag = 0;
uint8 RED_ON = 0;
uint32 g_counter=0;
uint8 g_Message_PC;
uint32 g_State_Check = 0;
uint8 nextStatus = 0;

/******************************************************************************/
/* Function Name: Check_Switch_Task                                           */
/* Inputs: void (no input)                                                    */
/* Outputs: void (no output)                                                  */
/* Reentrancy: Non-Reentrant Function                                         */
/* Synchronous: Synchronous                                                   */	        
/* Description: This Task is responsible for checking the status of 2 switches*/           
/*              and determine which state will be executed                   */                                         
/******************************************************************************/
void Check_Switch_Task (void){
  if (g_Switch_flag==0)
  {
    g_Switch_flag=1;
    uint8 Switch1_flag = 0;
    uint8 Switch2_flag = 0;
    uint8 Switch1_Switch2_flag = 0;
    static uint32 prev_counter=0;
    static uint32 tick=1;
    static uint8 State= 7;
    
  //  nextStatus = g_status;
    
if ((prev_counter == g_counter) && (tick % 451==0))
    {
      g_status = IDLE;
      RED_ON=0;
      //LED_write(PORTF, PIN1, LOGIC_LOW);
    }
    else if ( State == g_Message_PC && (tick % 451==0)  )
    {  
      g_status = IDLE;
      RED_ON=0; 
    }
    else if ((State != g_Message_PC))
    {
      State = g_Message_PC;
      prev_counter = g_counter;
      tick=1;
    }
    else if (prev_counter != g_counter)
    {
      prev_counter = g_counter;
    }

    else
    {
      tick++;
    }
    /*Check if Switch 1 is pressed, go to status of Clock-wise*/
    if (!(SwitchIsPressed ( Switch1_port , Switch1_pin))){
      while (g_tick % 40 != 0); /* For debouncing*/
      if (!(SwitchIsPressed ( Switch1_port , Switch1_pin))){
        if (Switch1_flag ==0){
          Switch1_flag = 1 ;
          g_counter++;
          g_status = CW ; /* setting status to CW*/
          RED_ON = 0;
        }
      }
    }
    else{
      Switch1_flag =0;
    }
    
    /*Check if Switch 2 is pressed, go to status of Counter Clock-wise*/
    if (!(SwitchIsPressed ( Switch2_port , Switch2_pin))){
      while (g_tick % 40 != 0); /* For debouncing*/
      if (!(SwitchIsPressed ( Switch2_port , Switch2_pin))){
        if (Switch2_flag ==0){
          Switch2_flag = 1 ;
          g_counter++;
          g_status = CCW ; /* setting status to CCW*/
          RED_ON = 0;
        }
      }  
    }
    else{
      Switch2_flag =0;
    }
    
    /*Check if Switch 1 and 2 are pressed, go to status of STOP (both red Leds of ECU1 and ECU2 will be enlightened)*/
    if (!(SwitchIsPressed ( Switch1_port , Switch1_pin)) && !(SwitchIsPressed ( Switch2_port , Switch2_pin)) ){
      while (g_tick % 40);  /* For debouncing*/
      if (!(SwitchIsPressed ( Switch1_port , Switch1_pin)) && !(SwitchIsPressed ( Switch2_port , Switch2_pin))){
        if (Switch1_Switch2_flag == 0){
          Switch1_Switch2_flag = 1 ;
          g_counter++;
          g_status = STOP ; /* setting status to STOP (Red Led)*/
          RED_ON = 1;
        }
      }
    }
    else{
      Switch1_Switch2_flag = 0;
    }
  }
}


/******************************************************************************/
/* Function Name: LED_Task                                                    */
/* Inputs: void (no input)                                                    */
/* Outputs: void (no output)                                                  */
/* Reentrancy:  Reentrant                                                     */
/* Synchronous: Synchronous                                                   */	        
/* Description:This task is responible for turning on Red Light               */
/*               when 2 switched are pressed                                  */         
/******************************************************************************/

void LED_Task (void){
  /* Check the variable RED_ON to turn on red light by setting it to LOGIC_HIGH
     and passing this value to GPIO_writeBit*/
  if(RED_ON){
    LED_write(RedLedPort, RedLedPin, LOGIC_HIGH);
  }
  /* Turn off Red light by setting it to LOGIC_LOW 
     and passing this value to GPIO_writeBit*/
  else{
    LED_write(RedLedPort, RedLedPin, LOGIC_LOW);
  }
}

/******************************************************************************/
/* Function Name: Send_message_to_ECU2                                        */
/* Inputs: void (no input)                                                    */
/* Outputs: void (no output)                                                  */
/* Reentrancy: Non_Reentrant                                                  */
/* Synchronous: Synchronous                                                   */	        
/* Description: This task is responible for passing the status to ECU2        */           
/******************************************************************************/

void Send_message_to_ECU2 (void){
  SSI_Send((uint16)g_status, SSI_Polling_Enable);
}

/******************************************************************************/
/* Function Name: Send_message_to_PC                                          */
/* Inputs: void (no input)                                                    */
/* Outputs: void (no output)                                                  */
/* Reentrancy: Non_Reentrant                                                  */
/* Synchronous: Synchronous                                                   */	        
/* Description: This task is responible for sending the status or error state */
/*               to PC by UART                                                */           
/******************************************************************************/

void Send_message_to_PC (void){
  
  if (g_Status_flag==0){
    
    /* Set flag to 1 to send the status once*/
    g_Status_flag = 1;
    static uint8 Status = 7;
    
    if (Status != g_Message_PC)
    {
      /* if status is STOP (RED), send to PC initial state*/
      if (g_Message_PC == STOP){
        uart_transmitString (  UART0 , "Initial State\n" ) ;
      }
      /* if status is Clock-wise, send to PC state is CW*/
      else if (g_Message_PC == CW){
        uart_transmitString (  UART0 , "State is CW\n" ) ;
      }
      /* if status is Clock-wise, send to PC state is CCW*/
      else if (g_Message_PC == CCW){
        uart_transmitString (  UART0 , "State is CCW\n" ) ;
      }
      /* if status is Clock-wise, send to PC state is IDLE state*/
      else if (g_Message_PC == IDLE){
        uart_transmitString (  UART0 , "IDLE State\n" ) ;
      }
      else if (g_Message_PC == 7){
        uart_transmitString (  UART0 , "Errorrrrrrrrrrr\n" ) ;
      }
      Status = g_Message_PC;
    }
    
  }
}

void Receive_message_from_ECU2(void) 
{
  g_Message_PC = uart_receiveByte(( UART5 )); 
}
