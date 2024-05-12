#include "FlashM.h"

uint16  WRKEY;

void GetBOOTCfg (void){
  
  if(BOOTCFG & (0x10)){
    
    WRKEY = WRKEY1;
    
  }else{
    
    WRKEY = WRKEY0;
  }
}

void EraseM_Block(uint32 Memaddress){
  
  Flash_t* flashCfg_ptr = FlashM_Cfg;
  GetBOOTCfg();
  
  uint32 addr = FLASHM_BASE + (Memaddress/BlockSize)*(BlockSize);
  
  flashCfg_ptr->FMA = addr;
  flashCfg_ptr->FMC = (WRKEY << 16) | (ERASE_Bit);
  
  while((flashCfg_ptr->FMC) & ERASE_Bit);
}


void WriteM_Byte(uint32 MemAddress, uint32 Value){
  
  EraseM_Block(MemAddress);
  
  Flash_t* Cfg_ptr = FlashM_Cfg;
  uint32 val = 0;
  GetBOOTCfg(); 
  val = Value;
  
  Cfg_ptr->FMD = val;
  Cfg_ptr->FMA = MemAddress;
  Cfg_ptr->FMC = (WRKEY << 16) | (WRITE_Bit);
  
  while((Cfg_ptr->FMC) & WRITE_Bit);
}


uint32 ReadM_Byte(uint32 MemAddress){
  
  return (*((uint32*)MemAddress));
}