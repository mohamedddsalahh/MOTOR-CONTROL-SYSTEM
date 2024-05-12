/****************************************************************************************************/
/* Module Name : General-Purpose Input/Outputs  ( source file )                                     */
/*																									*/
/* Author      : Team 1                                                                             */
/*																									*/
/* Purpose     : This source file provides the implementation for configuring and controlling 	    */
/*	   			 GPIO pins on a microcontroller. It includes functions for initializing 			*/
/*				 GPIO pins, setting pin modes (input/output), reading and writing pin values,		*/
/*				 and enabling/disabling internal pull-up or pull-down resistors. Users can 			*/
/*				 include this file in their projects to interact with GPIO pins based on their 		*/
/*				 specific requirements.																*/
/*																									*/
/****************************************************************************************************/

#include "gpio.h"



/*********************************************Functions***********************************************/

/*****************************************************************************************************/
/* Function Name :  GPIO_initDigital																 */
/* Inputs        :  PORT_ID ( PORT  )                                                                */
/* Outputs       :  void ( No outputs )																 */
/* Reentrancy    :  Reentrant																		 */
/* Synchronous   :  Synchronous									    								 */
/* Description   :	This function is used to initialize the port. it takes the requried port and 	 */
/*					open its clock and unlock the port. it also open the digital i/o for the port.   */
/*					and open the modification on the direction , pullup and the alternative function */
/*					options					 														 */
/*****************************************************************************************************/
void GPIO_initDigital ( PORT_ID PORT )
{
  /* Port Enable : General-Purpose Input/Output Run Mode Clock Gating Control */
  /* bit = 0 --> Disable port   											  */
  /* bit = 1 --> Enable  port												  */
  SET_BIT ( SYSCTL_RCGCGPIO_R , PORT );
  
  /* Port Enable : General-Purpose Input/Output Run Mode Clock Gating Control */
  /* Check port is enabled       											  */ 
  while ( ( GET_BIT ( SYSCTL_RCGCGPIO_R , PORT ) ) == 0 ) {} ; 
  
  /*    Switch case to unlock the required port     */
  switch( PORT )
  {
/**********************************PORTA*****************************************/
  case PORTA :
    /* Unlock Port : GPIO Lock Register */
    /* to unlock write  = 0x4C4F434B    */
    GPIO_PORTA_LOCK_R = UNLOCK     ;
    
    /* Allow modify in registers : GPIO Commit (GPIOCR) Register */
    /* bit 0:7 													 */
    /* Allow_Modify = 0xFF                                       */ 
    GPIO_PORTA_CR_R   = ALLOW_MODIFY ;
    
    /* Digital Input/output  :	GPIO Digital Enable				*/
    /* bit 0:7 --> bit for each pin 							*/
    GPIO_PORTA_DEN_R  = PORT_DIGITAL ;
    
    break ;
    
/**********************************PORTB*****************************************/
  case PORTB :
    /* Unlock Port : GPIO Lock Register */
    /* to unlock write  = 0x4C4F434B    */
    GPIO_PORTB_LOCK_R = UNLOCK       ;
    
    /* Allow modify in registers : GPIO Commit (GPIOCR) Register */
    /* bit 0:7 													 */
    /* Allow_Modify = 0xFF                                       */ 
    GPIO_PORTB_CR_R   = ALLOW_MODIFY ;    
  
    /* Digital Input/output  :	GPIO Digital Enable				*/
    /* bit 0:7 --> bit for each pin 							*/
    GPIO_PORTB_DEN_R  = PORT_DIGITAL ;
    
    break ; 
/**********************************PORTC*****************************************/   
  case PORTC :
    /* Unlock Port : GPIO Lock Register */
    /* to unlock write  = 0x4C4F434B    */
    GPIO_PORTC_LOCK_R = UNLOCK       ;
    
    /* Allow modify in registers : GPIO Commit (GPIOCR) Register */
    /* bit 0:7 													 */
    /* Allow_Modify = 0xFF                                       */ 
    GPIO_PORTC_CR_R   = ALLOW_MODIFY ;  
    
    /* Digital Input/output  :	GPIO Digital Enable				*/
    /* bit 0:7 --> bit for each pin 							*/
    GPIO_PORTC_DEN_R  = PORT_DIGITAL ;
    break ;
/**********************************PORTD*****************************************/    
  case PORTD :
    /* Unlock Port : GPIO Lock Register */
    /* to unlock write  = 0x4C4F434B    */
    GPIO_PORTD_LOCK_R = UNLOCK       ;
    
    /* Allow modify in registers : GPIO Commit (GPIOCR) Register */
    /* bit 0:7 													 */
    /* Allow_Modify = 0xFF                                       */ 
    GPIO_PORTD_CR_R   = ALLOW_MODIFY ; 
    
    /* Digital Input/output  :	GPIO Digital Enable				*/
    /* bit 0:7 --> bit for each pin 							*/
    GPIO_PORTD_DEN_R  = PORT_DIGITAL ;
    break ;
/**********************************PORTE*****************************************/    
  case PORTE :
    /* Unlock Port : GPIO Lock Register */
    /* to unlock write  = 0x4C4F434B    */
    GPIO_PORTE_LOCK_R = UNLOCK       ;
    
    /* Allow modify in registers : GPIO Commit (GPIOCR) Register */
    /* bit 0:7 													 */
    /* Allow_Modify = 0xFF                                       */ 
    GPIO_PORTE_CR_R   = ALLOW_MODIFY ;  
  
    /* Digital Input/output  :	GPIO Digital Enable				*/
    /* bit 0:7 --> bit for each pin 							*/
    GPIO_PORTE_DEN_R  = PORT_DIGITAL ;
    break ;
/**********************************PORTF*****************************************/  
  case PORTF :
    /* Unlock Port : GPIO Lock Register */
    /* to unlock write  = 0x4C4F434B    */
    GPIO_PORTF_LOCK_R = UNLOCK       ;
    
    /* Allow modify in registers : GPIO Commit (GPIOCR) Register */
    /* bit 0:7 													 */
    /* Allow_Modify = 0xFF                                       */ 
    GPIO_PORTF_CR_R   = ALLOW_MODIFY ; 
    
    /* Digital Input/output  :	GPIO Digital Enable				*/
    /* bit 0:7 --> bit for each pin 							*/
    GPIO_PORTF_DEN_R  = PORT_DIGITAL ;
    break ;
    
  }
 
}



