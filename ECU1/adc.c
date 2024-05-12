#include "adc.h"

void LM35_Init(void)
{
  
  SYSCTL_RCGCGPIO_R |= 0x8;
  SYSCTL_RCGCGPIO_R |= 0x10;   
  SYSCTL_RCGCADC_R |= 3;         
  SYSCTL_RCGCWTIMER_R |= 1;   
  GPIO_PORTE_AFSEL_R |= 8;       
  GPIO_PORTE_DEN_R &= ~8;         
  GPIO_PORTE_AMSEL_R |= 8;    
  
  
  GPIO_PORTE_AFSEL_R |= 4;
  GPIO_PORTE_DEN_R &= ~4;
  GPIO_PORTE_AMSEL_R |= 4;
  

  
  ADC1_ACTSS_R &= ~4;            
  ADC1_EMUX_R &= ~0x0F000;         
  ADC1_EMUX_R |= 0x0500;   
  ADC1_SSCTL2_R |= 6 ;          
  //ADC0_IM_R |= (1<<3);  
  ADC1_ACTSS_R |= 4 ;   

  
  WTIMER0_CTL_R = 0;       
  WTIMER0_CFG_R = 0x04;     
  WTIMER0_TAMR_R = 0x02;       
  WTIMER0_TAILR_R = 16000000;  
  WTIMER0_CTL_R |= 0x20;       
  WTIMER0_CTL_R |= 0x01;     
  
  
}



uint32 LM35_Value(void )
{
  ADC1_SSMUX2_R &= 0xFFF0 ;
  ADC1_SSMUX2_R = 0 ;
  uint32 Temp;
  uint32 AdcValue;
  uint32 AdcValue1;
  GPIO_PORTE_AMSEL_R |= 8;
  GPIO_PORTE_DEN_R &= ~8;
  ADC1_PSSI_R |= 8 ; 
  AdcValue = ADC1_SSFIFO2_R;	
  ADC1_ISC_R |= 8 ;
  Temp =(( AdcValue * 330 ) / 4096);
  GPIO_PORTE_AMSEL_R &= 0xFFFFFFF7;
  GPIO_PORTE_DEN_R |= 8;
  
  return Temp;
}


float32 ref_Voltage(void )
{
  ADC0_SSMUX3_R &= 0xFFF0 ; 
  ADC0_SSMUX3_R = 1; 
  float32 ref;
  uint32 AdcValue;
  uint32 AdcValue1;
  GPIO_PORTE_AMSEL_R |= 4;
  GPIO_PORTE_DEN_R &= 4;
  ADC0_PSSI_R |= 8 ; 
  AdcValue = ADC0_SSFIFO3_R;	
  ADC0_ISC_R |= 8 ;
  ref =(( AdcValue * 3.3) / 4096);      
  GPIO_PORTE_AMSEL_R &= 0xFFFFFFFB;
  GPIO_PORTE_DEN_R |= 4;
  return ref ;
}
