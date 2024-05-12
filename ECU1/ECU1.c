#include "ECU1.h"

uint32 DTC;

/*


uint8 g_Switch_flag=0;
uint8 g_Status_flag = 0;


uint8 g_Message_PC;
uint32 g_State_Check = 0;
uint8 nextStatus = 0;
*/

const CAN_configType  CAN_config = { CAN0_ID , BASIC , 2 , 1 , 1 , 4 , 0  };
uint8 transmitData[8] = {0};
uint8 receiveData[8] = {0};
const CAN_msgObj msgObjTransmit = { CAN0_ID , 1, MSG_TX , ID_11_BIT , 0x1 , 0x1 , transmitData , 8 , NO_INT , NO_INT };

CAN_msgObj msgObjReceive = { CAN0_ID , 2 , MSG_RX , ID_11_BIT , 2 , 2 , receiveData , 8 , NO_INT , NO_INT };


const CAN_msgObj msgObjTransmitRemote = { CAN0_ID , 1 , MSG_TX_REMOTE , ID_11_BIT , 7 , 7 , transmitData , 8 , NO_INT , NO_INT };



extern uint32 g_tick ;

State currentState = -1;
uint8 Keep_Alive = S_KEEP_ALIVE;
uint8 Temperature = 0;
uint32 Received_Voltage = -1;
uint8 Received_State= 2;
Status_typedef g_status = STOP;
extern Task_ref task1, task2, task3, task4, task5, task6, task7;
uint8 Switch1_Switch2_flag = 0;
uint8 Switch2_flag = 0;
uint8 Switch1_flag = 0;
uint8 Remote_Sent = 0;
uint8 NO_COMMUNICATION_FLAG = 0;
uint8 OVERHEAT_FLAG = 0;
uint8 Fault_Counter_NOCOMM = 0;
uint8 Fault_Counter_OVERHEAT= 0;
uint8 flag1 = 0, flag2 = 0, flag3 = 0, flag4 = 0, flag5 = 1, flag6 = 0, flag7=0;
/******************************************************************************/
/* Function Name: Check_Switch_Task                                           */
/* Inputs: void (no input)                                                    */
/* Outputs: void (no output)                                                  */
/* Reentrancy: Non-Reentrant Function                                         */
/* Synchronous: Synchronous                                                   */	        
/* Description: This Task is responsible for checking the status of 2 switches*/           
/*              and determine which state will be executed                   */                                         
/******************************************************************************/


void Send_KeepAlive_Task(void)
{
  while (1)
  {
    if (flag1 == 0)
    {
      Keep_Alive = S_KEEP_ALIVE;
      transmitData[0] = Keep_Alive;
      CAN_configMsgObj(&msgObjTransmit);
      //CAN_configTransmitMessageObject ( Keep_Alive );
    }
    OS_HoldTask(Send_KeepAliveTask);
    
  }
}

void Receive_KeepAlive_Task(void)
{
  while (1)
  {
    if (flag2 == 0)
    {
      static uint8 counter = 0;
      // static uint8 Prev_Keep_Alive = 0;;
      static uint32 No_Comm_Counter = 1;
      // Keep_Alive = (uint8)(CAN_configReceiveMessageObject());
      // Keep_Alive =  uart_receiveByte (UART5);
      CAN_receiveMsgObj (  &msgObjReceive ) ;
      Keep_Alive =  receiveData[0];
      
      if (Keep_Alive == 0 && (No_Comm_Counter % COMMUNICATION_TIMEOUT) ==  0)
      {
        currentState = BLUE;
        counter++;
        
        
        //while(1);
      }
      else
      {
        No_Comm_Counter++;
      }
      
      if (counter == 3)
      {
        counter = 0;
        SemaphoreGive(SwitchStatesSempahore);
      }
      if (Keep_Alive == R_KEEP_ALIVE)
      {
        receiveData[0]=0;
        if (currentState!=  WHITE && NO_COMMUNICATION_FLAG == 0)
        {
          currentState = GREEN;
          SemaphoreGive(SwitchStatesSempahore);
          
        }
        else if (NO_COMMUNICATION_FLAG == 1)
        {
          currentState = GREEN;
          NO_COMMUNICATION_FLAG = 0;
          Fault_Counter_NOCOMM = 0;
          flag3=0;
          flag4=0;
          SemaphoreGive(SwitchStatesSempahore);
        }
        Keep_Alive = S_KEEP_ALIVE;
        //  Prev_Keep_Alive = Keep_Alive;
        No_Comm_Counter = 1;
      }
      
    }
    
    OS_HoldTask(Receive_KeepAliveTask);
    
    
  }
}