/*****************************************************************************************************/
/* Function Name :  GPIO_setupPortDirection															 */
/* Inputs        :  PORT_ID ( PORT  )  , PORT_MODE (dir ) , PORT_PULL_UP ( useInternalPullUp )       */
/* Outputs       :  void ( No outputs )																 */
/* Reentrancy    :  Reentrant																		 */
/* Synchronous   :  Synchronous																     	 */
/* Description   :	This function is used to define the port direction and wether to use the internal*/ 
/*					pull up resistance or not				 										 */
/*****************************************************************************************************/
void GPIO_setupPortDirection ( PORT_ID PORT  , PORT_MODE dir , PORT_PULL_UP useInternalPullUp  )
{
  switch ( PORT )
  {
/**********************************PORTA*****************************************/
  case PORTA :
    /* define port output :  GPIO Direction  */ 
    /* dir --> OUTPUT_DIR					 */
    /* dir --> INPUT_DIR					 */
    GPIO_PORTA_DIR_R  = dir ;
    
    /*Pull up : GPIO Pull-Up Select Register */
    /* useInternalPullUp --> USE_PUR 		 */
    /* useInternalPullUp --> NO_USE			 */ 
    GPIO_PORTA_PUR_R  = useInternalPullUp ; 
    break ;
/**********************************PORTB*****************************************/
  case PORTB :
    /* define port output :  GPIO Direction  */ 
    /* dir --> OUTPUT_DIR					 */
    /* dir --> INPUT_DIR					 */
    GPIO_PORTB_DIR_R  = dir ;
    
    /*Pull up : GPIO Pull-Up Select Register */
    /* useInternalPullUp --> USE_PUR 		 */
    /* useInternalPullUp --> NO_USE			 */ 
    GPIO_PORTB_PUR_R  = useInternalPullUp ;
    break ; 
/**********************************PORTC*****************************************/
  case PORTC :
	/* define port output :  GPIO Direction  */ 
    /* dir --> OUTPUT_DIR					 */
    /* dir --> INPUT_DIR					 */
    GPIO_PORTC_DIR_R  = dir ;

    /*Pull up : GPIO Pull-Up Select Register */
    /* useInternalPullUp --> USE_PUR 		 */
    /* useInternalPullUp --> NO_USE			 */
    GPIO_PORTC_PUR_R  = useInternalPullUp ;
    break ;
/**********************************PORTD*****************************************/
  case PORTD :
	/* define port output :  GPIO Direction  */ 
    /* dir --> OUTPUT_DIR					 */
    /* dir --> INPUT_DIR					 */
    GPIO_PORTD_DIR_R  = dir ;
    
    /*Pull up : GPIO Pull-Up Select Register */
    /* useInternalPullUp --> USE_PUR 		 */
    /* useInternalPullUp --> NO_USE			 */
    GPIO_PORTD_PUR_R  = useInternalPullUp ;
    break ;
/**********************************PORTE*****************************************/
  case PORTE :
	/* define port output :  GPIO Direction  */ 
    /* dir --> OUTPUT_DIR					 */
    /* dir --> INPUT_DIR					 */
    GPIO_PORTE_DIR_R  = dir ;
    
    /*Pull up : GPIO Pull-Up Select Register */
    /* useInternalPullUp --> USE_PUR 		 */
    /* useInternalPullUp --> NO_USE			 */
    GPIO_PORTE_PUR_R  = useInternalPullUp ;
    break ;
/**********************************PORTF*****************************************/
  case PORTF :
	/* define port output :  GPIO Direction  */ 
    /* dir --> OUTPUT_DIR					 */
    /* dir --> INPUT_DIR					 */
    GPIO_PORTF_DIR_R  = dir ;
    
    /*Pull up : GPIO Pull-Up Select Register */
    /* useInternalPullUp --> USE_PUR 		 */
    /* useInternalPullUp --> NO_USE			 */
    GPIO_PORTF_PUR_R  = useInternalPullUp ; 
    break ;
  }
}



