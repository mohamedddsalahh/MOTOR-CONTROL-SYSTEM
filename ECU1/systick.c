/****************************************************************************************************/
/* Module Name : systick ( source file )                                                            */
/*																									*/
/* Author      : Team 1                                                                             */
/*																									*/
/* Purpose     : This source file contains the implementation of functions declared in "systick.h"  */
/*               for configuring and utilizing the SysTick timer on the TM4C123GH6PM                */
/*               microcontroller. 																	*/
/*				 The functions include enabling/disabling SysTick,setting/getting the timer period, */
/*               retrieving the current timer value, checking for timeouts, implementing delays,	*/
/* 				 and starting the SysTick with a default 1 ms period.								*/
/*																									*/
/****************************************************************************************************/



#include "types.h"
#include "tm4c123gh6pm.h"
#include "bitwise_operation.h"
#include "systick.h"


/*********************************************Functions***********************************************/

/*****************************************************************************************************/
/* Function Name :  SysTick_Disable																	 */
/* Inputs        :  void ( No inputs  )                                                              */
/* Outputs       :  void ( No outputs )																 */
/* Reentrancy    :  Reentrant																		 */
/* Synchronous   :  Asynchronous																	 */
/* Description   :	This function stops the SysTick timer, setting its control bits to zero and		 */
/* 					resetting the reload and current value registers. It includes a check for the	 */
/* 					SysTick interrupt enablement, but the result is not utilized.					 */
/*****************************************************************************************************/
void SysTick_Disable(void){

  /* Clock Source : SysTick Control and Status Register*/
  /* bit 2 = 1 --> Clock Source											*/
  /*  bit 2 = 0 --> Precision internal oscillator (PIOSC) divided by 4	*/
  CLEAR_BIT(NVIC_ST_CTRL_R,2);
  
  /* Interrupt Enable : SysTick Control and Status Register						*/
  /*  bit 1 = 0 --> Interrupt generation is disabled							*/
  /*  bit 1 = 1 --> An interrupt is generated to the NVIC when SysTick counts	*/
  /*                to 0.														*/
  CLEAR_BIT(NVIC_ST_CTRL_R,1);
  
  
  /* Enable Systick : SysTick Control and Status Register							*/
  /*  bit 0 = 0 --> The counter is disabled.										*/
  /*  bit 0 = 1 --> Enables SysTick to operate in a multi-shot way. That is, the	*/
  /*                 counter loads the RELOAD value and begins counting down.		*/
  CLEAR_BIT(NVIC_ST_CTRL_R,0);
  
  /* Reload Value : SysTick Reload Value Register */
  /* bits 0:23    : Systick reload value 		  */
  NVIC_ST_RELOAD_R = 0;
  
  /* Systick Value : SysTick Current Value Register		*/
  /* bits 0:23    : Curretnt value running 				*/
  NVIC_ST_CURRENT_R = 0;
  
  /* Count		  : SysTick Control and Status Register												*/
  /* bits 16 = 0 --> The SysTick timer has not counted to 0 since the last time this bit was read	*/
  /* bits 16 = 1 --> The SysTick timer has counted to 0 since the last time this bit was read.		*/
  if(GET_BIT(NVIC_ST_CTRL_R,16)){}
}


/*****************************************************************************************************/
/* Function Name :  SysTick_Enable																     */
/* Inputs        :  void ( No inputs  )                                                              */
/* Outputs       :  void ( No outputs )																 */
/* Reentrancy    :  Reentrant																		 */
/* Synchronous   :  Asynchronous																	 */
/* Description   :	This function enables the SysTick timer by setting the control bits responsible	 */
/* 					for its operation.It specifically sets the ENABLE and TICKINT bits in the control*/
/* 					register, allowing the SysTick timer to start counting and triggering interrupts.*/
/*****************************************************************************************************/
void SysTick_Enable(void){
	
  /* Clock Source : SysTick Control and Status Register					*/
  /*  bit 2 = 1 --> Clock Source										*/
  /*  bit 2 = 0 --> Precision internal oscillator (PIOSC) divided by 4	*/	
  SET_BIT(NVIC_ST_CTRL_R,2);
  
  /* Enable Systick : SysTick Control and Status Register							*/
  /*  bit 0 = 0 --> The counter is disabled.										*/
  /*  bit 0 = 1 --> Enables SysTick to operate in a multi-shot way. That is, the 	*/
  /*                 counter loads the RELOAD value and begins counting down.		*/
  SET_BIT(NVIC_ST_CTRL_R,0);
}