void switchStates(void)
{
  while(1)
  {
    
    SemaphoreTake(SwitchStatesSempahore, switchStatesTask);
    
    switch(currentState) {
    case WHITE:
      LED_write(RedLedPort, RedLedPin, LOGIC_HIGH);
      LED_write(BlueLedPort, BlueLedPin, LOGIC_HIGH);
      LED_write(GreenLedPort, GreenLedPin, LOGIC_HIGH);
      transmitData[5] = currentState;
      CAN_configMsgObj(&msgObjTransmit);
      OS_ActivateTask(OverheatTask);
      break;
    case RED:
      LED_write(RedLedPort, RedLedPin, LOGIC_HIGH);
      LED_write(BlueLedPort, BlueLedPin, LOGIC_LOW);
      LED_write(GreenLedPort, GreenLedPin, LOGIC_LOW);
      flag5= 0;
      transmitData[5] = currentState;
      CAN_configMsgObj(&msgObjTransmit);
      OS_ActivateTask(DTCTask);
      
      break;
    case GREEN:
      LED_write(RedLedPort, RedLedPin, LOGIC_LOW);
      LED_write(BlueLedPort, BlueLedPin, LOGIC_LOW);
      LED_write(GreenLedPort, GreenLedPin, LOGIC_LOW);
      transmitData[5] = currentState;
      CAN_configMsgObj(&msgObjTransmit);
      EraseM_Block(DTC_Addr);
      break;
    case BLUE:
      LED_write(RedLedPort, RedLedPin, LOGIC_LOW);
      LED_write(BlueLedPort, BlueLedPin, LOGIC_HIGH);
      LED_write(GreenLedPort, GreenLedPin, LOGIC_LOW);
      //uart_transmitString (  UART0 , "NO COMMUNICATION" ) ;
      OS_ActivateTask(No_CommunicationTask);
      break;
    default:
      break;
      
    }
    
  }
  
  
}