/****************************************************************************************************************/
/* Function Name :  GPIO_setupPinDirection															 			*/
/* Inputs        :  PORT_ID ( PORT  )  , PIN_ID PIN,PORT_MODE (dir ) , PORT_PULL_UP ( useInternalPullUp )       */
/* Outputs       :  void ( No outputs )																            */
/* Reentrancy    :  Reentrant																		            */
/* Synchronous   :  Synchronous																     	            */
/* Description   :	This function is used to define the PIN direction and wether to use the internal pull up    */
/*					resistance or not			 										                        */
/****************************************************************************************************************/
void GPIO_setupPinDirection ( PORT_ID PORT  ,  PIN_ID PIN , PIN_MODE dir , PIN_PULL_UP useInternalPullUp  )
{
  switch ( PORT )
  {
/**********************************PORTA*****************************************/
  case PORTA :
    /* define port output :  GPIO Direction  */ 
    /* dir --> OUTPUT_DIR					 */
    /* dir --> INPUT_DIR					 */
    if (dir == OUTPUT_PIN_DIR )
      SET_BIT ( GPIO_PORTA_DIR_R , PIN ) ;
    
    else if ( dir == INPUT_PIN_DIR )
      CLEAR_BIT (GPIO_PORTA_DIR_R , PIN ) ;
    
    
    /*Pull up : GPIO Pull-Up Select Register */
    /* useInternalPullUp --> USE_PUR 		 */
    /* useInternalPullUp --> NO_USE			 */
    if (useInternalPullUp == USE_PULL )
      SET_BIT ( GPIO_PORTA_PUR_R , PIN ) ;
    
    else if ( useInternalPullUp == NO_USE )
      CLEAR_BIT (GPIO_PORTA_PUR_R , PIN ) ;
    break ;
/**********************************PORTB*****************************************/
  case PORTB :
    /* define port output :  GPIO Direction  */ 
    /* dir --> OUTPUT_DIR					 */
    /* dir --> INPUT_DIR					 */
    if (dir == OUTPUT_PIN_DIR )
      SET_BIT ( GPIO_PORTB_DIR_R , PIN ) ;
    
    else if ( dir == INPUT_PIN_DIR )
      CLEAR_BIT (GPIO_PORTB_DIR_R , PIN ) ;
    
    
    /*Pull up : GPIO Pull-Up Select Register */
    /* useInternalPullUp --> USE_PUR 		 */
    /* useInternalPullUp --> NO_USE			 */
    if (useInternalPullUp == USE_PULL )
      SET_BIT ( GPIO_PORTB_PUR_R , PIN ) ;
    
    else if ( useInternalPullUp == NO_USE )
      CLEAR_BIT (GPIO_PORTB_PUR_R , PIN ) ;
    break ; 
/**********************************PORTC*****************************************/
  case PORTC :
    /* define port output :  GPIO Direction  */ 
    /* dir --> OUTPUT_DIR					 */
    /* dir --> INPUT_DIR					 */
    if (dir == OUTPUT_PIN_DIR )
      SET_BIT ( GPIO_PORTC_DIR_R , PIN ) ;
    
    else if ( dir == INPUT_PIN_DIR )
      CLEAR_BIT (GPIO_PORTC_DIR_R , PIN ) ;
    
    
    /*Pull up : GPIO Pull-Up Select Register */
    /* useInternalPullUp --> USE_PUR 		 */
    /* useInternalPullUp --> NO_USE			 */
    if (useInternalPullUp == USE_PULL )
      SET_BIT ( GPIO_PORTC_PUR_R , PIN ) ;
    
    else if ( useInternalPullUp == NO_USE )
      CLEAR_BIT (GPIO_PORTC_PUR_R , PIN ) ;
    break ;
/**********************************PORTD*****************************************/
  case PORTD :
    /* define port output :  GPIO Direction  */ 
    /* dir --> OUTPUT_DIR					 */
    /* dir --> INPUT_DIR					 */
    if (dir == OUTPUT_PIN_DIR )
      SET_BIT ( GPIO_PORTD_DIR_R , PIN ) ;
    
    else if( dir == INPUT_PIN_DIR )
      CLEAR_BIT (GPIO_PORTD_DIR_R , PIN ) ;
    
    
    /*Pull up : GPIO Pull-Up Select Register */
    /* useInternalPullUp --> USE_PUR 		 */
    /* useInternalPullUp --> NO_USE			 */
    if (useInternalPullUp == USE_PULL )
      SET_BIT ( GPIO_PORTD_PUR_R , PIN ) ;
    
    else if ( useInternalPullUp == NO_USE )
      CLEAR_BIT (GPIO_PORTD_PUR_R , PIN ) ;
    break ;

/**********************************PORTE*****************************************/
  case PORTE :
    /* define port output :  GPIO Direction  */ 
    /* dir --> OUTPUT_DIR					 */
    /* dir --> INPUT_DIR					 */ 
    if (dir == OUTPUT_PIN_DIR )
      SET_BIT( GPIO_PORTE_DIR_R , PIN ) ;
    
    else if ( dir == INPUT_PIN_DIR )
      CLEAR_BIT (GPIO_PORTE_DIR_R , PIN ) ;
    
    
    /*Pull up : GPIO Pull-Up Select Register */
    /* useInternalPullUp --> USE_PUR 		 */
    /* useInternalPullUp --> NO_USE			 */
    if (useInternalPullUp == USE_PULL )
      SET_BIT ( GPIO_PORTE_PUR_R , PIN ) ;
    
    else if ( useInternalPullUp == NO_USE  )
      CLEAR_BIT (GPIO_PORTE_PUR_R , PIN ) ;
    break ;
/**********************************PORTF*****************************************/    
  case PORTF :
    /* define port output :  GPIO Direction  */ 
    /* dir --> OUTPUT_DIR					 */
    /* dir --> INPUT_DIR					 */  
    if (dir == OUTPUT_PIN_DIR )
      SET_BIT ( GPIO_PORTF_DIR_R , PIN ) ;
    
    else if ( dir == INPUT_PIN_DIR )
      CLEAR_BIT (GPIO_PORTF_DIR_R , PIN ) ;
    
    
    /*Pull up : GPIO Pull-Up Select Register */
    /* useInternalPullUp --> USE_PUR 		 */
    /* useInternalPullUp --> NO_USE			 */
    if (useInternalPullUp == USE_PULL )
      SET_BIT ( GPIO_PORTF_PUR_R , PIN ) ;
    
    else if ( useInternalPullUp == NO_USE )
      CLEAR_BIT (GPIO_PORTF_PUR_R , PIN ) ;
    break ;
  }
  
  
}



