/******************************************************************************
*
* Module: SSI
*
* File Name: SSI.c
*
* Description: Source file for TM4C123GH6PM Microcontroller - SSI Driver.
*
* Author: Mohamed Salah Ibrahim
******************************************************************************/
#include "SSI.h"

static SSI_ConfigType* SSI_Ptr;
uint8 dataSize = 0;
uint8 SSI_Id;
/**SSI GPIO Pin Configuration Auxillary Function**/
void GPIO_SSI_CTL(uint8 SSI_ID, MODE_t Mode)
{
  switch(SSI_ID){
  case SSI0_ID: //Enable PORTA PA2,3,4,5
    SYSCTL_RCGCGPIO_R |= (1<<0);
    GPIO_PORTA_AFSEL_R |= (0x0F<<2);
    GPIO_PORTA_PCTL_R |= 0x00222200;
    GPIO_PORTA_CR_R |= (0x0F << 2);
    GPIO_PORTA_DEN_R|= 0x0F << 2;
    /**Control SS Pin**/
    if (Mode == SSI_MASTER){
      GPIO_PORTA_DIR_R |= (1<<3);
    }
    else {
      GPIO_PORTA_DIR_R &= ~(1<<3);
    }
    break;
  case SSI1_ID: //Enable PORTF  PF0,1,2,3
    SYSCTL_RCGCGPIO_R |= (1<<5);
    GPIO_PORTF_AFSEL_R |= (0x0F<<0);
    GPIO_PORTF_PCTL_R |= 0x00002222;
    GPIO_PORTF_DEN_R |= 0x0F << 0;
    /**Control SS Pin**/
    if (Mode == SSI_MASTER){
      GPIO_PORTF_DIR_R |= (1<<3);
    }
    else {
      GPIO_PORTF_DIR_R &= ~(1<<3);
    }
    break;
  case SSI2_ID: //Enable PORTB PB4,5,6,7
    SYSCTL_RCGCGPIO_R |= (1<<1);
    GPIO_PORTB_AFSEL_R |= (0x0F<<4);
    GPIO_PORTB_PCTL_R |= 0x22220000;
    GPIO_PORTB_DEN_R |= 0x0F << 4;
    /**Control SS Pin**/
    if (Mode == SSI_MASTER){
      GPIO_PORTB_DIR_R |= (1<<5);
    }
    else {
      GPIO_PORTB_DIR_R &= ~(1<<5);
    }
    break;
  case SSI3_ID: //Enable PORTD PD0,1,2,3
    SYSCTL_RCGCGPIO_R |= (1<<3);
    GPIO_PORTD_AFSEL_R |= (0x0F<<0);
    GPIO_PORTD_PCTL_R |= 0x00001111;
    GPIO_PORTD_DEN_R |= 0x0F << 0;
    /**Control SS Pin**/
    if (Mode == SSI_MASTER){
      GPIO_PORTD_DIR_R |= (1<<1);
    }
    else {
      GPIO_PORTD_DIR_R &= ~(1<<1);
    }
    break;      
  }
}
/***************************************************/
/************       Module APIs         ************/
/***************************************************/
void SSI_Init(SSI_Config_t* SSIConfig){
  uint8 SCR = 0;
  switch(SSIConfig->SSI_ID){
  case SSI0_ID:
    SSI_Ptr = SSI0;
    SSI_Id  = SSI0_ID;
    SSI0_CLK_EN();
    GPIO_SSI_CTL(SSI0_ID, SSIConfig->Mode);
    break;
  case SSI1_ID:
    SSI_Ptr = SSI1;
    SSI_Id  = SSI1_ID;
    SSI1_CLK_EN();
    GPIO_SSI_CTL(SSI1_ID, SSIConfig->Mode);
    break;
  case SSI2_ID:
    SSI_Ptr = SSI2;
    SSI_Id  = SSI2_ID;
    SSI2_CLK_EN();
    GPIO_SSI_CTL(SSI2_ID, SSIConfig->Mode);
    break;
  case SSI3_ID:
    SSI_Ptr = SSI3;
    SSI_Id  = SSI3_ID;
    SSI3_CLK_EN();
    GPIO_SSI_CTL(SSI3_ID, SSIConfig->Mode);
    break;
  }
  SSI_Ptr->SSICR1 = 0x0;         //Disable SSI
  SSI_Ptr->SSICR1 |= SSIConfig->Mode;
 /**Enable Clock Source**/
  if((SSIConfig->Clk_Src) == SSI_SysClk){
    switch ((SSIConfig->SSI_ID)){
    case SSI0_ID:
      SYSCLK_EN_SII0();
      break;
    case SSI1_ID:
      SYSCLK_EN_SII1();
      break;
    case SSI2_ID:
      SYSCLK_EN_SII2();
      break;
    case SSI3_ID:
      SYSCLK_EN_SII3();
      break;
    }
  }else{
    switch ((SSIConfig->SSI_ID)){
    case SSI0_ID:
      PIOSC_EN_SII0();
      break;
    case SSI1_ID:
      PIOSC_EN_SII1();
      break;
    case SSI2_ID:
      PIOSC_EN_SII2();
      break;
    case SSI3_ID:
      PIOSC_EN_SII3();
      break;
    }
  }
  /**Configure Prescaler**/
  SSI_Ptr->SSICPSR |= SSIConfig->Clk_Prescaler;
  /**Configure SCR**/
  SCR = (uint8)(((16000000)/(SSIConfig->Transmission_Freq))/(SSIConfig->Clk_Prescaler)) - 1;
  SSI_Ptr->SSICR0 |= (SCR << 8);
  SSI_Ptr->SSICR0 |= ((SSIConfig->Clk_Phase)<<7);
  SSI_Ptr->SSICR0 |= ((SSIConfig->Clk_Polarity)<<6);
  SSI_Ptr->SSICR0 |= ((SSIConfig->ProtocolMode)<<4);
  SSI_Ptr->SSICR0 |= (SSIConfig->DataSize);
  
  SSI_Ptr->SSICR1 |= 0x2;        //Enable SSI
  dataSize = SSIConfig->DataSize;
}