void Process_ADC_Reading (void)
{
  while(1)
  {
    static uint8 counter = 0;
    static uint8 counter2 = 0;
    if (flag4 == 0)
    {
      static uint8 prev_state = 0;
      static uint32 Fault_Counter = 1 ;
      uint8 Remote_received = 0;
      //   CAN_receiveMsgObj (  &msgObjReceive ) ;
      //Received_State = receiveData[5];
      CAN_receiveMsgObj (  &msgObjReceive ) ;
      Remote_received =  receiveData[7];
      
      Temperature =  receiveData[4];
      
      if (Temperature > MAX_TEMP)
      {
        if ( (Fault_Counter % 7) == 0)
        {
          CAN_configMsgObj(&msgObjTransmitRemote); 
          Remote_Sent = 1;
        }
        else
        {
          //Prev_Temp = Temperature;
          Fault_Counter++ ;
        }
        //CAN_configMsgObj(&msgObjTransmitRemote); 
      }
      else if (/*Temperature >= MIN_TEMP && */ Temperature <= MAX_TEMP)
      {
        
        currentState = GREEN;
        Remote_Sent = 0;
        SemaphoreGive(SwitchStatesSempahore);
        transmitData[5] = currentState;
        CAN_configMsgObj(&msgObjTransmit);
        Fault_Counter = 1;
        counter2++;
      }
      else
      {
        Fault_Counter = 1;
      }
      if (counter2 == 3)
      {
        SemaphoreGive(SwitchStatesSempahore);
      }
      CAN_receiveMsgObj (  &msgObjReceive ) ;
      Received_Voltage =  receiveData[3];
      if(Remote_Sent == 1)
      {
        if (Received_Voltage == KNOWN_VOLTAGE)
        {
          currentState = WHITE;
          currentState = WHITE;
          transmitData[5] = currentState;
          CAN_configMsgObj(&msgObjTransmit);
          counter++;
          // SemaphoreGive(SwitchStatesSempahore);
        }
        else if ((Received_Voltage != KNOWN_VOLTAGE) )//&& Received_Voltage != 0)
        {
          DTC = RED;
          WriteM_Byte(DTC_Addr, DTC);
          currentState = RED;
          transmitData[5] = currentState;
          CAN_configMsgObj(&msgObjTransmit);
          counter++;
          //SemaphoreGive(SwitchStatesSempahore);
        }
        
        if (counter== 3)
        {
          counter = 0;
          Fault_Counter=1;
          SemaphoreGive(SwitchStatesSempahore);
          
        }
      }
      
      OS_HoldTask(Process_ADC_ReadingTask);
    }
  }
}

