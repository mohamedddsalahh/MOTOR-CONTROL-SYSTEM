#include "CAN.h"


void GPIO_initCan ( void )
{
  
  SYSCTL_RCGCCAN_R |= ( STD_HIGH << CAN0_CLOCK );
  SYSCTL_RCGCCAN_R |= ( STD_HIGH << CAN1_CLOCK );
  /** Enable Clock for port **/
  SYSCTL_RCGCGPIO_R |= ( STD_HIGH << CAN0_PORT );
  /** Configure pin 4 and pin 5 for digital use **/
  GPIO_PORTB_DEN_R |= ( STD_HIGH << CAN0_RX ) ;
  GPIO_PORTB_DEN_R |= ( STD_HIGH << CAN0_TX ) ;
  /** set Alternitive function bits to use pin4 and pin5 for other function instead of I/O **/
  GPIO_PORTB_AFSEL_R |= ( STD_HIGH << CAN0_RX ) ;
  GPIO_PORTB_AFSEL_R |= ( STD_HIGH << CAN0_TX ) ; 
  /** Set PMC value = 8 for can 0 rx and tx **/
  GPIO_PORTB_PCTL_R |= ( CAN_PMC_V << PIN4_PMC ) ;
  GPIO_PORTB_PCTL_R |= ( CAN_PMC_V << PIN5_PMC ) ;
  /** Set amber for 8 mAmber **/
  GPIO_PORTB_DR8R_R = GPIO_PORTB_DR8R ;
  /** Enable Clock for port **/    
  SYSCTL_RCGCGPIO_R |= ( STD_HIGH << CAN0_PORT );
  /** Configure pin 0 and pin 1 for digital use **/
  
  
}


void CAN_init ( const CAN_configType * Config_canInit )
{
  /** Enter Initiation Mode **/
  REG ( Config_canInit->can + CANCTL_R ) = ( STD_HIGH << CANCTL_INIT ) ;
  
  /** Set Can Mode **/
  /**----------------------------------------------------------------------------------------------->**/
  REG ( Config_canInit->can + CANTST_R ) |= ( STD_HIGH << Config_canInit->mode ) ;
  
  REG ( Config_canInit->can + CANCTL_R ) |= ( STD_HIGH << CANCTL_CCE) ;
  /** Reset bit timing Register **/
  REG ( Config_canInit->can  + CANBIT_R ) = RESET_REG ;
  
  /** Set bit timing **/
  REG ( Config_canInit->can  + CANBIT_R ) = 0x34F1 ; 
  
  /** BRP Extension Set **/
  REG ( Config_canInit->can  + CANBRPE_R ) = RESET_REG ; 
  REG ( Config_canInit->can  + CANBRPE_R ) = ( Config_canInit->bitTiming.brpe >>  CANBRPE_BRPE ) ;
  
  REG ( Config_canInit->can + CANCTL_R ) &= CANCTL_CCE_OFF ;
  
}