/****************************************************************************************************************/
/* Function Name :  GPIO_writeBit													*/
/* Inputs        :  PORT_ID ( PORT  )  , PIN_ID PIN, uint8 (val	)										        */
/* Outputs       :  void ( No outputs )																            */
/* Reentrancy    :  Reentrant																		            */
/* Synchronous   :  Synchronous																     	            */
/* Description   :	This function is used to set a high or low for a specific bit in specific port				*/			                      
/****************************************************************************************************************/
void GPIO_writeBit ( PORT_ID PORT  ,  PIN_ID PIN , uint8 val )
{
  switch ( PORT ) 
  {
/**********************************PORTA*****************************************/
  case PORTA :
    /* GPIO data : GPIO Data register   */
    /* bit	0:7							*/
    /*									*/
    if ( val == LOGIC_HIGH )
      SET_BIT ( GPIO_PORTA_DATA_R , PIN );
    else if ( val == LOGIC_LOW )
      CLEAR_BIT ( GPIO_PORTA_DATA_R , PIN );
    break; 
/**********************************PORTB*****************************************/
  case PORTB :
    /* GPIO data : GPIO Data register   */
    /* bit	0:7							*/
    /*									*/
    if ( val == LOGIC_HIGH )
      SET_BIT ( GPIO_PORTB_DATA_R , PIN );
    else if ( val == LOGIC_LOW )
      CLEAR_BIT ( GPIO_PORTB_DATA_R , PIN );
    break; 
/**********************************PORTC*****************************************/
  case PORTC :
    /* GPIO data : GPIO Data register   */
    /* bit	0:7							*/
    /*									*/
    if ( val == LOGIC_HIGH )
      SET_BIT ( GPIO_PORTC_DATA_R , PIN );
    else if ( val == LOGIC_LOW )
      CLEAR_BIT ( GPIO_PORTC_DATA_R , PIN );
    break; 
/**********************************PORTD*****************************************/ 
  case PORTD :
    /* GPIO data : GPIO Data register   */
    /* bit	0:7							*/
    /*									*/
    if ( val == LOGIC_HIGH )
      SET_BIT ( GPIO_PORTD_DATA_R , PIN );
    else if ( val == LOGIC_LOW )
      CLEAR_BIT ( GPIO_PORTD_DATA_R , PIN );
    break; 
/**********************************PORTE*****************************************/ 
  case PORTE :
    /* GPIO data : GPIO Data register   */
    /* bit	0:7							*/
    /*									*/
    if ( val == LOGIC_HIGH )
      SET_BIT ( GPIO_PORTE_DATA_R , PIN );
    else if ( val == LOGIC_LOW )
      CLEAR_BIT ( GPIO_PORTE_DATA_R , PIN );
    break; 
/**********************************PORTF*****************************************/
  case PORTF :
    /* GPIO data : GPIO Data register   */
    /* bit	0:7							*/
    /*									*/
    if ( val == LOGIC_HIGH )
      SET_BIT ( GPIO_PORTF_DATA_R , PIN );
    else if ( val == LOGIC_LOW )
      CLEAR_BIT ( GPIO_PORTF_DATA_R , PIN );
    break; 
      
  }
}