void DTC_Task (void)
{
  
  if (ReadM_Byte(DTC_Addr)== BLUE)
  {
    currentState = BLUE;
      LED_write(RedLedPort, RedLedPin, LOGIC_LOW);
      LED_write(BlueLedPort, BlueLedPin, LOGIC_HIGH);
      LED_write(GreenLedPort, GreenLedPin, LOGIC_LOW);
      flag5=0;
  }
  else if(ReadM_Byte(DTC_Addr)== RED)
  {
    currentState = RED ;
    LED_write(RedLedPort, RedLedPin, LOGIC_HIGH);
      LED_write(BlueLedPort, BlueLedPin, LOGIC_LOW);
      LED_write(GreenLedPort, GreenLedPin, LOGIC_LOW);
      flag5=0;
  }
  else if (ReadM_Byte(DTC_Addr)== WHITE)
  {
    currentState = WHITE;
    LED_write(RedLedPort, RedLedPin, LOGIC_HIGH);
    LED_write(BlueLedPort, BlueLedPin, LOGIC_HIGH);
    LED_write(GreenLedPort, GreenLedPin, LOGIC_HIGH);
    flag5=0;
  }

  else
  {
  OS_HoldTask(DTCTask);
  }
  while(1)
  {
    if (flag5 == 0)
    {
      static uint8 GO_TO_DTC = 0;
      static uint8 counter =0;
      if (currentState == WHITE && counter <=30)
      {
       GO_TO_DTC = 1;
        transmitData[6] = GO_TO_DTC;
        CAN_configMsgObj(&msgObjTransmit);
        counter++ ;
      }
      uint8 OUT=0;
      CAN_receiveMsgObj (  &msgObjReceive ) ;
      OUT= receiveData[5];
      static uint8 Send_Ready = 0;
      static uint8 Receive_Ready = 0;
      //static uint8 received_state = -1;
      if(currentState  == RED)
      {
        flag1=1;
        flag2=1;
        flag3=1;
        flag4=1;
        
        CAN_receiveMsgObj (  &msgObjReceive ) ;
        Receive_Ready = receiveData[1];
        if (!(SwitchIsPressed ( Switch1_port , Switch1_pin)) && !(SwitchIsPressed ( Switch2_port , Switch2_pin)) ){
          while (OS_GetTime() % 40 != 0);  /* For debouncing*/
          if (!(SwitchIsPressed ( Switch1_port , Switch1_pin)) && !(SwitchIsPressed ( Switch2_port , Switch2_pin))){
            if (Switch1_Switch2_flag == 0){
              Switch1_Switch2_flag = 1 ;
              Send_Ready = READY;
              transmitData[1] = Send_Ready;
              CAN_configMsgObj(&msgObjTransmit);
            }
          }
        }
        else{
          Switch1_Switch2_flag = 0;
        }
        
        if( (Send_Ready == READY && Receive_Ready == READY) )//|| OUT == GREEN )
        {
          currentState = GREEN ;
          
          
          flag1 = 0;
          flag2 = 0;
          flag3 = 0;
          flag4 = 0;
          flag5 = 1;
          //flag6 = 1;
          Remote_Sent = 0;
          Received_Voltage = 0;
          transmitData[1] = Send_Ready;
          CAN_configMsgObj(&msgObjTransmit);
          transmitData[5] = currentState;
          CAN_configMsgObj(&msgObjTransmit);
          // Received_Voltage = 30;
          for(int i =0; i<8 ; i++)
          {
            transmitData[i] = 0;
            receiveData[i] = 0;
          }
          Send_Ready = 0;
          Receive_Ready=0;
         // EraseM_Block(DTC_Addr);
          SemaphoreGive(SwitchStatesSempahore);
          OS_HoldTask(DTCTask);
          // while (OS_GetTime() % 40 != 0);
          
        }
      }
      else if(currentState  == BLUE)
      {
        flag1=1;
        flag2=1;
        flag3=1;
        flag4=1;
        
        //Receive_Ready = uart_receiveByte(UART4);
        CAN_receiveMsgObj (  &msgObjReceive ) ;
        
        Receive_Ready = receiveData[1];
         if ((SwitchIsPressed ( Switch1_port , Switch1_pin)) && !(SwitchIsPressed ( Switch2_port , Switch2_pin)) ){
          while (OS_GetTime() % 40 != 0);  /* For debouncing*/
           if ((SwitchIsPressed ( Switch1_port , Switch1_pin)) && !(SwitchIsPressed ( Switch2_port , Switch2_pin)) ){
            if (Switch2_flag == 0){
              Switch2_flag = 1 ;
              Send_Ready = READY;
              transmitData[1] = Send_Ready;
              CAN_configMsgObj(&msgObjTransmit);
            }
          }
        }
        else{
          Switch2_flag = 0;
        }
        
        if( (Send_Ready == READY && Receive_Ready == READY)) //|| OUT == GREEN )
        {
          transmitData[1] = Send_Ready;
          CAN_configMsgObj(&msgObjTransmit);
          // Temperature = 20;
          //Received_Voltage = KNOWN_VOLTAGE;
          currentState = GREEN ;
          flag1 = 0;
          flag2 = 0;
          flag3 = 0;
          flag4 = 0;
          flag5 = 1;
          Send_Ready = 0;
          Receive_Ready = 0;
          
          SemaphoreGive(SwitchStatesSempahore);
          OS_HoldTask(DTCTask);
          // while (OS_GetTime() % 40 != 0);
          
        }
      }
      else if(currentState == WHITE)
      {
        flag1=1;
        flag2=1;
        flag3=1;
        flag4=1;
        
        CAN_receiveMsgObj (  &msgObjReceive ) ;
        
        Receive_Ready = receiveData[1];
        if (!(SwitchIsPressed ( Switch1_port , Switch1_pin)) && (SwitchIsPressed ( Switch2_port , Switch2_pin)) ){
          while (OS_GetTime() % 40 != 0);  /* For debouncing*/
           if (!(SwitchIsPressed ( Switch1_port , Switch1_pin)) && (SwitchIsPressed ( Switch2_port , Switch2_pin)) ){
            if (Switch1_flag == 0){
              Switch1_flag = 1 ;
              Send_Ready = READY;
              transmitData[1] = Send_Ready;
              CAN_configMsgObj(&msgObjTransmit);
            }
          }
        }
        else{
          Switch1_flag = 0;
        }
        
        CAN_receiveMsgObj (  &msgObjReceive ) ;
        Receive_Ready = receiveData[1];
        if( (Send_Ready == READY && Receive_Ready == READY) || (OUT == GREEN ))
        {
          
          // Temperature = 20;
          //Received_Voltage = KNOWN_VOLTAGE;
          currentState = GREEN ;
          
          
          flag1 = 0;
          flag2 = 0;
          flag3 = 0;
          flag4 = 0;
          flag5 = 0;
          flag6 = 1;
          Remote_Sent = 0;
          
          transmitData[1] = Send_Ready;
          CAN_configMsgObj(&msgObjTransmit);
          transmitData[5] = currentState;
          CAN_configMsgObj(&msgObjTransmit);
          GO_TO_DTC = 0;
          counter = 0;
          for(int i =0; i<8 ; i++)
          {
            transmitData[i] = 0;
            receiveData[i] = 0;
          }
          Send_Ready = 0;
          Receive_Ready=0;
           //EraseM_Block(DTC_Addr);
          SemaphoreGive(SwitchStatesSempahore);
          OS_HoldTask(DTCTask);
          // while (OS_GetTime() % 40 != 0);
          
        }
        CAN_receiveMsgObj (  &msgObjReceive ) ;
        Receive_Ready = receiveData[1];
      }
      // OS_HoldTask(&task5);
    }
  }
}