void CAN_configMsgObj ( const CAN_msgObj * Config_msgObj )
{
  /** Read/Write Check **/
  while ( REG ( Config_msgObj->can + CANIF1CRQ_R ) & CANIF1CRQ_BUSY ) {};
  
  
  
  /** Clear data registers **/
  
  uint8 shift = 0 ;
  for ( uint8 dataByte = 0 ; dataByte < MAX_DATA/2 ; dataByte++ ) 
  {
    REG ( Config_msgObj->can + CANIF1DA1_R + shift ) = RESET_REG ;
    shift += shitfDataReg ; 
  }
  
  /** Set data to data registers **/
  REG ( Config_msgObj->can + CANIF1DA1_R  ) = Config_msgObj->data[0] | ( Config_msgObj->data[1] << SEC_BYTE ) ;
  REG ( Config_msgObj->can + CANIF1DA2_R  ) = Config_msgObj->data[2] | ( Config_msgObj->data[3] << SEC_BYTE ) ;
  REG ( Config_msgObj->can + CANIF1DB1_R  ) = Config_msgObj->data[4] | ( Config_msgObj->data[5] << SEC_BYTE ) ;
  REG ( Config_msgObj->can + CANIF1DB2_R  ) = Config_msgObj->data[6] | ( Config_msgObj->data[7] << SEC_BYTE ) ;
  
  /** Reset command mask register **/
  REG ( Config_msgObj->can + CANIF1CMSK_R ) = RESET_REG ; 
  
  switch ( Config_msgObj->canMsgType )
  {
  case MSG_TX : 
    /** set the WRND bit to transfer data from Interfernce register to the CAN object **/
    REG ( Config_msgObj->can + CANIF1CMSK_R ) = ( STD_HIGH << CANIFCMSK_WRNRD ) ;
    
    /** transfer data bytes 0-3 in message object to CANIFnDA1 and CANIFnDA2. **/
    REG ( Config_msgObj->can + CANIF1CMSK_R ) |= ( STD_HIGH << CANIFCMSK_DATAA ) ;
    
    /** transfer data bytes 4-7 in message object to CANIFnDA1 and CANIFnDA2. **/
    REG ( Config_msgObj->can + CANIF1CMSK_R ) |= ( STD_HIGH << CANIFCMSK_DATAB ) ;
    
    /**Transfer control bits from the CANIFnMCTL register into the Interface registers    **/
    REG ( Config_msgObj->can + CANIF1CMSK_R ) |= ( STD_HIGH << CANIFCMSK_CONTROL ) ;
    
    /** Transfer IDMASK + DIR + MXTD of the message object into the Interface registers **/
    REG ( Config_msgObj->can + CANIF1CMSK_R ) |= ( STD_HIGH << CANIFCMSK_ARB ) ;
    
    /** Set unmask bit to allow acceptance filtering **/
    REG ( Config_msgObj->can + CANIF1CMSK_R ) |= ( STD_HIGH << CANIFCMSK_MASK ) ;
    
    if ( Config_msgObj->IDLen == ID_29_BIT )
    {
      /** Set mask bits for 29 bit ID **/
      REG ( Config_msgObj->can + CANIF1MSK1_R ) = RESET_REG ;
      REG ( Config_msgObj->can + CANIF1MSK1_R ) = Config_msgObj->msgIDMask ;
      
      REG ( Config_msgObj->can + CANIF1MSK2_R ) &= CANIF1MSK2_RE ;
      REG ( Config_msgObj->can + CANIF1MSK2_R ) |= ( Config_msgObj->msgIDMask >> MASK_EXT ) ;
      
      REG ( Config_msgObj->can + CANIF1MSK2_R ) |= ( STD_HIGH  << CANIFMSK2_MDIR ) ;
      REG ( Config_msgObj->can + CANIF1MSK2_R ) |= ( STD_HIGH  << CANIFMSK2_MXTD ) ;
      
      /** Set ID **/
      REG ( Config_msgObj->can + CANIF1ARB1_R ) = RESET_REG ;
      REG ( Config_msgObj->can + CANIF1ARB1_R ) = Config_msgObj->msgID ; 
      
      REG ( Config_msgObj->can + CANIF1ARB2_R ) &= CANIF1ARB2_RE ;
      REG ( Config_msgObj->can + CANIF1ARB2_R ) |= ( Config_msgObj->msgID >> ID_EXT ) ;  
      REG ( Config_msgObj->can + CANIF1ARB2_R ) |= ( STD_HIGH << CANIFARB2_XTD ) ;
      
    }
    else if ( Config_msgObj->IDLen == ID_11_BIT )
    {
      /** Set mask bits for 11 bit ID **/
      REG ( Config_msgObj->can + CANIF1MSK2_R ) &= CANIF1MSK2_RE ;
      REG ( Config_msgObj->can + CANIF1MSK2_R ) |= ( Config_msgObj->msgIDMask <<  MASK ) ;
      
      REG ( Config_msgObj->can + CANIF1MSK2_R ) |= ( STD_HIGH  << CANIFMSK2_MDIR ) ;
      REG ( Config_msgObj->can + CANIF1MSK2_R ) |= ( STD_HIGH  << CANIFMSK2_MXTD ) ;
      
      /** Set 11 bit id **/
      REG ( Config_msgObj->can + CANIF1ARB2_R ) &= CANIF1ARB2_RE ;
      REG ( Config_msgObj->can + CANIF1ARB2_R ) |= ( Config_msgObj->msgID << ID ) ;  
      
      
    }
    
    REG ( Config_msgObj->can + CANIF1ARB2_R ) |= ( STD_HIGH << CANIFARB2_MGSVAL ) ;
    REG ( Config_msgObj->can + CANIF1ARB2_R ) |= ( STD_HIGH << CANIFARB2_DIR ) ;
    
    REG ( Config_msgObj->can + CANIF1MCTL_R ) |= ( STD_HIGH << CANIFMCTL_EOB ) ;    
    REG ( Config_msgObj->can + CANIF1MCTL_R ) |= ( STD_HIGH << CANIFMCTL_TXRQST) ;
    
    REG ( Config_msgObj->can + CANIF1MCTL_R ) |= ( STD_HIGH << CANIFMCTL_UMASK ) ;
    
    /** Enable Interrupt **/ 
    if ( Config_msgObj->intTx == INT )
    {
      REG ( Config_msgObj->can + CANIF1MCTL_R ) |= ( STD_HIGH << CANIFMCTL_TXIE ) ;
    }
    
    /** Set Data size **/
    REG ( Config_msgObj->can + CANIF1MCTL_R ) &= CANIF1MCTL_DLC ;
    REG ( Config_msgObj->can + CANIF1MCTL_R ) |= Config_msgObj->dataSize ; 
    
    
    break ;
  case MSG_RX : 
    /** set the WRND bit to transfer data from Interfernce register to the CAN object **/
    REG ( Config_msgObj->can + CANIF1CMSK_R ) = ( STD_HIGH << CANIFCMSK_WRNRD ) ;
    
    /** transfer data bytes 0-3 in message object to CANIFnDA1 and CANIFnDA2. **/
    REG ( Config_msgObj->can + CANIF1CMSK_R ) |= ( STD_HIGH << CANIFCMSK_DATAA ) ;
    
    /** transfer data bytes 4-7 in message object to CANIFnDA1 and CANIFnDA2. **/
    REG ( Config_msgObj->can + CANIF1CMSK_R ) |= ( STD_HIGH << CANIFCMSK_DATAB ) ;
    
    /**Transfer control bits from the CANIFnMCTL register into the Interface registers    **/
    REG ( Config_msgObj->can + CANIF1CMSK_R ) |= ( STD_HIGH << CANIFCMSK_CONTROL ) ;
    
    /** Transfer IDMASK + DIR + MXTD of the message object into the Interface registers **/
    REG ( Config_msgObj->can + CANIF1CMSK_R ) |= ( STD_HIGH << CANIFCMSK_ARB ) ;
    
    /** Set unmask bit to allow acceptance filtering **/
    REG ( Config_msgObj->can + CANIF1CMSK_R ) |= ( STD_HIGH << CANIFCMSK_MASK ) ;
    
    if ( Config_msgObj->IDLen == ID_29_BIT )
    {
      /** Set mask bits for 29 bit ID **/
      REG ( Config_msgObj->can + CANIF1MSK1_R ) = RESET_REG ;
      REG ( Config_msgObj->can + CANIF1MSK1_R ) = Config_msgObj->msgIDMask ;
      
      REG ( Config_msgObj->can + CANIF1MSK2_R ) &= CANIF1MSK2_RE ;
      REG ( Config_msgObj->can + CANIF1MSK2_R ) |= ( Config_msgObj->msgIDMask >> MASK_EXT ) ;
      
      REG ( Config_msgObj->can + CANIF1MSK2_R ) |= ( STD_HIGH  << CANIFMSK2_MDIR ) ;
      REG ( Config_msgObj->can + CANIF1MSK2_R ) |= ( STD_HIGH  << CANIFMSK2_MXTD ) ;
      
      /** Set ID **/
      REG ( Config_msgObj->can + CANIF1ARB1_R ) = RESET_REG ;
      REG ( Config_msgObj->can + CANIF1ARB1_R ) = Config_msgObj->msgID ; 
      
      REG ( Config_msgObj->can + CANIF1ARB2_R ) &= CANIF1ARB2_RE ;
      REG ( Config_msgObj->can + CANIF1ARB2_R ) |= ( Config_msgObj->msgID >> ID_EXT ) ;  
      REG ( Config_msgObj->can + CANIF1ARB2_R ) |= ( STD_HIGH << CANIFARB2_XTD ) ;
      
    }
    else if ( Config_msgObj->IDLen == ID_11_BIT )
    {
      /** Set mask bits for 11 bit ID **/
      REG ( Config_msgObj->can + CANIF1MSK2_R ) &= CANIF1MSK2_RE ;
      REG ( Config_msgObj->can + CANIF1MSK2_R ) |= ( Config_msgObj->msgIDMask <<  MASK ) ;
      
      REG ( Config_msgObj->can + CANIF1MSK2_R ) |= ( STD_HIGH  << CANIFMSK2_MDIR ) ;
      REG ( Config_msgObj->can + CANIF1MSK2_R ) |= ( STD_HIGH  << CANIFMSK2_MXTD ) ;
      
      /** Set 11 bit id **/
      REG ( Config_msgObj->can + CANIF1ARB2_R ) &= CANIF1ARB2_RE ;
      REG ( Config_msgObj->can + CANIF1ARB2_R ) |= ( Config_msgObj->msgID << ID ) ;  
      
      
    }
    
    REG ( Config_msgObj->can + CANIF1ARB2_R ) |= ( STD_HIGH << CANIFARB2_MGSVAL ) ;
    
    REG ( Config_msgObj->can + CANIF1ARB2_R ) &= CANIF1ARB2_RX ;
    
    
    
    REG ( Config_msgObj->can + CANIF1MCTL_R ) |= ( STD_HIGH << CANIFMCTL_TXRQST) ;
    
    REG ( Config_msgObj->can + CANIF1MCTL_R ) |= ( STD_HIGH << CANIFMCTL_EOB ) ;    
    
    
    REG ( Config_msgObj->can + CANIF1MCTL_R ) |= ( STD_HIGH << CANIFMCTL_UMASK ) ;
    
    /** Enable Interrupt **/ 
    if ( Config_msgObj->intRx == INT )
    {
      REG ( Config_msgObj->can + CANIF1MCTL_R ) |= ( STD_HIGH << CANIFMCTL_RXIE ) ;
    }
    
    /** Set Data size **/
    REG ( Config_msgObj->can + CANIF1MCTL_R ) &= CANIF1MCTL_DLC ;
    REG ( Config_msgObj->can + CANIF1MCTL_R ) |= Config_msgObj->dataSize ; 
    break ;
    
  case MSG_TX_REMOTE :
    /** set the WRND bit to transfer data from Interfernce register to the CAN object **/
    REG ( Config_msgObj->can + CANIF1CMSK_R ) = ( STD_HIGH << CANIFCMSK_WRNRD ) ;
    
    /** transfer data bytes 0-3 in message object to CANIFnDA1 and CANIFnDA2. **/
    REG ( Config_msgObj->can + CANIF1CMSK_R ) |= ( STD_HIGH << CANIFCMSK_DATAA ) ;
    
    /** transfer data bytes 4-7 in message object to CANIFnDA1 and CANIFnDA2. **/
    REG ( Config_msgObj->can + CANIF1CMSK_R ) |= ( STD_HIGH << CANIFCMSK_DATAB ) ;
    
    /**Transfer control bits from the CANIFnMCTL register into the Interface registers    **/
    REG ( Config_msgObj->can + CANIF1CMSK_R ) |= ( STD_HIGH << CANIFCMSK_CONTROL ) ;
    
    /** Transfer IDMASK + DIR + MXTD of the message object into the Interface registers **/
    REG ( Config_msgObj->can + CANIF1CMSK_R ) |= ( STD_HIGH << CANIFCMSK_ARB ) ;
    
    /** Set unmask bit to allow acceptance filtering **/
    REG ( Config_msgObj->can + CANIF1CMSK_R ) |= ( STD_HIGH << CANIFCMSK_MASK ) ;
    
    if ( Config_msgObj->IDLen == ID_29_BIT )
    {
      /** Set mask bits for 29 bit ID **/
      REG ( Config_msgObj->can + CANIF1MSK1_R ) = RESET_REG ;
      REG ( Config_msgObj->can + CANIF1MSK1_R ) = Config_msgObj->msgIDMask ;
      
      REG ( Config_msgObj->can + CANIF1MSK2_R ) &= CANIF1MSK2_RE ;
      REG ( Config_msgObj->can + CANIF1MSK2_R ) |= ( Config_msgObj->msgIDMask >> MASK_EXT ) ;
      
      REG ( Config_msgObj->can + CANIF1MSK2_R ) |= ( STD_HIGH  << CANIFMSK2_MDIR ) ;
      REG ( Config_msgObj->can + CANIF1MSK2_R ) |= ( STD_HIGH  << CANIFMSK2_MXTD ) ;
      
      /** Set ID **/
      REG ( Config_msgObj->can + CANIF1ARB1_R ) = RESET_REG ;
      REG ( Config_msgObj->can + CANIF1ARB1_R ) = Config_msgObj->msgID ; 
      
      REG ( Config_msgObj->can + CANIF1ARB2_R ) &= CANIF1ARB2_RE ;
      REG ( Config_msgObj->can + CANIF1ARB2_R ) |= ( Config_msgObj->msgID >> ID_EXT ) ;  
      REG ( Config_msgObj->can + CANIF1ARB2_R ) |= ( STD_HIGH << CANIFARB2_XTD ) ;
      
    }
    else if ( Config_msgObj->IDLen == ID_11_BIT )
    {
      /** Set mask bits for 11 bit ID **/
      REG ( Config_msgObj->can + CANIF1MSK2_R ) &= CANIF1MSK2_RE ;
      REG ( Config_msgObj->can + CANIF1MSK2_R ) |= ( Config_msgObj->msgIDMask <<  MASK ) ;
      
      REG ( Config_msgObj->can + CANIF1MSK2_R ) |= ( STD_HIGH  << CANIFMSK2_MDIR ) ;
      REG ( Config_msgObj->can + CANIF1MSK2_R ) |= ( STD_HIGH  << CANIFMSK2_MXTD ) ;
      
      /** Set 11 bit id **/
      REG ( Config_msgObj->can + CANIF1ARB2_R ) &= CANIF1ARB2_RE ;
      REG ( Config_msgObj->can + CANIF1ARB2_R ) |= ( Config_msgObj->msgID << ID ) ;  
      
      
    }
    
    REG ( Config_msgObj->can + CANIF1ARB2_R ) |= ( STD_HIGH << CANIFARB2_MGSVAL ) ;
    
    REG ( Config_msgObj->can + CANIF1ARB2_R ) &= CANIF1ARB2_TX_REMOTE ;
    
    REG ( Config_msgObj->can + CANIF1MCTL_R ) |= ( STD_HIGH << CANIFMCTL_EOB ) ; 
    
    REG ( Config_msgObj->can + CANIF1MCTL_R ) |= ( STD_HIGH << CANIFMCTL_TXRQST) ;
    
    REG ( Config_msgObj->can + CANIF1MCTL_R ) |= ( STD_HIGH << CANIFMCTL_UMASK ) ;
    
    /** Enable Interrupt **/ 
    if ( Config_msgObj->intTx == INT )
    {
      REG ( Config_msgObj->can + CANIF1MCTL_R ) |= ( STD_HIGH << CANIFMCTL_TXIE ) ;
    }
    
    /** Set Data size **/
    REG ( Config_msgObj->can + CANIF1MCTL_R ) &= CANIF1MCTL_DLC ;
    REG ( Config_msgObj->can + CANIF1MCTL_R ) |= Config_msgObj->dataSize ; 
    
    break ;
  case MSG_RX_REMOTE :  
    /** set the WRND bit to transfer data from Interfernce register to the CAN object **/
    REG ( Config_msgObj->can + CANIF1CMSK_R ) = ( STD_HIGH << CANIFCMSK_WRNRD ) ;
    
    /** transfer data bytes 0-3 in message object to CANIFnDA1 and CANIFnDA2. **/
    REG ( Config_msgObj->can + CANIF1CMSK_R ) |= ( STD_HIGH << CANIFCMSK_DATAA ) ;
    
    /** transfer data bytes 4-7 in message object to CANIFnDA1 and CANIFnDA2. **/
    REG ( Config_msgObj->can + CANIF1CMSK_R ) |= ( STD_HIGH << CANIFCMSK_DATAB ) ;
    
    /**Transfer control bits from the CANIFnMCTL register into the Interface registers    **/
    REG ( Config_msgObj->can + CANIF1CMSK_R ) |= ( STD_HIGH << CANIFCMSK_CONTROL ) ;
    
    /** Transfer IDMASK + DIR + MXTD of the message object into the Interface registers **/
    REG ( Config_msgObj->can + CANIF1CMSK_R ) |= ( STD_HIGH << CANIFCMSK_ARB ) ;
    
    /** Set unmask bit to allow acceptance filtering **/
    REG ( Config_msgObj->can + CANIF1CMSK_R ) |= ( STD_HIGH << CANIFCMSK_MASK ) ;
    
    if ( Config_msgObj->IDLen == ID_29_BIT )
    {
      /** Set mask bits for 29 bit ID **/
      REG ( Config_msgObj->can + CANIF1MSK1_R ) = RESET_REG ;
      REG ( Config_msgObj->can + CANIF1MSK1_R ) = Config_msgObj->msgIDMask ;
      
      REG ( Config_msgObj->can + CANIF1MSK2_R ) &= CANIF1MSK2_RE ;
      REG ( Config_msgObj->can + CANIF1MSK2_R ) |= ( Config_msgObj->msgIDMask >> MASK_EXT ) ;
      
      REG ( Config_msgObj->can + CANIF1MSK2_R ) |= ( STD_HIGH  << CANIFMSK2_MDIR ) ;
      REG ( Config_msgObj->can + CANIF1MSK2_R ) |= ( STD_HIGH  << CANIFMSK2_MXTD ) ;
      
      /** Set ID **/
      REG ( Config_msgObj->can + CANIF1ARB1_R ) = RESET_REG ;
      REG ( Config_msgObj->can + CANIF1ARB1_R ) = Config_msgObj->msgID ; 
      
      REG ( Config_msgObj->can + CANIF1ARB2_R ) &= CANIF1ARB2_RE ;
      REG ( Config_msgObj->can + CANIF1ARB2_R ) |= ( Config_msgObj->msgID >> ID_EXT ) ;  
      REG ( Config_msgObj->can + CANIF1ARB2_R ) |= ( STD_HIGH << CANIFARB2_XTD ) ;
      
    }
    else if ( Config_msgObj->IDLen == ID_11_BIT )
    {
      /** Set mask bits for 11 bit ID **/
      REG ( Config_msgObj->can + CANIF1MSK2_R ) &= CANIF1MSK2_RE ;
      REG ( Config_msgObj->can + CANIF1MSK2_R ) |= ( Config_msgObj->msgIDMask <<  MASK ) ;
      
      REG ( Config_msgObj->can + CANIF1MSK2_R ) |= ( STD_HIGH  << CANIFMSK2_MDIR ) ;
      REG ( Config_msgObj->can + CANIF1MSK2_R ) |= ( STD_HIGH  << CANIFMSK2_MXTD ) ;
      
      /** Set 11 bit id **/
      REG ( Config_msgObj->can + CANIF1ARB2_R ) &= CANIF1ARB2_RE ;
      REG ( Config_msgObj->can + CANIF1ARB2_R ) |= ( Config_msgObj->msgID << ID ) ;  
      
      
    }
    
    REG ( Config_msgObj->can + CANIF1ARB2_R ) |= ( STD_HIGH << CANIFARB2_MGSVAL ) ;
    
    //REG ( Config_msgObj->can + CANIF1ARB2_R ) &= CANIF1ARB2_RX ;

    REG ( Config_msgObj->can + CANIF1ARB2_R ) |= ( STD_HIGH << CANIFARB2_DIR ) ;
    
    REG ( Config_msgObj->can + CANIF1MCTL_R ) |= ( STD_HIGH << CANIFMCTL_RMTEN) ;
    
    REG ( Config_msgObj->can + CANIF1MCTL_R ) |= ( STD_HIGH << CANIFMCTL_EOB ) ;    
    
    
    REG ( Config_msgObj->can + CANIF1MCTL_R ) |= ( STD_HIGH << CANIFMCTL_UMASK ) ;
    
    
    
    
    /** Enable Interrupt **/ 
    if ( Config_msgObj->intRx == INT )
    {
      REG ( Config_msgObj->can + CANIF1MCTL_R ) |= ( STD_HIGH << CANIFMCTL_RXIE ) ;
    }
    
    /** Set Data size **/
    REG ( Config_msgObj->can + CANIF1MCTL_R ) &= CANIF1MCTL_DLC ;
    REG ( Config_msgObj->can + CANIF1MCTL_R ) |= Config_msgObj->dataSize ; 
    
    break ;
    
  }
  REG ( Config_msgObj->can + CANIF1CRQ_R ) = Config_msgObj->msgObj ;

  
}