/****************************************************************************************************************/
/* Function Name :  GPIO_readBit															 	        		*/
/* Inputs        :  PORT_ID ( PORT  )  , PIN_ID PIN 													        */
/* Outputs       :  uint8           																            */
/* Reentrancy    :  Reentrant																		            */
/* Synchronous   :  Synchronous																     	            */
/* Description   :	This function is used to red  a specific bit in specific port								*/
/****************************************************************************************************************/
uint8 GPIO_readBit ( PORT_ID PORT  ,  PIN_ID PIN )
{
  switch ( PORT ) 
  {
/**********************************PORTA*****************************************/
  case PORTA :
    /* GPIO data : GPIO Data register   */
    /* bit	0:7							*/
    /*									*/
    return GET_BIT ( GPIO_PORTA_DATA_R , PIN );
    break; 
/**********************************PORTB*****************************************/
  case PORTB :
    /* GPIO data : GPIO Data register   */
    /* bit	0:7							*/
    /*									*/
    return GET_BIT ( GPIO_PORTB_DATA_R , PIN );
    break; 
/**********************************PORTC*****************************************/   
  case PORTC :
    /* GPIO data : GPIO Data register   */
    /* bit	0:7							*/
    /*									*/
    return GET_BIT ( GPIO_PORTC_DATA_R , PIN );
    break; 
/**********************************PORTD*****************************************/ 
  case PORTD :
    /* GPIO data : GPIO Data register   */
    /* bit	0:7							*/
    /*									*/
    return GET_BIT ( GPIO_PORTD_DATA_R , PIN );
    break; 
/**********************************PORTE*****************************************/  
  case PORTE :
    /* GPIO data : GPIO Data register   */
    /* bit	0:7							*/
    /*									*/
    return GET_BIT ( GPIO_PORTE_DATA_R , PIN );
    break; 
/**********************************PORTF*****************************************/
  case PORTF :
    /* GPIO data : GPIO Data register   */
    /* bit	0:7							*/
    /*									*/
    return GET_BIT ( GPIO_PORTF_DATA_R , PIN );
    break; 
      
  }
  return 0 ;
}



