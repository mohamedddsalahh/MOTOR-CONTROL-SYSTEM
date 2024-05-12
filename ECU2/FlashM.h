#ifndef FLASH_M_H_
#define FLASH_M_H_

#include "types.h"
#include "bitwise_operation.h"
#include "tm4c123gh6pm.h"

#define DTC_Addr        0x00004400

#define FLASH_BASE      0x400FD000
#define BOOTCFG         FLASH_BOOTCFG_R

#define BlockSize       1024
#define WRKEY0          0x71D5
#define WRKEY1          0xA442
#define ERASE_Bit       0x02
#define WRITE_Bit       0x01

#define FLASHM_BASE     0x00000000

typedef struct{
  
  uint32        FMA;
  uint32        FMD;
  uint32        FMC;
  uint32        FCRIS;
  uint32        FCIM;
  uint32        FCMISC;
  
}Flash_t; 

#define FlashM_Cfg      ((Flash_t*)FLASH_BASE);



void EraseM_Block(uint32 Memaddress);
void WriteM_Byte(uint32 MemAddress, uint32 Value);
uint32 ReadM_Byte(uint32 MemAddress);

#endif