void CAN_receiveMsgObj ( CAN_msgObj * Config_msgObj ) 
{
    uint8 check =0 ;
  check = REG ( Config_msgObj->can + CANNWDA1_R ) ;
  /** set the WRND bit to transfer data from Interfernce register to the CAN object **/
  
  REG ( Config_msgObj->can + CANIF2ARB2_R ) |= ( Config_msgObj->msgID << ID ) ;  
  
  REG ( Config_msgObj->can + CANIF2CRQ_R ) = Config_msgObj->msgObj ; //----------------------------->
  
 // REG ( Config_msgObj->can + CANIF2MCTL_R ) &= CANIF2MCTL_TXRQST_RE  ;
  
  REG ( Config_msgObj->can + CANIF2CMSK_R ) &= CANIF2CMSK_WR_RE ;
  
  /** transfer data bytes 0-3 in message object to CANIFnDA1 and CANIFnDA2. **/
  REG ( Config_msgObj->can + CANIF2CMSK_R ) = ( STD_HIGH << CANIFCMSK_DATAA ) ;
  
  /** transfer data bytes 4-7 in message object to CANIFnDA1 and CANIFnDA2. **/
  REG ( Config_msgObj->can + CANIF2CMSK_R ) |= ( STD_HIGH << CANIFCMSK_DATAB ) ;
  
  /**Transfer control bits from the CANIFnMCTL register into the Interface registers    **/
  REG ( Config_msgObj->can + CANIF2CMSK_R ) |= ( STD_HIGH << CANIFCMSK_CONTROL ) ;
  
  /** Transfer IDMASK + DIR + MXTD of the message object into the Interface registers **/
  REG ( Config_msgObj->can + CANIF2CMSK_R ) |= ( STD_HIGH << CANIFCMSK_ARB ) ;
  
  /** Set unmask bit to allow acceptance filtering **/
  REG ( Config_msgObj->can + CANIF2CMSK_R ) |= ( STD_HIGH << CANIFCMSK_MASK ) ;
  
  REG ( Config_msgObj->can + CANIF2CMSK_R ) |= ( STD_HIGH << 2 ) ;//------------------------------------------------->
  
  while ( REG ( Config_msgObj->can + CANIF2CRQ_R ) & CANIF2CRQ_BUSY ) {};
  
  if ( REG ( Config_msgObj->can + CANIF2MCTL_R ) & CANIF2MCTL_NEWDAT )
  {
    
    Config_msgObj->data[0] =   REG ( Config_msgObj->can   + CANIF2DA1_R  ) ;
    Config_msgObj->data[1] = ( REG ( Config_msgObj->can   + CANIF2DA1_R  ) >> SEC_BYTE ) ;
    Config_msgObj->data[2] =   REG ( Config_msgObj->can   + CANIF2DA2_R  ) ;
    Config_msgObj->data[3] = ( REG ( Config_msgObj->can   + CANIF2DA2_R  ) >> SEC_BYTE ) ;
    Config_msgObj->data[4] =   REG ( Config_msgObj->can   + CANIF2DB1_R  ) ;
    Config_msgObj->data[5] = ( REG ( Config_msgObj->can   + CANIF2DB1_R  ) >> SEC_BYTE ) ;
    Config_msgObj->data[6] =   REG ( Config_msgObj->can   + CANIF2DB2_R  ) ;
    Config_msgObj->data[7] = ( REG ( Config_msgObj->can   + CANIF2DB2_R  ) >> SEC_BYTE ) ;
    
    REG ( Config_msgObj->can + CANIF2CMSK_R  ) = ( STD_HIGH << CANIFCMSK_NEWDAT ) ;
    
    REG ( Config_msgObj->can + CANIF2MCTL_R ) &= CANMCTL_RE ;  
    
    while ( REG ( Config_msgObj->can + CANIF2CRQ_R ) & CANIF2CRQ_BUSY ) {};
    
  }

  /** Receive Remote Frame **/ 
  if (  ( REG ( Config_msgObj->can + CANIF2MCTL_R )   & CANIF2MCTL_TXRQST )   || ( REG ( Config_msgObj->can + CANNWDA1_R ) )  )
  {
      REG ( Config_msgObj->can + CANIF2MCTL_R ) &= CANIF2MCTL_TXRQST_RE  ;
      Config_msgObj->data[7]= 1;
  }
  //&& Config_msgObj->canMsgType == MSG_RX_REMOTE '

 /*
  if  (  check == 0 )
  {
    
    for ( uint8 i = 0 ; i < 8 ; i++ )
    {
      Config_msgObj->data[i] = 0 ; 
    }

  }
*/
}


