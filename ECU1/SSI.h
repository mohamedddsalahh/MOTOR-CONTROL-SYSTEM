 /******************************************************************************
 *
 * Module: SSI
 *
 * File Name: SSI.h
 *
 * Description: Header file for TM4C123GH6PM Microcontroller - SSI Driver.
 *
 * Author: Mohamed Salah Ibrahim
 ******************************************************************************/
#ifndef SSI_H_
#define SSI_H_

#include "tm4c123gh6pm.h"
#include "types.h"
#include "bitwise_operation.h"

#define SSI0_BASE    (0x40008000UL)
#define SSI1_BASE    (0x40009000UL)
#define SSI2_BASE    (0x4000A000UL)
#define SSI3_BASE    (0x4000B000UL)

/**SSI Base Structure**/
typedef struct{
  uint32        SSICR0;
  uint32        SSICR1;
  uint32        SSIDR;
  uint32        SSISR;
  uint32        SSICPSR;
  uint32        SSIIM;
  uint32        SSIRIS;
  uint32        SSIIMIS;
  uint32        SSIICR;
  uint32        SSIDMACTL;
  //uint32        SSICC;
}SSI_ConfigType;

/**@ref Specify Mode**/
typedef enum{
  SSI_MASTER = 0x00000000,
  SSI_SLAVE_OUTPUT_Enabled = 0x00000004,
  SSI_SLAVE_OUTPUT_Disabled = 0x0000000C,
}MODE_t;

/**@ref SSI Configuration Structure**/
typedef struct{
  
  uint8          SSI_ID;                 //Specify SSI ID @ref SSI ID
  MODE_t         Mode;                   //Specify SSI Mode @ref Specify Mode 
  uint8          Clk_Src;                //Specify SSI CLK SRC @ref SSI CLock Source
  uint32         Transmission_Freq;      //Specify SSI Transmission Frequency @ref SSI Transmission Frequency
  uint8          Clk_Prescaler;          //Specify SSI CLK Prescaler @ref SSI CLock Prescaler
  uint8          Clk_Phase;              //Specify CLK Phase @ref CLK Phase
  uint8          Clk_Polarity;           //Specify CLK Phase @ref CLK Polarity
  uint8          ProtocolMode;           //Specify CLK Phase @ref Protocol Mode
  uint8          DataSize;               //Specify CLK Phase @ref Data Size
  
}SSI_Config_t;

/*---------------------------*/
/*********Definitions*********/
/**SSI Peripherals**/
#define SSI0    (SSI_ConfigType*)SSI0_BASE
#define SSI1    (SSI_ConfigType*)SSI1_BASE
#define SSI2    (SSI_ConfigType*)SSI2_BASE
#define SSI3    (SSI_ConfigType*)SSI3_BASE

/**SSI CLock Control**/
#define SSI0_CLK_EN()     SYSCTL_RCGCSSI_R |= (1<<0);
#define SSI1_CLK_EN()     SYSCTL_RCGCSSI_R |= (1<<1);
#define SSI2_CLK_EN()     SYSCTL_RCGCSSI_R |= (1<<2);
#define SSI3_CLK_EN()     SYSCTL_RCGCSSI_R |= (1<<3);

/** Clock Source Control **/
#define SYSCLK_EN_SII0()  SSI0_CC_R = 0;
#define SYSCLK_EN_SII1()  SSI1_CC_R = 0;
#define SYSCLK_EN_SII2()  SSI2_CC_R = 0;
#define SYSCLK_EN_SII3()  SSI3_CC_R = 0;

#define PIOSC_EN_SII0()   SSI0_CC_R = 0x5;
#define PIOSC_EN_SII1()   SSI1_CC_R = 0x5;
#define PIOSC_EN_SII2()   SSI2_CC_R = 0x5;
#define PIOSC_EN_SII3()   SSI3_CC_R = 0x5;


/**@ref SSI ID**/
#define SSI0_ID    0            
#define SSI1_ID    1
#define SSI2_ID    2
#define SSI3_ID    3


/**@ref SSI CLock Source**/
#define SSI_SysClk       0x0
#define SSI_PIOSC        0x5

/**@ref CLK Phase**/
//0 Data is captured on the first clock edge transition.
//1 Data is captured on the second clock edge transition
#define DATA_CAP_FIRST_EDGE     0x0
#define DATA_CAP_SECOND_EDGE    0x1

/**@ref CLK Polarity**/
//0 A steady state Low value is placed on the SSInClk pin.
//1 A steady state High value is placed on the SSInClk pin when
//data is not being transferred.
/*Note: 0 A steady state Low value is placed on the SSInClk pin.
A steady state High value is placed on the SSInClk pin when
data is not being transferred.*/
#define CLK_SS_LOW      0x0
#define CLK_SS_HIGH     0x1     /**Not Supported Yet**/

/**@ref Protocol Mode**/
//0x0 Freescale SPI Frame Format
//0x1 Texas Instruments Synchronous Serial Frame Format
//0x2 MICROWIRE Frame Format
//0x3 Reserved
#define FreeScale_SPI                    0x0
#define TexasInstruments_Sync_Serial     0x1    /**Not Supported Yet**/
#define MICROWIRE                        0x2    /**Not Supported Yet**/

/**@ref Data Size**/
#define SSI_DATA_SIZE_4Bits      0x3
#define SSI_DATA_SIZE_5Bits      0x4
#define SSI_DATA_SIZE_6Bits      0x5
#define SSI_DATA_SIZE_7Bits      0x6
#define SSI_DATA_SIZE_8Bits      0x7
#define SSI_DATA_SIZE_9Bits      0x8
#define SSI_DATA_SIZE_10Bits     0x9
#define SSI_DATA_SIZE_11Bits     0xA
#define SSI_DATA_SIZE_12Bits     0xB
#define SSI_DATA_SIZE_13Bits     0xC
#define SSI_DATA_SIZE_14Bits     0xD
#define SSI_DATA_SIZE_15Bits     0xE
#define SSI_DATA_SIZE_16Bits     0xF

/**@ref SSI Transmission Frequency**/
#define SSI_Transmission_FREQ_1MHz  1000000UL
#define SSI_Transmission_FREQ_2MHz  2000000UL

/**@ref SSI CLock Prescaler**/
#define SSI_Clk_Prescaler_2         2UL
#define SSI_Clk_Prescaler_4         4UL
#define SSI_Clk_Prescaler_8         8UL

/**@ref SSI Polling**/
typedef enum{
  SSI_Polling_Disable,  /*Disable SSI POLLING*/
  SSI_Polling_Enable    /*Enable SSI POLLING*/
}SSI_Polling;

/***************************************************/
/************      APIs Definitions     ************/
/***************************************************/
/* To use this function: refer to @ref SSI Configuration Structure */
void SSI_Init(SSI_Config_t* SSIConfig);

/* To use this function: Pass the value to be sent and polling state refer to @ref SSI Polling */
void SSI_Send(uint16 Value, SSI_Polling Polling);

/* To use this function: Pass the polling state refer to @ref SSI Polling */
uint16 SSI_Recieve(SSI_Polling Polling);

#endif