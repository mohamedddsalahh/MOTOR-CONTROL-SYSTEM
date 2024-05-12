/****************************************************************************************************/
/* Module Name : General-Purpose Timer Moduel  ( header file )                                      */
/*												    */
/* Author      : Team 1                                                                             */
/*												    */
/* Purpose     :  This header file defines function prototypes for configuring and controlling	    */
/*		  General-Purpose Timers on a microcontroller. Users can include this file in their */
/*		  projects to interact with timer modules based on their specific timing 	    */
/*		  requirements. It declares functions for initializing timers, setting time 	    */
/*		  intervals, enabling disabling timers, and configuring interrupt handling for      */
/*				  timers.							    */
/*												    */
/*												    */
/****************************************************************************************************/

#ifndef GPTM_H__
#define GPTM_H_

#include "tm4c123gh6pm.h"
#include "types.h"
#include "gpio.h"
#include "bitwise_operation.h"
/*******************************PIN DEFINITATION*******************************/

#define TMRA_EN_PIN           0
#define TMRB_EN_PIN           8
#define TRIGGER_EN_PIN        6
#define SNAPSHOT_EN_PIN       7           
#define TMRA_FLAG_PIN         0
#define TMRB_FLAG_PIN         8
#define TMRA_INT_CLEAR_PIN    0
#define TMRB_INT_CLEAR_PIN    8


/***************************************************************************/

/************************************TYPDEF*********************************/

typedef enum {
  TIMER_A , TIMER_B , TIMER_CONC
}TI_A_B;

typedef enum {
  TIMER0 ,  TIMER1 ,  TIMER2 ,  TIMER3 ,  TIMER4 ,  TIMER5 ,
 WTIMER0 , WTIMER1 , WTIMER2 , WTIMER3 , WTIMER4 , WTIMER5
}TIMER_ID;

typedef enum {
	RESERVED = 0x00 , ONE_SHOT = 0x01  , PERIDOIC = 0x02 , CAPTURE = 0x03 
}TIMER_MODE;

typedef enum {
	BIT_16_MODE = 0x4 , BIT_32_MODE = 0x0 , BIT_32_RTC_MODE = 0x1 
}TIMER_BIT_MODE;

typedef enum {
	COUNT_DOWN = 0x0 , COUNT_UP = 0x10 
}COUNT_MODE;

typedef enum {
	DISABLE_INTERRUPT = 0x00 , ENABLE_INTERRUPT = 0x01 
}INTERRUPT_EN;

typedef enum {
        PRESCALE_1   =  0x00    ,
        PRESCALE_2   =  0x02    ,
        PRESCALE_3   =  0x03    ,
        PRESCALE_254 =  0xFD    ,
        PRESCALE_255 =  0xFE    ,
        PRESCALE_256 =  0xFF 
}PRESCALE_V;

/******************************UART COFIGN*************************************/

typedef struct {
	uint64          interval_load ;         
        INTERRUPT_EN    int_en        ;
	COUNT_MODE      count_m       ;
	TIMER_MODE      mode          ;
        TIMER_BIT_MODE  bitMode       ;
        PRESCALE_V      prescale      ;
} Timer_ConfigType;


/*******************************************Functions******************************************************/


/*****************************************************************************************************/
/* Function Name :  Timer_Init																		 */
/* Inputs        :  TIMER_ID ( TIMER ) , TI_A_B ( TIM ) ,  const Timer_ConfigType * Config_Ptr)      */
/* Outputs       :  void ( No outputs )																 */
/* Reentrancy    :  Reentrant																		 */
/* Synchronous   :  Asynchronous  																	 */
/* Description   :	This function initializes and configures the specified General-Purpose			 */
/* 					Timer (GPTM) module and its operating mode based on the provided 				 */
/*					configuration structure. It enables the necessary clock and sets the mode,		 */
/*					interval, and other parameters for Timer A or Timer B as specified.				 */
/*					The function also handles interrupt configuration, enabling and clearing		 */
/*					based on user preferences.				 										 */
/*****************************************************************************************************/
void Timer_init( TIMER_ID TIMER , TI_A_B TIM , const Timer_ConfigType * Config_Ptr);




/*****************************************************************************************************/
/* Function Name :  Timer_deInit																	 */
/* Inputs        :  TIMER_ID ( TIMER )                                                               */
/* Outputs       :  void ( No outputs )																 */
/* Reentrancy    :  Reentrant																		 */
/* Synchronous   :  Synchronous  																	 */
/* Description   :	This function deinitializes and disables the clock for the specified  			 */
/*					General-Purpose Timer (GPTM) module. It is designed to halt the operation		 */
/*					of the timer and can be used during system shutdown								 */
/*					or when the timer is no longer needed to conserve power.						 */
/*****************************************************************************************************/
void Timer_deInit ( TIMER_ID TIMER ) ;
















#endif
