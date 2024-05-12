/*****************************************************************************************************/
/* Module Name : systick ( header file )                                                             */
/*																									 */
/* Author      : Team 1                                                                              */
/*																									 */
/* Purpose     : This header file defines function prototypes for configuring and utilizing the		 */
/* 				SysTick timer in embedded systems. The SysTick timer is commonly used for generating */
/* 				precise delays, implementing timeouts, and creating periodic interrupts.			 */
/* 				Users can include this header file in their projects to enable and customize		 */
/* 				SysTick functionality based on their specific requirements	                         */
/*																									 */
/*****************************************************************************************************/



#ifndef _SYSTICK_H
#define _SYSTICK_H

#include "types.h"


/****************************************Functions Prototype******************************************/


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
void SysTick_Disable(void);




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
void SysTick_Enable(void);



/*****************************************************************************************************/
/* Function Name :  SysTick_Period_Get															     */
/* Inputs        :  void   ( No inputs  )                                                            */
/* Outputs       :  uint32 ( NVIC_ST_CURRENT_R ) 								   					 */
/* Reentrancy    :  Reentrant																		 */
/* Synchronous   :  Synchronous																		 */
/* Description   :	This function calculates and returns the elapsed time in milliseconds by dividing*/ 
/* 					the current value of the SysTick timer by the assumed clock frequency (16000 Hz) */
/*****************************************************************************************************/
uint32 SysTick_Period_Get(void);


/*****************************************************************************************************/
/* Function Name :  SysTick_Period_Set															     */
/* Inputs        :  uint332 ( timer_ms  )                                                            */
/* Outputs       :  void  ( No_output ) 								   	   						 */
/* Reentrancy    :  Reentrant																		 */
/* Synchronous   :  Asynchronous																	 */
/* Description   :	This function calculates the appropriate reload value based on the given time    */
/* 					in milliseconds and sets the SysTick reload register (NVIC_ST_RELOAD_R)			 */
/* 					synchronously. It allows users to adjust the period of the SysTick timer for	 */
/* 					generating periodic interrupts or implementing delays.							 */
/*****************************************************************************************************/
void SysTick_Period_Set(uint32);


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
uint32 SysTick_Value_Get(void);


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
uint8 SysTick_Is_Time_Out(void);


/*****************************************************************************************************/
/* Function Name :  SysTick_Delay																     */
/* Inputs        :  uint32 	( n ) 		                           	                                 */
/* Outputs       :  void  ( No_outputs )		 								   	   				 */
/* Reentrancy    :  Reentrant																		 */
/* Synchronous   :  Asynchronous																	 */
/* Description   :	This function configures the SysTick timer for a one-shot delay by setting the   */ 
/*					reload value and then waits in a loop until the COUNTFLAG is set, indicating that*/
/*					the specified number of SysTick timer ticks has elapsed. The delay duration is   */
/*					determined by the value of 'n'.												     */
/*****************************************************************************************************/
void SysTick_Delay(uint32 n);



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
void SysTick_1ms (uint32 delay);


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
void Systick_Start(void);
#endif



