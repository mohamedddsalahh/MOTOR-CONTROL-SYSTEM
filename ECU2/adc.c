#include "adc.h"

void LM35_Init(){
  
  SYSCTL_RCGCGPIO_R |= 0x10;      
  SYSCTL_RCGCADC_R |= 1;         
  SYSCTL_RCGCWTIMER_R |= 1;  
  
  
  GPIO_PORTE_AFSEL_R |= 8;       
  GPIO_PORTE_DEN_R &= ~8;         
  GPIO_PORTE_AMSEL_R |= 8;     
  

  ADC0_ACTSS_R &= ~8;            
  ADC0_EMUX_R &= ~0xF000;        
  ADC0_EMUX_R |= 0x5000;        
  ADC0_SSMUX3_R = 0;          
  ADC0_SSCTL3_R |= 6;       
  ADC0_IM_R |= (1<<3);  
  
  ADC0_ACTSS_R |= 8;          
  
  
  WTIMER0_CTL_R = 0;        
  WTIMER0_CFG_R = 0x04;     
  WTIMER0_TAMR_R = 0x02;    
  WTIMER0_TAILR_R = 16000000;
  WTIMER0_CTL_R |= 0x20;      
  WTIMER0_CTL_R |= 0x01;    
  
  
}



uint32 LM35_Value(){
  uint32 Temp;
  uint32 AdcValue;
  AdcValue = ADC0_SSFIFO3_R;									
  Temp =(( AdcValue * 330) / 4096);      
  return Temp;
}

void REF_init ( void )
{
  SYSCTL_RCGCGPIO_R |= 0x10;      
  SYSCTL_RCGCADC_R |= (1 << 1 );         
  //SYSCTL_RCGCWTIMER_R |= 1;  
  
  
  GPIO_PORTE_AFSEL_R |= 4;       
  GPIO_PORTE_DEN_R &= ~4;         
  GPIO_PORTE_AMSEL_R |= 4;     
  
  
  ADC1_ACTSS_R &= ~8;            
  ADC1_EMUX_R &= ~0xF000;        
  ADC1_EMUX_R |= 0x5000;        
  ADC1_SSMUX3_R = 1;          
  ADC1_SSCTL3_R |= 6;       
  ADC1_IM_R |= (1<<3);  
  
  ADC1_ACTSS_R |= 8;     
}


float32 REF_Value(){
  float32 volt;
  uint32 AdcValue;
  AdcValue = ADC1_SSFIFO3_R;									
  volt =(( AdcValue * 3.3) / 4096);    
  if ( volt >= 3.25 && volt <=3.4 )
  {
     volt = 5 ;
  }
  return volt;
}













