#include "ECU2.h"

uint32 DTC;


/************************************************************************ Variables Definitions **************************************************************************************/

uint8 transmitData[8] = {0};
const CAN_configType  CAN_config = { CAN0_ID , BASIC , 2 , 1 , 1 , 4 , 0  };
uint8 receiveData[8] = {0};
CAN_msgObj msgObjReceive = { CAN0_ID , 1 , MSG_RX , ID_11_BIT , 1 , 1 , receiveData , 8 , NO_INT , NO_INT };
const CAN_msgObj msgObjTransmit = { CAN0_ID , 2 , MSG_TX , ID_11_BIT , 2 , 2 , transmitData , 8 , NO_INT , NO_INT };

CAN_msgObj msgObjReceiveRemote = { CAN0_ID , 2 , MSG_RX_REMOTE , ID_11_BIT , 7 , 7 , receiveData , 8 , NO_INT , NO_INT };


State currentState = -1 ;
Direction currentDirection = STOP;
uint8 Keep_Alive = S_KEEP_ALIVE;
uint8 Switch1_flag = 0;
uint8 ADC_Reading = NORMAL_TEMPERATURE;
float Known_Voltage = KNOWN_VOLTAGE;
uint8 Remote_Frame = 0;
uint8 Switch1_Switch2_flag = 0;
uint8 Switch2_flag = 0;
uint8 Fault_Counter = 0;
uint8 flag1=0, flag2=0, flag3=0, flag4=0, flag5=0, flag6=1, flag7=0;
uint8 NO_COMMUNICATION_FLAG = 0;
uint8 OVERHEAT_FLAG = 0;
uint8 Fault_Counter_NOCOMM = 0;
uint8 Fault_Counter_OVERHEAT= 0;
uint8 ECU1State=0;
//extern Timer_ConfigType Timer0A ;


void Send_KeepAlive_Task(void)
{
  while (1)
  {
    if (flag1 == 0)
    {
      if(Keep_Alive == R_KEEP_ALIVE)
      {
        Keep_Alive = S_KEEP_ALIVE;
        transmitData[0] = Keep_Alive;
        CAN_configMsgObj(&msgObjTransmit);
        
        // CAN_configTransmitMessageObject ( Keep_Alive );
      }
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
      static uint32 No_Comm_Counter = 1;
      
      CAN_receiveMsgObj (  &msgObjReceive ) ;
      Keep_Alive =  receiveData[0];
      
      
      if (Keep_Alive == 0 && (No_Comm_Counter % COMMUNICATION_TIMEOUT) ==  0)
      {
        Fault_Counter = 0;
        currentState = BLUE;
        SemaphoreGive(SwitchStatesSempahore);
        //  while(1);
      }
      else
      {
        No_Comm_Counter++;
      }
      
      if (Keep_Alive == R_KEEP_ALIVE)
      {
        receiveData[0] = 0;
        currentState = GREEN;
        if (OVERHEAT_FLAG == 0 )
        {
          SemaphoreGive(SwitchStatesSempahore);
        }
        if (NO_COMMUNICATION_FLAG==1)
        {
          NO_COMMUNICATION_FLAG = 0;
          Fault_Counter_NOCOMM = 0;
          SemaphoreGive(SwitchStatesSempahore);
          
        }
        No_Comm_Counter = 1;
      }
      //Keep_Alive = (uint8)(CAN_configReceiveMessageObject());
      
    }
    OS_HoldTask(Receive_KeepAliveTask);
  }
}

void switchStates(void)
{
  
  while(1)
  {
    SemaphoreTake(SwitchStatesSempahore, switchStatesTask);
    // if (flag3 == 0)
    // {
    // currentState= uart_receiveByte(UART7);
    CAN_receiveMsgObj(&msgObjReceive);
    ECU1State = receiveData[5];
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
      flag6 = 0;
      transmitData[5] = currentState;
      CAN_configMsgObj(&msgObjTransmit);
      OS_ActivateTask(DTCTask);
      
      //   while(1);
      break;
    case GREEN:
      LED_write(RedLedPort, RedLedPin, LOGIC_LOW);
      LED_write(BlueLedPort, BlueLedPin, LOGIC_LOW);
      LED_write(GreenLedPort, GreenLedPin, LOGIC_HIGH);
      transmitData[5] = currentState;
      CAN_configMsgObj(&msgObjTransmit);
      EraseM_Block(DTC_Addr);
         OS_HoldTask(OverheatTask);
      break;
    case BLUE:
      LED_write(RedLedPort, RedLedPin, LOGIC_LOW);
      LED_write(BlueLedPort, BlueLedPin, LOGIC_HIGH);
      LED_write(GreenLedPort, GreenLedPin, LOGIC_LOW);
      OS_ActivateTask(No_CommunicationTask);
      
      break;
    default:
      currentState=GREEN;
      break;
      
    }
    //}
    //   OS_HoldTask(switchStatesTask);
  }
}