void No_Communication_Task (void)
{
  OS_HoldTask(No_CommunicationTask);
  
  while(1)
  {
    
    NO_COMMUNICATION_FLAG = 1;
    Fault_Counter_NOCOMM++;
    flag3=1;
    flag4=1;
    if (Fault_Counter_NOCOMM == 3)
    {
      Fault_Counter_NOCOMM = 0;
      DTC= BLUE;
      WriteM_Byte(DTC_Addr, DTC);
      
      flag5=0;
      currentState = BLUE;
      OS_ActivateTask(DTCTask);
      //OS_HoldTask(&task6);
    }
    if( Fault_Counter_NOCOMM  !=3 )
      while (OS_GetTime() % 2000 != 0)
      {
        Keep_Alive = S_KEEP_ALIVE;
        transmitData[0] = Keep_Alive;
        CAN_configMsgObj(&msgObjTransmit);
        
        
      };
    
    
    OS_HoldTask(No_CommunicationTask);
  }
}

void Overheat_Task (void)
{
  OS_HoldTask(OverheatTask);
  
  while(1)
  {
    uint8 FLAG= 0;
    //static uint8 Fault_Counter = 0;
    //OVERHEAT_FLAG= 1;
    Fault_Counter_OVERHEAT++;
    if (Fault_Counter_OVERHEAT == 3)
    {
      Fault_Counter_OVERHEAT = 0;
      DTC  = WHITE;
      WriteM_Byte(DTC_Addr, DTC);
      
      flag5=0;
      currentState = WHITE;
      OS_ActivateTask(DTCTask);
      OS_HoldTask(OverheatTask);
      //OS_HoldTask(&task6);
    }
    if( Fault_Counter_OVERHEAT !=3 )
      while (OS_GetTime() % 2000 != 0)
      {
        
      };
    
    
    OS_HoldTask(OverheatTask);
  }
}

void Send_message_to_PC (void)
{
  while(1)
  {
    switch(currentState) {
    case WHITE:
      
      uart_transmitString (  UART0 , "OVERHEAT\n" ) ;
      break;
    case RED:
      uart_transmitString (  UART0 , "FAULT STATE\n" ) ;
      
      break;
    case GREEN:
      uart_transmitString (  UART0 , "GOOD\n" ) ;
      break;
    case BLUE:
      uart_transmitString (  UART0 , "NO COMMUNICATION\n" ) ;
      break;
    default:
      break;
    }
    OS_HoldTask(SendToPCTask);
  }
}