void CAN_enable ( const CAN_configType * Config_canInit )
{
  REG ( Config_canInit->can + CANCTL_R ) &= CANCTL_ENABLE ;  
}

void CAN_disable ( const CAN_configType * Config_canInit )
{
  REG ( Config_canInit->can + CANCTL_R ) = ( STD_HIGH << CANCTL_INIT ) ;
}


void CAN_intEnable ( const CAN_configType * Config_canInit )
{
  REG ( Config_canInit->can + CANCTL_R ) |= ( STD_HIGH << CANCTL_IE ) ;
}

void CAN_intDisable  ( const CAN_configType * Config_canInit )
{
  REG ( Config_canInit->can + CANCTL_R ) &=  CANCTL_INTDIS ;
}




void CAN_reset ( void )
{
  CAN0_CTL_R              = RESET_REG ;          
  CAN0_STS_R              = RESET_REG ;          
  CAN0_ERR_R              = RESET_REG ;          
  CAN0_BIT_R              = RESET_REG ;          
  CAN0_INT_R              = RESET_REG ;          
  CAN0_TST_R              = RESET_REG ;          
  CAN0_BRPE_R             = RESET_REG ;          
  CAN0_IF1CRQ_R           = RESET_REG ;          
  CAN0_IF1CMSK_R          = RESET_REG ;          
  CAN0_IF1MSK1_R          = RESET_REG ;          
  CAN0_IF1MSK2_R          = RESET_REG ;          
  CAN0_IF1ARB1_R          = RESET_REG ;          
  CAN0_IF1ARB2_R          = RESET_REG ;          
  CAN0_IF1MCTL_R          = RESET_REG ;          
  CAN0_IF1DA1_R           = RESET_REG ; 
  CAN0_IF1DA2_R           = RESET_REG ; 
  CAN0_IF1DB1_R           = RESET_REG ; 
  CAN0_IF1DB2_R           = RESET_REG ; 
  CAN0_IF2DA1_R           = RESET_REG ; 
  CAN0_IF2DA2_R           = RESET_REG ; 
  CAN0_IF2DB1_R           = RESET_REG ; 
  CAN0_IF2DB2_R           = RESET_REG ; 
  CAN0_IF2CRQ_R           = RESET_REG ;          
  CAN0_IF2CMSK_R          = RESET_REG ;          
  CAN0_IF2MSK1_R          = RESET_REG ;          
  CAN0_IF2MSK2_R          = RESET_REG ;          
  CAN0_IF2ARB1_R          = RESET_REG ;          
  CAN0_IF2ARB2_R          = RESET_REG ;          
  CAN0_IF2MCTL_R          = RESET_REG ;          
  
  
}