/*****************************************************************************************************/
/* Function Name :  SysTick_Period_Get															     */
/* Inputs        :  void   ( No inputs  )                                                            */
/* Outputs       :  uint32 ( NVIC_ST_CURRENT_R ) 								   					 */
/* Reentrancy    :  Reentrant																		 */
/* Synchronous   :  Asynchronous						   											 */
/* Description   :	This function calculates and returns the elapsed time in milliseconds by dividing*/ 
/* 					the current value of the SysTick timer by the assumed clock frequency (16000 Hz) */
/*****************************************************************************************************/
uint32 SysTick_Period_Get(void){
	
  /* Systick Value : SysTick Current Value Register	*/
  /* bits 0:23    : Curretnt value running /16000	*/
  return NVIC_ST_CURRENT_R / 16000;
}




/*****************************************************************************************************/
/* Function Name :  SysTick_Period_Set															     */
/* Inputs        :  uint32 ( timer_ms  )                                                             */
/* Outputs       :  void  ( No_output ) 								   	   						 */
/* Reentrancy    :  Reentrant																		 */
/* Synchronous   :  Asynchronous																	 */
/* Description   :	This function calculates the appropriate reload value based on the given time    */
/* 					in milliseconds and sets the SysTick reload register (NVIC_ST_RELOAD_R)			 */
/* 					synchronously. It allows users to adjust the period of the SysTick timer for	 */
/* 					generating periodic interrupts or implementing delays.							 */
/*****************************************************************************************************/
void SysTick_Period_Set(uint32 time_ms){
	
  /* Reload Value : SysTick Reload Value Register */
  /* bits 0:23    : Systick reload value* 16000	  */
  NVIC_ST_RELOAD_R = time_ms*16000;
}


/*****************************************************************************************************/
/* Function Name :  SysTick_Value_Get															     */
/* Inputs        :  void 	( No_input  )                                                            */
/* Outputs       :  uint32  ( NVIC_ST_CURRENT_R ) 								   	   				 */
/* Reentrancy    :  Reentrant																		 */
/* Synchronous   :  Asynchronous																	 */
/* Description   :	This function returns the current value of the SysTick timer's countdown register*/
/*					(NVIC_ST_CURRENT_R), providing a snapshot of the remaining time before the next  */
/*					SysTick interrupt occurs. Users can use this function to assess the current state*/
/*					of the SysTick timer.															 */
/*****************************************************************************************************/
uint32 SysTick_Value_Get(void){

  /* Systick Value : SysTick Current Value Register	*/
  /*bits 0:23    : Curretnt value running /16000	*/	
  return NVIC_ST_CURRENT_R;
}


/*****************************************************************************************************/
/* Function Name :  SysTick_Is_Time_Out															     */
/* Inputs        :  void 	( No_input  )                                                            */
/* Outputs       :  uint8  ( return 1 , 0 )		 								   	   				 */
/* Reentrancy    :  Reentrant																		 */
/* Synchronous   :  Asynchronous																	 */
/* Description   :	This function checks the COUNTFLAG bit in the SysTick Control and Status register*/
/* 					(NVIC_ST_CTRL_R) to determine if the SysTick timer has timed out since the last  */
/*					reload.If the COUNTFLAG is set, indicating a timeout, the function returns 1;	 */ 
/*					otherwise, it returns 0.														 */
/*****************************************************************************************************/
uint8 SysTick_Is_Time_Out(void){

  /* Count		  : SysTick Control and Status Register												*/
  /* bits 16 = 0 --> The SysTick timer has not counted to 0 since the last time this bit was read	*/
  /* bits 16 = 1 --> The SysTick timer has counted to 0 since the last time this bit was read.		*/
  if(GET_BIT(NVIC_ST_CTRL_R,16)){
    return 1;
  }
  else{
    return 0;
  }
}