void SSI_Send(uint16 Value, SSI_Polling Polling){
  switch(SSI_Id){
  case SSI0_ID:
    GPIO_PORTA_DATA_R &= ~(1<<3);
    break;
  case SSI1_ID:
    GPIO_PORTF_DATA_R &= ~(1<<3);
    break;
  case SSI2_ID:
    GPIO_PORTB_DATA_R &= ~(1<<5);
    break;
  case SSI3_ID:
    GPIO_PORTD_DATA_R &= ~(1<<1);
    break;
  }
  if (Polling){
    //while(!(SSI_Ptr->SSISR & 0x01));
    switch(dataSize){
    case SSI_DATA_SIZE_4Bits:
      SSI_Ptr->SSIDR = (uint8)(Value & 0xF);
      break;
    case SSI_DATA_SIZE_5Bits:
      SSI_Ptr->SSIDR = (uint8)(Value & 0x1F);
      break;
    case SSI_DATA_SIZE_6Bits:
      SSI_Ptr->SSIDR = (uint8)(Value & 0x3F);
      break;
    case SSI_DATA_SIZE_7Bits:
      SSI_Ptr->SSIDR = (uint8)(Value & 0x7F);
      break;
    case SSI_DATA_SIZE_8Bits:
      SSI_Ptr->SSIDR = (uint8)(Value & 0xFF);
      break;
    case SSI_DATA_SIZE_9Bits:
      SSI_Ptr->SSIDR = (uint16)(Value & 0x1FF);
      break;
    case SSI_DATA_SIZE_10Bits:
      SSI_Ptr->SSIDR = (uint16)(Value & 0x3FF);
      break;
    case SSI_DATA_SIZE_11Bits:
      SSI_Ptr->SSIDR = (uint16)(Value & 0x7FF);
      break;
    case SSI_DATA_SIZE_12Bits:
      SSI_Ptr->SSIDR = (uint16)(Value & 0xFFF);
      break;
    case SSI_DATA_SIZE_13Bits:
      SSI_Ptr->SSIDR = (uint16)(Value & 0x1FFF);
      break;
    case SSI_DATA_SIZE_14Bits:
      SSI_Ptr->SSIDR = (uint16)(Value & 0x3FFF);
      break;
    case SSI_DATA_SIZE_15Bits:
      SSI0_SR_R = (uint16)(Value & 0x7FFF);
      break;
    case SSI_DATA_SIZE_16Bits:
      SSI0_SR_R = (uint16)(Value & 0xFFFF);
      break;
    } 
   // while((SSI_Ptr->SSISR & 0x10));
  }
  switch(SSI_Id){
  case SSI0_ID:
    GPIO_PORTA_DATA_R |= (1<<3);
    break;
  case SSI1_ID:
    GPIO_PORTF_DATA_R |= (1<<3);
    break;
  case SSI2_ID:
    GPIO_PORTB_DATA_R |= (1<<5);
    break;
  case SSI3_ID:
    GPIO_PORTD_DATA_R |= (1<<1);
    break;
  }
}

uint16 SSI_Recieve(SSI_Polling Polling){
  
  if(Polling){
    while(!(SSI_Ptr->SSISR & 0x04));
    switch(dataSize){
    case SSI_DATA_SIZE_4Bits:
      return (uint8)((SSI_Ptr->SSIDR) & 0xF);
      break;
    case SSI_DATA_SIZE_5Bits:
      return (uint8)((SSI_Ptr->SSIDR) & 0x1F);
      break;
    case SSI_DATA_SIZE_6Bits:
      return (uint8)((SSI_Ptr->SSIDR) & 0x3F);
      break;
    case SSI_DATA_SIZE_7Bits:
      return (uint8)((SSI_Ptr->SSIDR) & 0x7F);
      break;
    case SSI_DATA_SIZE_8Bits:
      return (uint8)((SSI_Ptr->SSIDR) & 0xFF);
      break;
    case SSI_DATA_SIZE_9Bits:
      return (uint16)((SSI_Ptr->SSIDR) & 0x1FF);
      break;
    case SSI_DATA_SIZE_10Bits:
      return (uint16)((SSI_Ptr->SSIDR) & 0x3FF);
      break;
    case SSI_DATA_SIZE_11Bits:
      return (uint16)((SSI_Ptr->SSIDR) & 0x7FF);
      break;
    case SSI_DATA_SIZE_12Bits:
      return (uint16)((SSI_Ptr->SSIDR) & 0xFFF);
      break;
    case SSI_DATA_SIZE_13Bits:
      return (uint16)((SSI_Ptr->SSIDR) & 0x1FFF);
      break;
    case SSI_DATA_SIZE_14Bits:
      return (uint16)((SSI_Ptr->SSIDR) & 0x3FFF);
      break;
    case SSI_DATA_SIZE_15Bits:
      return (uint16)((SSI_Ptr->SSIDR) & 0x7FFF);
      break;
    case SSI_DATA_SIZE_16Bits:
      return (uint16)((SSI_Ptr->SSIDR) & 0xFFFF);
      break;
    }
    while((SSI_Ptr->SSISR & 0x10));
  }
}
