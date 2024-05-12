#ifndef _CAN_H_
#define _CAN_H_


#ifndef REG_H_
#define REG_H_

#define REG(x) (*((volatile unsigned long *)(x)))


#endif 



#define CANCTL_R     0x000
#define CANSTS_R     0x004
#define CANERR_R     0x008
#define CANBIT_R     0x00C
#define CANINT_R     0x010
#define CANTST_R     0x014
#define CANBRPE_R    0x018

#define CANIF1CRQ_R  0x020
#define CANIF1CMSK_R 0x024
#define CANIF1MSK1_R 0x028
#define CANIF1MSK2_R 0x02C
#define CANIF1ARB1_R 0x030
#define CANIF1ARB2_R 0x034
#define CANIF1MCTL_R 0x038

#define CANIF1DA1_R  0x03C
#define CANIF1DA2_R  0x040
#define CANIF1DB1_R  0x044
#define CANIF1DB2_R  0x048
#define CANDATASHIFT 0x004

#define CANIF2CRQ_R  0x080
#define CANIF2CMSK_R 0x084
#define CANIF2MSK1_R 0x088
#define CANIF2MSK2_R 0x08C
#define CANIF2ARB1_R 0x090
#define CANIF2ARB2_R 0x094
#define CANIF2MCTL_R 0x098

#define CANIF2DA1_R  0x09C
#define CANIF2DA2_R  0x0A0
#define CANIF2DB1_R  0x0A4
#define CANIF2DB2_R  0x0A8
#define CANNWDA1_R   0x120

#define STD_HIGH        1
#define MAX_DATA        8
#define SEC_BYTE        8
#define MASK_EXT        16
#define MASK            2
#define ID_EXT          16
#define ID              2
#define shitfDataReg    0x004
/*********************Regiseters***********************/
#define RESET_REG               0
#define CANIF1CRQ_BUSY          0x00008000
#define CANIF2CRQ_BUSY          0x00008000

#define CANIF1MSK1_RE           0x0000FFFF
#define CANIF1MSK2_RE           0x0000E000
#define CANIF1ARB2_RE           0x0000E000      
#define CANIF1MCTL_DLC          0x0000FFF0
#define CANIF2MCTL_NEWDAT       0x00008000
#define CANCTL_ENABLE           0xFFFFFFFE
#define CANCTL_CCE_OFF          0xFFFFFFBF
#define CANMCTL_RE              0xFFFF7FFF
#define CANIF2CMSK_WR_RE        0xFFFFFF7F
#define CANIF1ARB2_RX           0xFFFFDFFF
#define CANIF1MCTL_TXQ          0xFFFFFEFF
#define CANCTL_INTDIS           0xFFFFFFFD
#define CANIF1ARB2_TX_REMOTE    0xFFFFDFFF
#define CANIF2MCTL_TXRQST       0x00000100
#define CANIF2MCTL_TXRQST_RE    0xFFFFFEFF
/************************ BITS ***********************/
#define CANCTL_INIT            0
#define CANCTL_IE              1
#define CANCTL_CCE             6
#define CANBIT_TSEG1           8
#define CANBIT_TSEG2           12
#define CANBIT_SJW             6
#define CANBIT_BRP             0
#define CANBRPE_BRPE           6
#define CANIFCMSK_WRNRD        7
#define CANIFCMSK_MASK         6
#define CANIFCMSK_ARB          5
#define CANIFCMSK_CONTROL      4
#define CANIFCMSK_CLRINTPND    3
#define CANIFCMSK_NEWDAT       2
#define CANIFCMSK_DATAA        1
#define CANIFCMSK_DATAB        0
#define CANIFMCTL_EOB          7
#define CANIFMCTL_TXIE         8
#define CANIFMCTL_RXIE         9
#define CANIFMCTL_TXRQST       8
#define CANIFMCTL_UMASK        12
#define CANIFMSK2_MDIR         14
#define CANIFMSK2_MXTD         15
#define CANIFARB2_DIR          13
#define CANIFARB2_XTD          14
#define CANIFARB2_MGSVAL       15
#define CANIFMCTL_RMTEN         9

#define PORTB                   1
#define CAN0_PORT               PORTB
#define PORTA                   0
#define CAN1_PORT               PORTA 
#define CAN_CTL_IE              1
#define STD_HIGH                1   
#define STD_LOW                 0
#define CAN0_CLOCK              0
#define CAN1_CLOCK              1
#define PIN0                    0
#define PIN1                    1
#define PIN4                    4
#define PIN5                    5
#define CAN0_RX                 PIN4
#define CAN0_TX                 PIN5
#define CAN1_RX                 PIN0
#define CAN1_TX                 PIN1 
#define CAN_PMC_V               1
#define PIN4_PMC                19
#define PIN5_PMC                23
#define PIN0_PMC                3
#define PIN1_PMC                7
#define GPIO_PORTA_DR8R         0x03    
#define GPIO_PORTB_DR8R         0x30


#include "tm4c123gh6pm.h"
#include "types.h"
#include "bitwise_operation.h"


typedef enum {
  CAN0_ID = 0x40040000 , CAN1_ID = 0x40041000 
}canID ;

typedef enum 
{
  BASIC = 2 , SILENT = 3 , LBACK = 4u
}canMode ;

typedef struct{
  uint8  tseg1 ;
  uint8  tseg2 ;
  uint8  sjw   ; 
  uint8  brp   ;
  uint16 brpe  ;
}canBitTiming  ;


typedef enum {
  MSG_TX , MSG_RX , MSG_TX_REMOTE , MSG_RX_REMOTE 
}msgType;

typedef struct {
  canID          can       ;  
  canMode        mode      ;
  canBitTiming   bitTiming ;
}CAN_configType;


typedef enum {
  ID_11_BIT , ID_29_BIT
}idExt ;

typedef enum {
  NO_INT , INT 
}intEn ;

typedef struct {
  canID   can            ;   
  uint8   msgObj         ;
  msgType canMsgType     ;
  idExt   IDLen          ;
  uint32  msgIDMask      ;
  uint32  msgID          ;
  uint8 *  data          ;
  uint8   dataSize       ; 
  intEn   intRx          ;
  intEn   intTx          ;
  
}CAN_msgObj ;

void GPIO_initCan ( void ) ; 

void CAN_init ( const CAN_configType * Config_canInit );

void CAN_configMsgObj ( const CAN_msgObj * Config_msgObj );

void CAN_receiveMsgObj (  CAN_msgObj * Config_msgObj ) ;

void CAN_enable ( const CAN_configType * Config_canInit );

void CAN_disable ( const CAN_configType * Config_canInit );

void CAN_intEnable ( const CAN_configType * Config_canInit ) ;

void CAN_intDisable  ( const CAN_configType * Config_canInit );

void CAN_reset ( void ) ;



#endif 