/*****************************************************************************************************/
/* Function Name :  SysTick_Delay																     */
/* Inputs        :  uint32 	( Delay )                           	                                 */
/* Outputs       :  void  ( No_outputs )		 								   	   				 */
/* Reentrancy    :  Reentrant																		 */
/* Synchronous   :  Asynchronous																	 */
/* Description   :	This function configures the SysTick timer for a one-shot delay by setting the   */ 
/*					reload value and then waits in a loop until the COUNTFLAG is set, indicating that*/
/*					the specified number of SysTick timer ticks has elapsed. The delay duration is   */
/*					determined by the value of 'n'.											  		 */
/*****************************************************************************************************/
void SysTick_Delay (uint32 n)
{
  /* Clock Source : SysTick Control and Status Register					*/
  /*  bit 2 = 1 --> Clock Source										*/
  /*  bit 2 = 0 --> Precision internal oscillator (PIOSC) divided by 4	*/
  CLEAR_BIT(NVIC_ST_CTRL_R,2);
  
  /* Interrupt Enable : SysTick Control and Status Register						*/
  /*  bit 1 = 0 --> Interrupt generation is disabled							*/
  /*  bit 1 = 1 --> An interrupt is generated to the NVIC when SysTick counts	*/
  /*                to 0.														*/
  CLEAR_BIT(NVIC_ST_CTRL_R,1);
  
  
  /* Enable Systick : SysTick Control and Status Register							*/
  /*  bit 0 = 0 --> The counter is disabled.										*/
  /*  bit 0 = 1 --> Enables SysTick to operate in a multi-shot way. That is, the 	*/
  /*                 counter loads the RELOAD value and begins counting down.		*/
  CLEAR_BIT(NVIC_ST_CTRL_R,0);

  /* Reload Value : SysTick Reload Value Register	*/
  /* bits 0:23    : Systick reload value-n			*/
  NVIC_ST_RELOAD_R = n-1;
  
  /* Systick Value : SysTick Current Value Register	*/
  /* bits 0:23    : Curretnt value running 			*/
  NVIC_ST_CURRENT_R = 0;
  
  /* Clock Source : SysTick Control and Status Register					*/
  /*  bit 2 = 1 --> Clock Source										*/
  /*  bit 2 = 0 --> Precision internal oscillator (PIOSC) divided by 4	*/
  SET_BIT(NVIC_ST_CTRL_R,2);
  
  /* Enable Systick : SysTick Control and Status Register							*/
  /*  bit 0 = 0 --> The counter is disabled.										*/
  /*  bit 0 = 1 --> Enables SysTick to operate in a multi-shot way. That is, the	*/
  /*                 counter loads the RELOAD value and begins counting down.		*/	
  SET_BIT(NVIC_ST_CTRL_R,0);
  
  /* Count		  : SysTick Control and Status Register												*/
  /* bits 16 = 0 --> The SysTick timer has not counted to 0 since the last time this bit was read	*/
  /* bits 16 = 1 --> The SysTick timer has counted to 0 since the last time this bit was read.		*/
  
  while((NVIC_ST_CTRL_R & (1<<16))==0){}
}



/*****************************************************************************************************/
/* Function Name :  SysTick_1ms														    		     */
/* Inputs        :  uint32 	( delay )                           	                                 */
/* Outputs       :  void  ( No_outputs )		 								   	   				 */
/* Reentrancy    :  Reentrant																		 */
/* Synchronous   :  Synchronous																		 */
/* Description   :	This function utilizes the SysTick_Delay function to implement a delay of the	 */
/*					specified duration in milliseconds. It loops 'delay' times, each time invoking   */
/*					SysTick_Delay with a parameter equivalent to 1 millisecond  					 */
/*					(16000 SysTick timer ticks).											     	 */
/*****************************************************************************************************/
void SysTick_1ms (uint32 delay)
{
  // Call for systick delay function to apply 1 ms delay
  for(int i=0;i<delay;i++){
    SysTick_Delay(16000);
  }
}