/****************************************************************************************************************/
/* Function Name :  GPIO_writePort															 	        		*/
/* Inputs        :  PORT_ID ( PORT  )  , uint8 val  													        */
/* Outputs       :  void	           																            */
/* Reentrancy    :  Reentrant																		            */
/* Synchronous   :  Synchronous																     	            */
/* Description   :	This function is used write  8 bit value for the entire port								*/
/****************************************************************************************************************/
void GPIO_writePORT( PORT_ID PORT , uint8 val )
{
    switch ( PORT ) 
  {
/**********************************PORTA*****************************************/
  case PORTA :
    /* GPIO data : GPIO Data register   */
    /* bit	0:7							*/
    /*									*/
    GPIO_PORTA_DATA_R= val ;
    break; 
/**********************************PORTB*****************************************/
  case PORTB :
    /* GPIO data : GPIO Data register   */
    /* bit	0:7							*/
    /*									*/
    GPIO_PORTB_DATA_R= val ;
    break; 
/**********************************PORTC*****************************************/
  case PORTC :
    /* GPIO data : GPIO Data register   */
    /* bit	0:7							*/
    /*									*/
    GPIO_PORTC_DATA_R = val ;
    break; 
/**********************************PORTD*****************************************/
  case PORTD :
    /* GPIO data : GPIO Data register   */
    /* bit	0:7							*/
    /*									*/
    GPIO_PORTD_DATA_R= val ;
    break; 
/**********************************PORTE*****************************************/
  case PORTE :
    /* GPIO data : GPIO Data register   */
    /* bit	0:7							*/
    /*									*/
    GPIO_PORTE_DATA_R= val ;
    break; 
/**********************************PORTF*****************************************/
  case PORTF :
    /* GPIO data : GPIO Data register   */
    /* bit	0:7							*/
    /*									*/
    GPIO_PORTF_DATA_R= val ;
    break;
  }
}

/****************************************************************************************************************/
/* Function Name :  GPIO_readPort															 	        		*/
/* Inputs        :  PORT_ID ( PORT  )  , uint8 val  													        */
/* Outputs       :  uint8	           																            */
/* Reentrancy    :  Reentrant																		            */
/* Synchronous   :  Synchronous																     	            */
/* Description   :	This function is used read  8 bit value for the entire port     							*/
/****************************************************************************************************************/
uint8 GPIO_readPORT( PORT_ID PORT , uint8 val )
{
    switch ( PORT ) 
  {
/**********************************PORTA*****************************************/
  case PORTA :
    /* GPIO data : GPIO Data register   */
    /* bit	0:7							*/
    /*									*/
    return GPIO_PORTA_DATA_R ;
    break; 
/**********************************PORTB*****************************************/
  case PORTB :
    /* GPIO data : GPIO Data register   */
    /* bit	0:7							*/
    /*									*/
    return GPIO_PORTB_DATA_R ;
    break; 
/**********************************PORTC*****************************************/
  case PORTC :
    /* GPIO data : GPIO Data register   */
    /* bit	0:7							*/
    /*									*/
    return GPIO_PORTC_DATA_R ;
    break; 
/**********************************PORTD*****************************************/
  case PORTD :
    /* GPIO data : GPIO Data register   */
    /* bit	0:7							*/
    /*									*/
    return GPIO_PORTD_DATA_R ;
    break; 
/**********************************PORTE*****************************************/
  case PORTE :
    /* GPIO data : GPIO Data register   */
    /* bit	0:7							*/
    /*									*/
    return GPIO_PORTE_DATA_R ;
    break; 
/**********************************PORTF*****************************************/
  case PORTF :
    /* GPIO data : GPIO Data register   */
    /* bit	0:7							*/
    /*									*/
    return GPIO_PORTF_DATA_R ;
    break;
  }
  return 0 ; 
}