void Send_ADC_Reading(void)
{
  while(1)
  {
    
    if (flag4 == 0)
    {
      
      CAN_receiveMsgObj(&msgObjReceive);
      ECU1State = receiveData[5];
      CAN_receiveMsgObj(&msgObjReceiveRemote);
      // ADC_Reading = 15;
      /*
      if (!(SwitchIsPressed ( Switch1_port , Switch1_pin))){
      while (OS_GetTime() % 40 != 0); 
      if (!(SwitchIsPressed ( Switch1_port , Switch1_pin))){
      if (Switch1_flag ==0){
      Switch1_flag = 1 ;
      ADC_Reading = OVERHEAT;
      // currentState = RED;
      transmitData[4] = ADC_Reading;
      CAN_configMsgObj(&msgObjTransmit);
      
    }
    }
    }
      
      
      else{
      Switch1_flag = 0;
    }
      */
      // CAN_configMsgObj(&msgObjTransmit);
      ADC_Reading = LM35_Value();
      transmitData[4] = ADC_Reading;
      CAN_configMsgObj(&msgObjTransmit);
      
      
    }
    
    OS_HoldTask(Send_ADC_ReadingTask);
  }
}

void sendVoltage (void)
{
  while(1)
  {
    if (flag5 == 0)
    {
      
      CAN_receiveMsgObj(&msgObjReceive);
      CAN_receiveMsgObj(&msgObjReceiveRemote);
      Remote_Frame =  receiveData[7];
      Known_Voltage  = REF_Value();
      CAN_receiveMsgObj(&msgObjReceive);
      ECU1State = receiveData[5];
      // Remote_Frame = uart_receiveByte(UART1);
      if (Remote_Frame == 1)
      {
        transmitData[3] = Known_Voltage;
        CAN_configMsgObj(&msgObjTransmit);
      }
      if (ECU1State != GREEN)
      {
        if (ECU1State == RED)
        {
          currentState = RED;
          Remote_Frame = 0;
          DTC = RED;
           WriteM_Byte(DTC_Addr, DTC);
          SemaphoreGive(SwitchStatesSempahore);
        }
        else if (ECU1State == WHITE)
        {
          currentState = WHITE;
          Remote_Frame = 0;
          SemaphoreGive(SwitchStatesSempahore);
        }
      }
      else
      {
        currentState = GREEN;  
        Remote_Frame = 0;
        
        SemaphoreGive(SwitchStatesSempahore);
        
      }
      /*
      if (Remote_Frame == 1)
      {
      transmitData[3] = Known_Voltage;
      CAN_configMsgObj(&msgObjTransmit);
      transmitData[7] = Remote_Frame;
      CAN_configMsgObj(&msgObjTransmit);
    }
      CAN_receiveMsgObj(&msgObjReceive);
      ECU1State = receiveData[5];
      if (ECU1State == WHITE)
      {
      currentState=WHITE;
      Remote_Frame = 0;
      SemaphoreGive(SwitchStatesSempahore);
      
    }
      else if (ECU1State != WHITE && Remote_Frame == 1)
      {
      currentState = RED;
      Remote_Frame = 0;
      SemaphoreGive(SwitchStatesSempahore);
      
    }
      */
      CAN_receiveMsgObj(&msgObjReceive);
    }
    OS_HoldTask(sendVoltageTask);
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
      flag6=0;
  }
  else if(ReadM_Byte(DTC_Addr)== RED)
  {
    currentState = RED;
    LED_write(RedLedPort, RedLedPin, LOGIC_HIGH);
      LED_write(BlueLedPort, BlueLedPin, LOGIC_LOW);
      LED_write(GreenLedPort, GreenLedPin, LOGIC_LOW);
      flag6=0;
  }
  else if (ReadM_Byte(DTC_Addr)== WHITE)
  {
    currentState = WHITE;
    LED_write(RedLedPort, RedLedPin, LOGIC_HIGH);
    LED_write(BlueLedPort, BlueLedPin, LOGIC_HIGH);
    LED_write(GreenLedPort, GreenLedPin, LOGIC_HIGH);
    flag6=0;
  }
  else
  {
  currentState=GREEN;
  SemaphoreGive(SwitchStatesSempahore);
  OS_HoldTask(DTCTask);
  }
  
  
  
  while(1)
  {
    if (flag6 == 0)
    {
      
      uint8 OUT=0;
      
      static uint8 Send_Ready = 0;
      static uint8 Receive_Ready = 0;
      
      CAN_receiveMsgObj (  &msgObjReceive ) ;
      OUT= receiveData[5];
      if(currentState == RED)
      {
        flag1=1;
        flag2=1;
        flag3=1;
        flag4=1;
        flag5=1;
        
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
        
        //Receive_Ready = uart_receiveByte(UART4);
        
        if ((Send_Ready == READY && Receive_Ready == READY ))//|| OUT == GREEN )
        {
          currentState = GREEN ;
          flag1 = 0;
          flag2 = 0;
          flag3 = 0;
          flag4 = 0;
          flag5 = 0;
          flag6 = 1;
          Remote_Frame=0;
          transmitData[1] = Send_Ready;
          CAN_configMsgObj(&msgObjTransmit);
          //Known_Voltage = 30;
          ECU1State=0;
          for(int i =1; i<8 ; i++)
          {
            transmitData[i] = 0;
            
            receiveData[i] = 0;
          }
          Send_Ready = 0;
          Receive_Ready=0;
          //EraseM_Block(DTC_Addr);

          SemaphoreGive(SwitchStatesSempahore);
          OS_HoldTask(DTCTask);
          //  while (OS_GetTime() % 40 != 0);
          
        }
      }
      
      else if(currentState == BLUE)
      {
        flag1=1;
        flag2=1;
        flag3=1;
        flag4=1;
        
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
          
          // Temperature = 20;
          //Received_Voltage = KNOWN_VOLTAGE;
          currentState = GREEN ;
          flag1 = 0;
          flag2 = 0;
          flag3 = 0;
          flag4 = 0;
          flag5 = 0;
          flag6 = 1;
          transmitData[1] = Send_Ready;
          CAN_configMsgObj(&msgObjTransmit);
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
        flag5=1;
        
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
        
        
        if( (Send_Ready == READY && Receive_Ready == READY) || OUT == GREEN )
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
          transmitData[1] = Send_Ready;
          CAN_configMsgObj(&msgObjTransmit);
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
         transmitData[1] = Send_Ready;
              CAN_configMsgObj(&msgObjTransmit);
      }
      // OS_HoldTask(&task5);
      
    }
    // OS_HoldTask(&task6);
    
    
  }
}