/*****************************************************************************************************/
/* Function Name :  SysTick_Start	 												    		     */
/* Inputs        :  void  	( No_input )                           	                                 */
/* Outputs       :  void  ( No_outputs )		 								   	   				 */
/* Reentrancy    :  Reentrant																		 */
/* Synchronous   :  Asynchronous																	 */
/* Description   :	This function configures the SysTick timer by clearing control bits, reloading	 */
/*					the timer, and setting the initial value. The reload value is set to achieve     */
/*					a 1 ms period based on the assumption that the CPU frequency is 16 MHz.          */
/*					The function then enables the SysTick timer and sets its priority level.		 */
/*****************************************************************************************************/
void Systick_Start(void)
{
	
  /* Clock Source : SysTick Control and Status Register					*/
  /*  bit 2 = 1 --> Clock Source										*/
  /*  bit 2 = 0 --> Precision internal oscillator (PIOSC) divided by 4	*/
  CLEAR_BIT(NVIC_ST_CTRL_R,2);
  
  /* Interrupt Enable : SysTick Control and Status Register						*/
  /*  bit 1 = 0 --> Interrupt generation is disabled							*/
  /*  bit 1 = 1 --> An interrupt is generated to the NVIC when SysTick counts	*/
  /*                to 0.														*/
  CLEAR_BIT(NVIC_ST_CTRL_R,1);
  
  
  /* Enable Systick : SysTick Control and Status Register							*/
  /*  bit 0 = 0 --> The counter is disabled.										*/
  /*  bit 0 = 1 --> Enables SysTick to operate in a multi-shot way. That is, the	*/
  /*                 counter loads the RELOAD value and begins counting down.		*/
  CLEAR_BIT(NVIC_ST_CTRL_R,0);
  
  /* Reload Value : SysTick Reload Value Register	*/
  /* bits 0:23    : Systick reload value			*/
  NVIC_ST_RELOAD_R = 0;
  
  
  /* Systick Value : SysTick Current Value Register		*/
  /* bits 0:23    : Curretnt value running 				*/ 
  NVIC_ST_CURRENT_R = 0;      
  
  /* Count		  : SysTick Control and Status Register												*/
  /* bits 16 = 0 --> The SysTick timer has not counted to 0 since the last time this bit was read	*/
  /* bits 16 = 1 --> The SysTick timer has counted to 0 since the last time this bit was read.		*/
  if(GET_BIT(NVIC_ST_CTRL_R,16)){}; 
  NVIC_ST_RELOAD_R = 1 *16000;  //Set the Reload Value (16000 to get 1 ms as CPU_Freq = 16MHz)
  
  /* Enable SysTick   */
  /* Clock Source : SysTick Control and Status Register					*/
  /*  bit 2 = 1 --> Clock Source										*/
  /*  bit 2 = 0 --> Precision internal oscillator (PIOSC) divided by 4	*/
  SET_BIT(NVIC_ST_CTRL_R,2);
  
  /* Interrupt Enable : SysTick Control and Status Register							*/
  /*  bit 1 = 0 --> Interrupt generation is disabled								*/	
  /*  bit 1 = 1 --> An interrupt is generated to the NVIC when SysTick counts		*/
  /*                to 0.															*/
  SET_BIT(NVIC_ST_CTRL_R,1) ; 
  
  
  /* Enable Systick : SysTick Control and Status Register							*/
  /*  bit 0 = 0 --> The counter is disabled.										*/
  /*  bit 0 = 1 --> Enables SysTick to operate in a multi-shot way. That is, the	*/
  /*                 counter loads the RELOAD value and begins counting down.		*/
  SET_BIT(NVIC_ST_CTRL_R,0); 
  
  //Set systick priority 
  //NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x00FFFFFF) | 0x40000000; 
}




