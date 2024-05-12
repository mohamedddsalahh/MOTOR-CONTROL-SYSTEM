/****************************************************************************************************/
/* Module Name : General-Purpose Timer Moduel  ( source file )                                      */
/*																									*/
/* Author      : Team 1                                                                             */
/*																									*/
/* Purpose     :   This source file provides the implementation for configuring and controlling	    */
/* 				   General-Purpose Timers on a microcontroller. It includes functions for			*/ 
/*				   initializin timers, setting time intervals, enabling/disabling timers, 			*/
/*				   and configuring interrupt handling for timers. Users can include this 			*/
/*				   file in their projects to interact with timer modules based on their specific 	*/
/*				   timing requirements																*/
/*																									*/
/*																									*/
/****************************************************************************************************/


#include "GPTM.h" 



/************************************  Generic Variables   ******************************************/
//uint8 Error_flag = 0;
//uint8 CutOFF_Flag = 0;



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
void Timer_init( TIMER_ID TIMER , TI_A_B TIM , const Timer_ConfigType * Config_Ptr)
{
  /* Enable and provide a clock to the analog comparator module in sleep mode  */
  SET_BIT ( SYSCTL_SCGCACMP_R  , 0 );
  
  switch(TIMER)
  {
/**********************************TIMER0**************************************/
    case TIMER0 :
      /* Enable and provide a clock to 16/32-bit general-purpose timer     */
      SET_BIT ( SYSCTL_RCGCTIMER_R , TIMER0 );
      
      /* for timer A-------------------------------------------------> */
      /* SET_BIT ( SYSCTL_RCGCGPIO_R  , TIMER0 );                      */
       
      /* Timer BIT mode : Register GPTM Configuration					*/
      /*  to set mode bit  , three bits --> bit 0,  bit 1 , bit 3		*/
      /*  16-bit        --> set bits 0									*/
      /*  32-bit RTC    --> set bits 0x1								*/
	  /*  32-bit        --> set bits 0x4								*/
	  /*																*/
       TIMER0_CFG_R = Config_Ptr->bitMode ;  

      switch(TIM)
      {
      /*********************************TIMER0-A********************************/
      case TIMER_A :
        
        /*		disable timer A  	*/
         CLEAR_BIT ( TIMER0_CTL_R , TMRA_EN_PIN );
         
         /* Timer A mode : Register GPTM Timer A Mode -					*/
		 /*  to set mode for timer a  , two bits --> bit 0,  bit 1		*/
         /*  Capture  --> set bits 0x3									*/
         /*  Periodic --> set bits 0x2									*/
         /*  One-shot --> set bits 0x1									*/
         /*  Reserved --> set bits 0x0									*/
         TIMER0_TAMR_R = ( TIMER0_TAMR_R & 0xFFFFFFFC ) | Config_Ptr->mode ;
         
         /* Timer A Snapshot : Register GPTM Timer A Mode - bit 7
          *  bit 7 = 0 -> snapshot disabled
          *  bit 7 = 1 -> snapshot enabled         
          */
         SET_BIT ( TIMER0_TAMR_R , SNAPSHOT_EN_PIN );
         
          /* Timer A trigger START : Register GPTM Timer A Mode - bit 6		*/
          /*  bit 6 = 0 -> start count as the timer is enabled				*/
          /*  bit 6 = 1 -> waits for trigger to start cound       			*/
         CLEAR_BIT ( TIMER0_TAMR_R ,TRIGGER_EN_PIN );
         
         /* Timer A Interrup enable  : Register GPTM Interrupt Mask 		*/
         /*  bit 0 = 0 -> disable interrupt									*/
         /*  bit 0 = 1 -> enable interrupt    								*/
         TIMER0_IMR_R = ( TIMER0_IMR_R & 0xFFFFFFFE ) | Config_Ptr->int_en ;
         
         
         /* Timer A count mode : Register GPTM Timer A Mode - bit 4			*/
         /*  bit 4 = 0 -> Count down										*/
         /*  bit 4 = 1 -> Count up       									*/
         TIMER0_TAMR_R = ( TIMER0_TAMR_R & 0xFFFFFFEF ) | Config_Ptr->count_m ;
         
         /* Timer A load value  : Register GPTM Timer A Interval Load 32-bit       	*/
         TIMER0_TAILR_R = Config_Ptr->interval_load ;
         
         /*		put prescale value in the Prescale Register                  */
         TIMER0_TAPR_R = Config_Ptr->prescale ;
         
         /* Set Clear-Interrupt bit to clear flag in Raw Interrupt Status register */
         SET_BIT ( TIMER0_ICR_R , TMRA_INT_CLEAR_PIN );
         
         
         /* Enable interrupt in NVIC  	*/ 
         NVIC_EN0_R |= ( Config_Ptr->int_en << 19 );
         
         /*		Start Timer A 		*/
         SET_BIT ( TIMER0_CTL_R , TMRA_EN_PIN );
        
         
         /* Check if the interrupt is enabled to skip polling		*/
         if ( Config_Ptr->int_en == DISABLE_INTERRUPT )
         {
           /*	Polling over Time out Flag in Raw interrupt Status register */
           while( GET_BIT( TIMER0_RIS_R , TMRA_FLAG_PIN ) == 0 ){};
           
           /*	Set Clear-Interrupt bit to clear flag in Raw Interrupt Status register */
          SET_BIT ( TIMER0_ICR_R , TMRA_INT_CLEAR_PIN );
         }
       
         
        break; 
      /*********************************TIMER0-B*********************************/  
      case TIMER_B :
        
        /* disable timer B			*/
        CLEAR_BIT ( TIMER0_CTL_R , TMRB_EN_PIN );
        
        /* Timer B mode : Register GPTM Timer B Mode -					*/
        /*  to set mode for timer B  , two bits --> bit 0,  bit 1		*/
        /*  Capture  --> set bits 0x3									*/
        /*  Periodic --> set bits 0x2									*/
        /*  One-shot --> set bits 0x1									*/
        /*  Reserved --> set bits 0x0									*/
         TIMER0_TBMR_R = ( TIMER0_TBMR_R & 0xFFFFFFFC ) | Config_Ptr->mode ;
         
         /* Timer B Snapshot : Register GPTM Timer B Mode - bit 7	*/
         /*  bit 7 = 0 -> snapshot disabled							*/
         /*  bit 7 = 1 -> snapshot enabled         					*/
         SET_BIT ( TIMER0_TBMR_R , SNAPSHOT_EN_PIN );
         
         /* Timer B trigger START : Register GPTM Timer B Mode - bit 6		*/
         /*  bit 6 = 0 -> start count as the timer is enabled				*/
         /*  bit 6 = 1 -> waits for trigger to start cound       			*/
         CLEAR_BIT ( TIMER0_TBMR_R ,TRIGGER_EN_PIN );
         
         /* Timer A Interrup enable  : Register GPTM Interrupt Mask 		*/
         /*  bit 0 = 0 -> disable interrupt									*/
         /*  bit 0 = 1 -> enable interrupt    								*/
         TIMER0_IMR_R = ( TIMER0_IMR_R & 0xFFFFFFFE ) |( Config_Ptr->int_en << 8 ) ;
        
         
         /* Timer B count mode : Register GPTM Timer B Mode - bit 4			*/
         /*  bit 4 = 0 -> Count down										*/
         /*  bit 4 = 1 -> Count up      									*/
         TIMER0_TAMR_R = ( TIMER0_TAMR_R & 0xFFFFFFEF ) | Config_Ptr->count_m ;
         
         /* Timer A load value  : Register GPTM Timer A Interval Load 32-bit       	*/
         TIMER0_TAILR_R = Config_Ptr->interval_load ;
         
         /*	put prescale value in the Prescale Register		*/
         TIMER0_TBPR_R = Config_Ptr->prescale ;
        
         
         /*	 Set Clear-Interrupt bit to clear flag in Raw Interrupt Status register */
         SET_BIT ( TIMER0_ICR_R , TMRB_INT_CLEAR_PIN );
         
         /*	Enable interrupt in NVIC		*/ 
         NVIC_EN0_R |= ( Config_Ptr->int_en << 20 );
         
         /* 	Start Timer B	*/
         SET_BIT ( TIMER0_CTL_R , TMRB_EN_PIN );
        
         
         if ( Config_Ptr->int_en == 0 )
         {
           while( GET_BIT( TIMER0_RIS_R , TMRB_FLAG_PIN ) == 0 ){};
           
         /*	Set Clear-Interrupt bit to clear flag in Raw Interrupt Status register	*/
         SET_BIT ( TIMER0_ICR_R , TMRB_INT_CLEAR_PIN );
         }
         
        break; 
      case TIMER_CONC :
        break; 
      }
      break; 
/************************************TIMER 1*************************************/
    case TIMER1 :
      /*	Enable and provide a clock to 16/32-bit general-purpose timer 	*/
      SET_BIT ( SYSCTL_RCGCTIMER_R , TIMER1 );
      
      /* for timer A------------------------------------------------->   */
      /*SET_BIT ( SYSCTL_RCGCGPIO_R  , TIMER0 );						 */
      
      /* Timer BIT mode : Register GPTM Configuration					*/
      /*  to set mode bit  , three bits --> bit 0,  bit 1 , bit 2		*/
      /*  16-bit        --> set bits 0X4								*/
      /*  32-bit RTC    --> set bits 0x1								*/
      /*  32-bit        --> set bits 0x0								*/
       TIMER1_CFG_R = Config_Ptr->bitMode ;  

      switch(TIM)
      {
      /******************************TIMER 1-A********************************/
      case TIMER_A :
        
        /*	disable timer A			*/ 
         CLEAR_BIT ( TIMER1_CTL_R , TMRA_EN_PIN );
         
         /* Timer A mode : Register GPTM Timer A Mode -					*/
         /*  to set mode for timer a  , two bits --> bit 0,  bit 1		*/
         /*  Capture  --> set bits 0x3									*/
         /*  Periodic --> set bits 0x2									*/
         /*  One-shot --> set bits 0x1									*/
         /*  Reserved --> set bits 0x0									*/
         TIMER1_TAMR_R = ( TIMER1_TAMR_R & 0xFFFFFFFC ) | Config_Ptr->mode ;
         
         /* Timer A Snapshot : Register GPTM Timer A Mode - bit 7		*/
         /*  bit 7 = 0 -> snapshot disabled								*/
         /*  bit 7 = 1 -> snapshot enabled         						*/
         SET_BIT ( TIMER1_TAMR_R , SNAPSHOT_EN_PIN );	
         
          /* Timer A trigger START : Register GPTM Timer A Mode - bit 6			*/
          /*  bit 6 = 0 -> start count as the timer is enabled					*/
          /* bit 6 = 1 -> waits for trigger to start count						*/       
         CLEAR_BIT ( TIMER1_TAMR_R ,TRIGGER_EN_PIN );
         
         /* Timer A Interrup enable  : Register GPTM Interrupt Mask 	*/
         /* bit 0 = 0 -> disable interrupt								*/
         /*  bit 0 = 1 -> enable interrupt    							*/
         TIMER1_IMR_R = ( TIMER1_IMR_R & 0xFFFFFFFE ) | Config_Ptr->int_en ;
         
         
         /* Timer A count mode : Register GPTM Timer A Mode - bit 4		*/
         /*  bit 4 = 0 -> Count down									*/
         /*  bit 4 = 1 -> Count up       								*/
         TIMER1_TAMR_R = ( TIMER1_TAMR_R & 0xFFFFFFEF ) | Config_Ptr->count_m ;
         
         /* Timer A load value  : Register GPTM Timer A Interval Load 32-bit       */
         TIMER1_TAILR_R = Config_Ptr->interval_load ;
         
         /*	put prescale value in the Prescale Register						*/
         TIMER1_TAPR_R = Config_Ptr->prescale ;
        
         
         /*	Set Clear-Interrupt bit to clear flag in Raw Interrupt Status register	*/
         SET_BIT ( TIMER1_ICR_R , TMRA_INT_CLEAR_PIN );
         
        /* 	Enable interrupt in NVIC 				*/
         NVIC_EN0_R |= ( Config_Ptr->int_en << 21 );
         
         /* Start Timer A 			*/
         SET_BIT ( TIMER1_CTL_R , TMRA_EN_PIN );
        
         
         /*	Check if the interrupt is enabled to skip polling 			*/
         if ( Config_Ptr->int_en == 0 )
         {
           /* Polling over Time out Flag in Raw interrupt Status register	*/
           while( GET_BIT( TIMER1_RIS_R , TMRA_FLAG_PIN ) == 0 ){};
           
           /*	Set Clear-Interrupt bit to clear flag in Raw Interrupt Status register	*/
           SET_BIT ( TIMER1_ICR_R , TMRA_INT_CLEAR_PIN );
         }
         
        break; 
      case TIMER_B :
      /*********************************TIMER1-B*******************************/ 
        /*	disable timer B				*/
        CLEAR_BIT ( TIMER1_CTL_R , TMRB_EN_PIN );
        
        /* Timer B mode : Register GPTM Timer B Mode - */
        /*  to set mode for timer B  , two bits --> bit 0,  bit 1		*/
        /*  Capture  --> set bits 0x3									*/
        /*  Periodic --> set bits 0x2									*/
        /*  One-shot --> set bits 0x1									*/	
        /*  Reserved --> set bits 0x0									*/
         TIMER1_TBMR_R = ( TIMER1_TBMR_R & 0xFFFFFFFC ) | Config_Ptr->mode ;
         
         /* Timer B Snapshot : Register GPTM Timer B Mode - bit 7		*/
         /*  bit 7 = 0 -> snapshot disabled								*/
         /*  bit 7 = 1 -> snapshot enabled         						*/
         SET_BIT ( TIMER1_TBMR_R , SNAPSHOT_EN_PIN );
         
         /* Timer B trigger START : Register GPTM Timer B Mode - bit 6		*/
         /*  bit 6 = 0 -> start count as the timer is enabled				*/
         /*  bit 6 = 1 -> waits for trigger to start cound       			*/
         CLEAR_BIT ( TIMER1_TBMR_R ,TRIGGER_EN_PIN );
         
         /* Timer A Interrup enable  : Register GPTM Interrupt Mask 		*/
         /*  bit 0 = 0 -> disable interrupt									*/
         /*  bit 0 = 1 -> enable interrupt    								*/
         TIMER1_IMR_R = ( TIMER1_IMR_R & 0xFFFFFFFE ) | (Config_Ptr->int_en << 8 ) ;
        
         
         /* Timer B count mode : Register GPTM Timer B Mode - bit 4			*/
         /*  bit 4 = 0 -> Count down										*/
         /*  bit 4 = 1 -> Count up       									*/
         TIMER1_TAMR_R = ( TIMER1_TAMR_R & 0xFFFFFFEF ) | Config_Ptr->count_m ;
         
         /* Timer A load value  : Register GPTM Timer A Interval Load 32-bit       */
         TIMER1_TAILR_R = Config_Ptr->interval_load ;
        
         /*	put prescale value in the Prescale Register				*/
         TIMER1_TBPR_R = Config_Ptr->prescale ;
           
         /*	Set Clear-Interrupt bit to clear flag in Raw Interrupt Status register */
         SET_BIT ( TIMER1_ICR_R , TMRB_INT_CLEAR_PIN );
         
         /*	Enable interrupt in NVIC 		*/
         NVIC_EN0_R |= ( Config_Ptr->int_en << 22 );
         
         /*		Start Timer B		*/
         SET_BIT ( TIMER1_CTL_R , TMRB_EN_PIN );
         
         if ( Config_Ptr->int_en == 0 )
         {
           while( GET_BIT( TIMER1_RIS_R , TMRB_FLAG_PIN ) == 0 ){};
           
           /*	Set Clear-Interrupt bit to clear flag in Raw Interrupt Status register	*/
         SET_BIT ( TIMER1_ICR_R , TMRB_INT_CLEAR_PIN );
         }
         
         
        break; 
      case TIMER_CONC :
        break; 
      }
      break; 
      
/**************************************TIMER 2********************************/      
   case TIMER2 :
      /*	Enable and provide a clock to 16/32-bit general-purpose timer	*/
      SET_BIT ( SYSCTL_RCGCTIMER_R , TIMER2 );
      
      /*	for timer A-------------------------------------------------> */
      /*	SET_BIT ( SYSCTL_RCGCGPIO_R  , TIMER0 ); 					  */
      TIMER2_CFG_R = Config_Ptr->bitMode ;  

      switch(TIM)
      {
      /***************************TIMER2-A*************************************/
      case TIMER_A :
        
        //disable timer A 
         CLEAR_BIT ( TIMER2_CTL_R , TMRA_EN_PIN );
         
         /* Timer A mode : Register GPTM Timer A Mode -					*/
         /*  to set mode for timer a  , two bits --> bit 0,  bit 1		*/	
         /*  Capture  --> set bits 0x3									*/
         /*  Periodic --> set bits 0x2									*/
         /*  One-shot --> set bits 0x1									*/
         /*  Reserved --> set bits 0x0									*/
         TIMER2_TAMR_R = ( TIMER2_TAMR_R & 0xFFFFFFFC ) | Config_Ptr->mode ;
         
         /* Timer A Snapshot : Register GPTM Timer A Mode - bit 7		*/
         /*  bit 7 = 0 -> snapshot disabled								*/
         /*  bit 7 = 1 -> snapshot enabled        						*/
         SET_BIT ( TIMER2_TAMR_R , SNAPSHOT_EN_PIN );
         
          /* Timer A trigger START : Register GPTM Timer A Mode - bit 6		 */
          /*  bit 6 = 0 -> start count as the timer is enabled				 */
          /*  bit 6 = 1 -> waits for trigger to start cound       			 */
         CLEAR_BIT ( TIMER2_TAMR_R ,TRIGGER_EN_PIN );
         
         /* Timer A Interrup enable  : Register GPTM Interrupt Mask 	*/
         /*  bit 0 = 0 -> disable interrupt								*/
         /*  bit 0 = 1 -> enable interrupt    							*/
         TIMER2_IMR_R = ( TIMER2_IMR_R & 0xFFFFFFFE ) | Config_Ptr->int_en ;
         
         
         /* Timer A count mode : Register GPTM Timer A Mode - bit 4			*/
         /*  bit 4 = 0 -> Count down										*/
         /*  bit 4 = 1 -> Count up       									*/
         TIMER2_TAMR_R = ( TIMER2_TAMR_R & 0xFFFFFFEF ) | Config_Ptr->count_m ;
         
         /* Timer A load value  : Register GPTM Timer A Interval Load 32-bit       */
         TIMER2_TAILR_R = Config_Ptr->interval_load ;
         
         /* put prescale value in the Prescale Register     */
         TIMER2_TAPR_R = Config_Ptr->prescale ;
         
         
         /*	Set Clear-Interrupt bit to clear flag in Raw Interrupt Status register  */
         SET_BIT ( TIMER2_ICR_R , TMRA_INT_CLEAR_PIN );
         
         /* Enable interrupt in NVIC  */
         NVIC_EN0_R |= ( Config_Ptr->int_en << 23 );
         
         /*  Start Timer A   */
         SET_BIT ( TIMER2_CTL_R , TMRA_EN_PIN );
         
         
         /* Check if the interrupt is enabled to skip polling	*/
         if ( Config_Ptr->int_en == 0 )
         {
           /* Polling over Time out Flag in Raw interrupt Status register */
           while( GET_BIT( TIMER2_RIS_R , TMRA_FLAG_PIN ) == 0 ){};
           
           
           /* Set Clear-Interrupt bit to clear flag in Raw Interrupt Status register  */
          SET_BIT ( TIMER2_ICR_R , TMRA_INT_CLEAR_PIN );
         }
         
        break;
       
      /***********************************TIMER2-B******************************/  
      case TIMER_B :
        
        /* 	disable timer B			*/
        CLEAR_BIT ( TIMER2_CTL_R , TMRB_EN_PIN );
        
        /* Timer B mode : Register GPTM Timer B Mode -					*/
        /*  to set mode for timer B  , two bits --> bit 0,  bit 1		*/
        /*  Capture  --> set bits 0x3									*/
        /*  Periodic --> set bits 0x2									*/
        /*  One-shot --> set bits 0x1									*/
        /*  Reserved --> set bits 0x0									*/
         TIMER2_TBMR_R = ( TIMER2_TBMR_R & 0xFFFFFFFC ) | Config_Ptr->mode ;
         
         /* Timer B Snapshot : Register GPTM Timer B Mode - bit 7		*/
         /*  bit 7 = 0 -> snapshot disabled								*/
         /*  bit 7 = 1 -> snapshot enabled         						*/
         SET_BIT ( TIMER2_TBMR_R , SNAPSHOT_EN_PIN );
         
         /* Timer B trigger START : Register GPTM Timer B Mode - bit 6		*/
         /*  bit 6 = 0 -> start count as the timer is enabled				*/
         /*  bit 6 = 1 -> waits for trigger to start cound       			*/
         CLEAR_BIT ( TIMER2_TBMR_R ,TRIGGER_EN_PIN );
         
         /* Timer A Interrupt enable  : Register GPTM Interrupt Mask		*/ 
         /*  bit 0 = 0 -> disable interrupt									*/
         /*  bit 0 = 1 -> enable interrupt    								*/
         TIMER2_IMR_R = ( TIMER2_IMR_R & 0xFFFFFFFE ) | ( Config_Ptr->int_en << 8 );
        
         
         /* Timer B count mode : Register GPTM Timer B Mode - bit 4		*/
         /*  bit 4 = 0 -> Count down									*/
         /*  bit 4 = 1 -> Count up       								*/
         TIMER2_TAMR_R = ( TIMER2_TAMR_R & 0xFFFFFFEF ) | Config_Ptr->count_m ;
         
         /* Timer A load value  : Register GPTM Timer A Interval Load 32-bit       	*/
         TIMER2_TAILR_R = Config_Ptr->interval_load ;
        
         /* put prescale value in the Prescale Register		*/
         TIMER2_TBPR_R = Config_Ptr->prescale ;
        
         /* Set Clear-Interrupt bit to clear flag in Raw Interrupt Status register */
         SET_BIT ( TIMER2_ICR_R , TMRB_INT_CLEAR_PIN );
         
         /* Enable interrupt in NVIC 		*/
         NVIC_EN0_R |= ( Config_Ptr->int_en << 24 );
         
         /* Start Timer B		*/
         SET_BIT ( TIMER2_CTL_R , TMRB_EN_PIN );
        
         
         if ( Config_Ptr->int_en == 0 )
         {
           while( GET_BIT( TIMER2_RIS_R , TMRB_FLAG_PIN ) == 0 ){};
           
           /* Set Clear-Interrupt bit to clear flag in Raw Interrupt Status register		*/
           SET_BIT ( TIMER2_ICR_R , TMRB_INT_CLEAR_PIN );
         }
         
         
        break; 
      case TIMER_CONC :
        break; 
      }
      
      break; 
/*************************************TIMER 3***********************************/      
   case TIMER3 :
      /* 	Enable and provide a clock to 16/32-bit general-purpose timer */
      SET_BIT ( SYSCTL_RCGCTIMER_R , TIMER3);
      
      /* for timer A------------------------------------------------->	*/
      /* SET_BIT ( SYSCTL_RCGCGPIO_R  , TIMER0 );						*/
      TIMER3_CFG_R = Config_Ptr->bitMode ;  

      switch(TIM)
      {
      /**************************TIMER3-A**************************************/  
      case TIMER_A :
        
        //disable timer A 
         CLEAR_BIT ( TIMER3_CTL_R , TMRA_EN_PIN );
         
         /* Timer A mode : Register GPTM Timer A Mode -							*/
         /*  to set mode for timer a  , two bits --> bit 0,  bit 1				*/
       	 /*  Capture  --> set bits 0x3											*/
         /*  Periodic --> set bits 0x2											*/
         /*  One-shot --> set bits 0x1											*/
         /*  Reserved --> set bits 0x0											*/
         TIMER3_TAMR_R = ( TIMER3_TAMR_R & 0xFFFFFFFC ) | Config_Ptr->mode ;	
         
         /* Timer A Snapshot : Register GPTM Timer A Mode - bit 7	*/
         /* bit 7 = 0 -> snapshot disabled							*/
         /*  bit 7 = 1 -> snapshot enabled         					*/	
         SET_BIT ( TIMER3_TAMR_R , SNAPSHOT_EN_PIN );
         
          /* Timer A trigger START : Register GPTM Timer A Mode - bit 6	*/
          /*  bit 6 = 0 -> start count as the timer is enabled			*/
          /* bit 6 = 1 -> waits for trigger to start cound       		*/
         CLEAR_BIT ( TIMER3_TAMR_R ,TRIGGER_EN_PIN );
         
         /* Timer A Interrup enable  : Register GPTM Interrupt Mask 	*/
         /*  bit 0 = 0 -> disable interrupt								*/
         /*  bit 0 = 1 -> enable interrupt    							*/
         TIMER3_IMR_R = ( TIMER3_IMR_R & 0xFFFFFFFE ) | Config_Ptr->int_en ;
         
         
         /* Timer A count mode : Register GPTM Timer A Mode - bit 4		*/
         /*  bit 4 = 0 -> Count down									*/
         /*  bit 4 = 1 -> Count up       								*/
         TIMER3_TAMR_R = ( TIMER3_TAMR_R & 0xFFFFFFEF ) | Config_Ptr->count_m ;
         
         /* Timer A load value  : Register GPTM Timer A Interval Load 32-bit       */
         TIMER3_TAILR_R = Config_Ptr->interval_load ;
         
         /* put prescale value in the Prescale Register	*/
         TIMER3_TAPR_R = Config_Ptr->prescale ;
         
         /* Set Clear-Interrupt bit to clear flag in Raw Interrupt Status register */
         SET_BIT ( TIMER3_ICR_R , TMRA_INT_CLEAR_PIN );
         
         
        /* Enable interrupt in NVIC 	*/
         NVIC_EN1_R |= ( Config_Ptr->int_en << 3 );
         
         /* Start Timer A  */
         SET_BIT ( TIMER3_CTL_R , TMRA_EN_PIN );
         
         
         /* Check if the interrupt is enabled to skip polling	*/
         if ( Config_Ptr->int_en == 0 )
         {
           /* Polling over Time out Flag in Raw interrupt Status register */
           while( GET_BIT( TIMER3_RIS_R , TMRA_FLAG_PIN ) == 0 ){};
           
           /* Set Clear-Interrupt bit to clear flag in Raw Interrupt Status register */
          SET_BIT ( TIMER3_ICR_R , TMRA_INT_CLEAR_PIN );
         }
         
        break; 
      /**********************************TIMER3-B******************************/  
      case TIMER_B :
        
        /* disable timer B		*/
        CLEAR_BIT ( TIMER3_CTL_R , TMRB_EN_PIN );
        
        /* Timer B mode : Register GPTM Timer B Mode -					*/
        /*  to set mode for timer B  , two bits --> bit 0,  bit 1		*/
        /*  Capture  --> set bits 0x3									*/
        /*  Periodic --> set bits 0x2									*/
        /*  One-shot --> set bits 0x1									*/
        /*  Reserved --> set bits 0x0									*/	
         TIMER3_TBMR_R = ( TIMER3_TBMR_R & 0xFFFFFFFC ) | Config_Ptr->mode ;
         
         /* Timer B Snapshot : Register GPTM Timer B Mode - bit 7		*/
         /*  bit 7 = 0 -> snapshot disabled								*/
         /*  bit 7 = 1 -> snapshot enabled         						*/
         SET_BIT ( TIMER3_TBMR_R , SNAPSHOT_EN_PIN );
         
         /* Timer B trigger START : Register GPTM Timer B Mode - bit 6		*/
         /*  bit 6 = 0 -> start count as the timer is enabled				*/
         /*  bit 6 = 1 -> waits for trigger to start cound       			*/
         CLEAR_BIT ( TIMER3_TBMR_R ,TRIGGER_EN_PIN );
         
         /* Timer A Interrup enable  : Register GPTM Interrupt Mask 	*/
         /*  bit 0 = 0 -> disable interrupt								*/
         /*  bit 0 = 1 -> enable interrupt    							*/
         TIMER3_IMR_R = ( TIMER3_IMR_R & 0xFFFFFFFE ) | ( Config_Ptr->int_en << 8 );
        
         
         /* Timer B count mode : Register GPTM Timer B Mode - bit 4		*/
         /*  bit 4 = 0 -> Count down									*/
         /*  bit 4 = 1 -> Count up       								*/
         TIMER3_TAMR_R = ( TIMER3_TAMR_R & 0xFFFFFFEF ) | Config_Ptr->count_m ;
         
         /* Timer A load value  : Register GPTM Timer A Interval Load 32-bit       	*/
         TIMER3_TAILR_R = Config_Ptr->interval_load ;
        
         /* put prescale value in the Prescale Register		*/
         TIMER3_TBPR_R = Config_Ptr->prescale ;
        
         /* Set Clear-Interrupt bit to clear flag in Raw Interrupt Status register  */
         SET_BIT ( TIMER3_ICR_R , TMRB_INT_CLEAR_PIN );
         
        /*	Enable interrupt in NVIC */
         NVIC_EN1_R |= ( Config_Ptr->int_en << 4 );
        
         /* 	Start Timer B 	*/
         SET_BIT ( TIMER3_CTL_R , TMRB_EN_PIN );
        
         
         if ( Config_Ptr->int_en == 0 )
         {
           while( GET_BIT( TIMER3_RIS_R , TMRB_FLAG_PIN ) == 0 ){};
           
         /* Set Clear-Interrupt bit to clear flag in Raw Interrupt Status register */
         SET_BIT ( TIMER3_ICR_R , TMRB_INT_CLEAR_PIN );
         
         }
         
         
        break; 
      case TIMER_CONC :
        break; 
      }
      
      break; 
/***********************************TIMER 4************************************/
   case TIMER4 :
      /* Enable and provide a clock to 16/32-bit general-purpose timer */
      SET_BIT ( SYSCTL_RCGCTIMER_R , TIMER4 );
      
      /* for timer A-------------------------------------------------> */
      /* SET_BIT ( SYSCTL_RCGCGPIO_R  , TIMER0 ); 					   */
      TIMER4_CFG_R = Config_Ptr->bitMode ;  

      switch(TIM)
      {
      /*****************************TIMER4-A***********************************/
      case TIMER_A :
        
        /* disable timer A 			*/
         CLEAR_BIT ( TIMER4_CTL_R , TMRA_EN_PIN );
         
         /* Timer A mode : Register GPTM Timer A Mode -					*/
         /*  to set mode for timer a  , two bits --> bit 0,  bit 1		*/
         /*  Capture  --> set bits 0x3									*/
         /*  Periodic --> set bits 0x2									*/
         /*  One-shot --> set bits 0x1									*/	
         /*  Reserved --> set bits 0x0									*/
         TIMER4_TAMR_R = ( TIMER4_TAMR_R & 0xFFFFFFFC ) | Config_Ptr->mode ;
         
         /* Timer A Snapshot : Register GPTM Timer A Mode - bit 7	*/
         /*  bit 7 = 0 -> snapshot disabled							*/	
         /*  bit 7 = 1 -> snapshot enabled         					*/
         SET_BIT ( TIMER4_TAMR_R , SNAPSHOT_EN_PIN );
         
          /* Timer A trigger START : Register GPTM Timer A Mode - bit 6		*/
          /*  bit 6 = 0 -> start count as the timer is enabled				*/
          /*  bit 6 = 1 -> waits for trigger to start cound       			*/
         CLEAR_BIT ( TIMER4_TAMR_R ,TRIGGER_EN_PIN );
         
         /* Timer A Interrup enable  : Register GPTM Interrupt Mask 		*/
         /*  bit 0 = 0 -> disable interrupt									*/
         /*  bit 0 = 1 -> enable interrupt    								*/
         TIMER4_IMR_R = ( TIMER4_IMR_R & 0xFFFFFFFE ) | Config_Ptr->int_en ;
         
         
         /* Timer A count mode : Register GPTM Timer A Mode - bit 4			*/
         /*  bit 4 = 0 -> Count down										*/
         /*  bit 4 = 1 -> Count up       									*/
         TIMER4_TAMR_R = ( TIMER4_TAMR_R & 0xFFFFFFEF ) | Config_Ptr->count_m ;
         
         /* Timer A load value  : Register GPTM Timer A Interval Load 32-bit      */
         TIMER4_TAILR_R = Config_Ptr->interval_load ;
         
         /* put prescale value in the Prescale Register	*/
         TIMER4_TAPR_R = Config_Ptr->prescale ;
         
         
         /* Set Clear-Interrupt bit to clear flag in Raw Interrupt Status register */
         SET_BIT ( TIMER4_ICR_R , TMRA_INT_CLEAR_PIN );
         
        /* Enable interrupt in NVIC  */
         NVIC_EN2_R |= ( Config_Ptr->int_en << 6 );
         
         /* Start Timer A 	*/
         SET_BIT ( TIMER4_CTL_R , TMRA_EN_PIN );
         
         
         /* Check if the interrupt is enabled to skip polling		*/
         if ( Config_Ptr->int_en == 0 )
         {
           /* Polling over Time out Flag in Raw interrupt Status register	*/
           while( GET_BIT( TIMER4_RIS_R , TMRA_FLAG_PIN ) == 0 ){};
           
           /* Set Clear-Interrupt bit to clear flag in Raw Interrupt Status register */
          SET_BIT ( TIMER4_ICR_R , TMRA_INT_CLEAR_PIN );
         }
         
        break;
      /********************************TIMER4-B********************************/  
      case TIMER_B :
        
        /* disable timer B */
        CLEAR_BIT ( TIMER4_CTL_R , TMRB_EN_PIN );
        
        /* Timer B mode : Register GPTM Timer B Mode - */
        /*  to set mode for timer B  , two bits --> bit 0,  bit 1		*/
        /*  Capture  --> set bits 0x3									*/
    	/*  Periodic --> set bits 0x2									*/
        /*  One-shot --> set bits 0x1									*/
    	/*  Reserved --> set bits 0x0									*/	
         TIMER4_TBMR_R = ( TIMER4_TBMR_R & 0xFFFFFFFC ) | Config_Ptr->mode ;
         
         /* Timer B Snapshot : Register GPTM Timer B Mode - bit 7	*/
         /*  bit 7 = 0 -> snapshot disabled							*/
         /*  bit 7 = 1 -> snapshot enabled         					*/
         SET_BIT ( TIMER4_TBMR_R , SNAPSHOT_EN_PIN );
         
         /* Timer B trigger START : Register GPTM Timer B Mode - bit 6		*/
         /*  bit 6 = 0 -> start count as the timer is enabled				*/
         /*  bit 6 = 1 -> waits for trigger to start cound       			*/
         CLEAR_BIT ( TIMER4_TBMR_R ,TRIGGER_EN_PIN );
         
         /* Timer A Interrup enable  : Register GPTM Interrupt Mask 		*/
         /*  bit 0 = 0 -> disable interrupt									*/
         /*  bit 0 = 1 -> enable interrupt    								*/
         TIMER4_IMR_R = ( TIMER4_IMR_R & 0xFFFFFFFE ) | ( Config_Ptr->int_en << 8 ) ;
        
         
         /* Timer B count mode : Register GPTM Timer B Mode - bit 4		*/
         /*  bit 4 = 0 -> Count down									*/
         /*  bit 4 = 1 -> Count up       								*/
         TIMER4_TAMR_R = ( TIMER4_TAMR_R & 0xFFFFFFEF ) | Config_Ptr->count_m ;
         
         /* Timer A load value  : Register GPTM Timer A Interval Load 32-bit       */
         TIMER4_TAILR_R = Config_Ptr->interval_load ;
       
         /* put prescale value in the Prescale Register	*/
         TIMER4_TBPR_R = Config_Ptr->prescale ;
       
         /* Set Clear-Interrupt bit to clear flag in Raw Interrupt Status register */
         SET_BIT ( TIMER4_ICR_R , TMRB_INT_CLEAR_PIN );
         
         /* Enable interrupt in NVIC   */
         NVIC_EN2_R |= ( Config_Ptr->int_en << 7 );
         
         /* Start Timer B   */
         SET_BIT ( TIMER4_CTL_R , TMRB_EN_PIN );
      
         
         if ( Config_Ptr->int_en == 0 )
         {
           while( GET_BIT( TIMER4_RIS_R , TMRB_FLAG_PIN ) == 0 ){};
           
           
           /* Set Clear-Interrupt bit to clear flag in Raw Interrupt Status register */
           SET_BIT ( TIMER4_ICR_R , TMRB_INT_CLEAR_PIN );
         }
         
         
        break; 
      case TIMER_CONC :
        break; 
      }
      
      break;
      
/************************************TIMER 5***********************************/
   case TIMER5 :
      /* Enable and provide a clock to 16/32-bit general-purpose timer */
      SET_BIT ( SYSCTL_RCGCTIMER_R , TIMER5 );
      
      /* for timer A-------------------------------------------------> */
      /* SET_BIT ( SYSCTL_RCGCGPIO_R  , TIMER0 );  					   */
      
      TIMER5_CFG_R = Config_Ptr->bitMode ;  

      switch(TIM)
      {
      /*********************************TIMER5-A*******************************/
      case TIMER_A :
        
        //disable timer A 
         CLEAR_BIT ( TIMER5_CTL_R , TMRA_EN_PIN );
         
         /* Timer A mode : Register GPTM Timer A Mode -								*/
         /*  to set mode for timer a  , two bits --> bit 0,  bit 1					*/
         /*  Capture  --> set bits 0x3												*/
         /*  Periodic --> set bits 0x2												*/
     	 /*  One-shot --> set bits 0x1												*/
         /*  Reserved --> set bits 0x0												*/
         TIMER5_TAMR_R = ( TIMER5_TAMR_R & 0xFFFFFFFC ) | Config_Ptr->mode ;
         
         /* Timer A Snapshot : Register GPTM Timer A Mode - bit 7		*/
         /*  bit 7 = 0 -> snapshot disabled								*/
         /*  bit 7 = 1 -> snapshot enabled         						*/
         SET_BIT ( TIMER5_TAMR_R , SNAPSHOT_EN_PIN );
         
          /* Timer A trigger START : Register GPTM Timer A Mode - bit 6			*/
          /*  bit 6 = 0 -> start count as the timer is enabled					*/	
          /* bit 6 = 1 -> waits for trigger to start cound       				*/
         CLEAR_BIT ( TIMER5_TAMR_R ,TRIGGER_EN_PIN );
         
         /* Timer A Interrup enable  : Register GPTM Interrupt Mask 			*/
         /*  bit 0 = 0 -> disable interrupt										*/
         /*  bit 0 = 1 -> enable interrupt    									*/
         TIMER5_IMR_R = ( TIMER5_IMR_R & 0xFFFFFFFE ) | Config_Ptr->int_en ;
         
         
         /* Timer A count mode : Register GPTM Timer A Mode - bit 4		*/
         /*  bit 4 = 0 -> Count down									*/
         /*  bit 4 = 1 -> Count up      								*/ 
         TIMER5_TAMR_R = ( TIMER5_TAMR_R & 0xFFFFFFEF ) | Config_Ptr->count_m ;
         
         /* Timer A load value  : Register GPTM Timer A Interval Load 32-bit       	*/
         TIMER5_TAILR_R = Config_Ptr->interval_load ;
         
         /* put prescale value in the Prescale Register  */
         TIMER5_TAPR_R = Config_Ptr->prescale ;
        
         
         /* Set Clear-Interrupt bit to clear flag in Raw Interrupt Status register */
         SET_BIT ( TIMER5_ICR_R , TMRA_INT_CLEAR_PIN );
         
         /* Enable interrupt in NVIC   */
         NVIC_EN2_R |= ( Config_Ptr->int_en << 28 );
         
         /* Start Timer A  */
         SET_BIT ( TIMER5_CTL_R , TMRA_EN_PIN );
        
         
         /* Check if the interrupt is enabled to skip polling */
         if ( Config_Ptr->int_en == 0 )
         {
           /* Polling over Time out Flag in Raw interrupt Status register */
           while( GET_BIT( TIMER5_RIS_R , TMRA_FLAG_PIN ) == 0 ){};
           
           /* Set Clear-Interrupt bit to clear flag in Raw Interrupt Status register	*/
          SET_BIT ( TIMER5_ICR_R , TMRA_INT_CLEAR_PIN );
         }
         
        break; 
        
      /*********************************TIMER5-B*******************************/
      case TIMER_B :
        
        /* disable timer B  */
        CLEAR_BIT ( TIMER5_CTL_R , TMRB_EN_PIN );
        
        /* Timer B mode : Register GPTM Timer B Mode - */
        /*  to set mode for timer B  , two bits --> bit 0,  bit 1		*/
        /*  Capture  --> set bits 0x3									*/
        /*  Periodic --> set bits 0x2									*/
        /*  One-shot --> set bits 0x1									*/
        /*  Reserved --> set bits 0x0									*/
         TIMER5_TBMR_R = ( TIMER5_TBMR_R & 0xFFFFFFFC ) | Config_Ptr->mode ;
         
         /* Timer B Snapshot : Register GPTM Timer B Mode - bit 7		*/
         /*  bit 7 = 0 -> snapshot disabled								*/
         /*  bit 7 = 1 -> snapshot enabled         						*/
         SET_BIT ( TIMER5_TBMR_R , SNAPSHOT_EN_PIN );
         
         /* Timer B trigger START : Register GPTM Timer B Mode - bit 6		*/
         /*  bit 6 = 0 -> start count as the timer is enabled				*/
         /*  bit 6 = 1 -> waits for trigger to start cound       			*/
         CLEAR_BIT ( TIMER5_TBMR_R ,TRIGGER_EN_PIN );
         
         /* Timer A Interrup enable  : Register GPTM Interrupt Mask 	*/
         /*  bit 0 = 0 -> disable interrupt								*/
         /*  bit 0 = 1 -> enable interrupt    							*/
         TIMER5_IMR_R = ( TIMER5_IMR_R & 0xFFFFFFFE ) | ( Config_Ptr->int_en << 8 );
        
         
         /* Timer B count mode : Register GPTM Timer B Mode - bit 4			*/
         /*  bit 4 = 0 -> Count down										*/
         /*  bit 4 = 1 -> Count up       									*/
         TIMER5_TAMR_R = ( TIMER5_TAMR_R & 0xFFFFFFEF ) | Config_Ptr->count_m ;
         
         /* Timer A load value  : Register GPTM Timer A Interval Load 32-bit       */
         TIMER5_TAILR_R = Config_Ptr->interval_load ;
       
         /* put prescale value in the Prescale Register				*/
         TIMER5_TBPR_R = Config_Ptr->prescale ;
         
         /* Set Clear-Interrupt bit to clear flag in Raw Interrupt Status register */
         SET_BIT ( TIMER5_ICR_R , TMRB_INT_CLEAR_PIN );
         
         /* Enable interrupt in NVIC   */
         NVIC_EN2_R |= ( Config_Ptr->int_en << 29 );
         
         /* Start Timer B  */
         SET_BIT ( TIMER5_CTL_R , TMRB_EN_PIN );
         
         if ( Config_Ptr->int_en == 0 )
         {
           while( GET_BIT( TIMER5_RIS_R , TMRB_FLAG_PIN ) == 0 ){};
           
           /* Set Clear-Interrupt bit to clear flag in Raw Interrupt Status register */
           SET_BIT ( TIMER5_ICR_R , TMRB_INT_CLEAR_PIN );
         
         }
         
        break; 
      case TIMER_CONC :
        break; 
      }
      break; 
     
      
  }
 
  
 
}


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
void Timer_deInit ( TIMER_ID TIMER ) 
{
  switch( TIMER )
  {
  case TIMER0:
  	/* Disable Clock    */
    CLEAR_BIT ( SYSCTL_RCGCTIMER_R , TIMER0 );
    break;
  case TIMER1:
  	/* Disable Clock    */
    CLEAR_BIT ( SYSCTL_RCGCTIMER_R , TIMER1 );
    break;
  case TIMER2:
  	/* Disable Clock    */
    CLEAR_BIT ( SYSCTL_RCGCTIMER_R , TIMER2 );
    break;
  case TIMER3:
  	/* Disable Clock    */
    CLEAR_BIT ( SYSCTL_RCGCTIMER_R , TIMER3 );
    break;
  case TIMER4:
  	/* Disable Clock    */
    CLEAR_BIT ( SYSCTL_RCGCTIMER_R , TIMER4 );
    break;
  case TIMER5:
  	/* Disable Clock    */
    CLEAR_BIT ( SYSCTL_RCGCTIMER_R , TIMER5 );
    break;

  }
}