void No_Communication_Task (void)
{
  OS_HoldTask(No_CommunicationTask);
  
  while(1)
  {
    NO_COMMUNICATION_FLAG = 1;
    // static uint8 Fault_Counter = 0;
    Fault_Counter_NOCOMM++;
    
    if (Fault_Counter_NOCOMM == 3)
    {
      Fault_Counter_NOCOMM = 0;
      DTC = BLUE;
      WriteM_Byte(DTC_Addr, DTC);
      flag6=0;
      currentState = BLUE;
      OS_ActivateTask(DTCTask);
      //OS_HoldTask(&task6);
    }
    if( Fault_Counter_NOCOMM !=3 )
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
    
   static uint8 GO_TO_DTC = 0;
   
    flag1=1;
    flag2=1;
    flag3=1;
    flag4=1;
    flag5=1;
    CAN_receiveMsgObj(&msgObjReceive);
    ECU1State = receiveData[5];
    CAN_receiveMsgObj(&msgObjReceive);
    GO_TO_DTC = receiveData[6];
    ADC_Reading = LM35_Value();
    transmitData[4] = ADC_Reading;
    CAN_configMsgObj(&msgObjTransmit);
    if (ECU1State == GREEN)
    {
      flag1=0;
      flag2=0;
      flag3=0;
      flag4=0;
      flag5=0;
      currentState= GREEN;
      CAN_receiveMsgObj(&msgObjReceive);
      GO_TO_DTC = receiveData[6];
      SemaphoreGive(SwitchStatesSempahore);
      
   
    }
    else if ( GO_TO_DTC == 1)
    {
         GO_TO_DTC = 0;
            flag6 = 0;
      DTC = WHITE;
       WriteM_Byte(DTC_Addr, DTC);
      OS_ActivateTask(DTCTask);
      //OS_HoldTask(OverheatTask);

    }
    
  }
}
  
  
  void CheckState_Task (void)
  {
    
    while(1)
    {
      if (flag7 == 0)
      {
        uint8 State = -1;
        CAN_receiveMsgObj(&msgObjReceive);
        State = receiveData[5];
        if (State== WHITE)
        {
          currentState = WHITE;
          SemaphoreGive(SwitchStatesSempahore);
        }
        
        
        OS_HoldTask(CheckStateTask);
      }
    }
  }
  
  
  