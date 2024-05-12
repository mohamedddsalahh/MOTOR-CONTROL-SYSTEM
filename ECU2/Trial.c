#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "DIO.h"
#include "systick.h"

#include "tm4c123gh6pm.h"
#include "bitwise_operation.h"
#include "types.h"

typedef enum {
    WHITE,
    RED,
    GREEN,
    BLUE
} State;

State previousState = GREEN;
State currentState = WHITE;
State nextState = RED;
uint32 g_tick=0;


void SysTick_Handler(void)
{
 g_tick++;
}

void readSwitch(void)
{
  if(!(DIO_ReadPin (PORTF, PIN0)) && !(DIO_ReadPin (PORTF, PIN4)))
  {
    currentState=WHITE;
  }
  else if (!(DIO_ReadPin (PORTF, PIN0)))
  {
    previousState=WHITE;
    currentState=RED;
    nextState= BLUE;
  }
  else if (!(DIO_ReadPin (PORTF, PIN4)))
  {
    previousState=WHITE;
    currentState=GREEN;
    nextState= BLUE;
  }
}

void switchStates(void)
{
  switch(currentState) {
    case WHITE:
      break;
}

int main (){
  DIO_Init_pin (PORTF,PIN0,PIN_INPUT);
  DIO_Init_pin (PORTF,PIN4,PIN_INPUT);
  SET_BIT( GPIO_PORTF_PUR_R, PIN0);
  SET_BIT( GPIO_PORTF_PUR_R, PIN4);
  DIO_Init_pin (PORTF,PIN1,PIN_OUTPUT);
  DIO_Init_pin (PORTF,PIN2,PIN_OUTPUT);
  DIO_Init_pin (PORTF,PIN3,PIN_OUTPUT);

  
  
  CLEAR_BIT (NVIC_ST_CTRL_R,2);
  CLEAR_BIT (NVIC_ST_CTRL_R, 1);
  CLEAR_BIT(NVIC_ST_CTRL_R,0 );
  NVIC_ST_RELOAD_R = 0;
  NVIC_ST_CURRENT_R = 0;
  if(GET_BIT(NVIC_ST_CTRL_R,16)){}; 
  
  NVIC_ST_RELOAD_R = 1000 *16000;
 
  SET_BIT(NVIC_ST_CTRL_R,2);
  SET_BIT(NVIC_ST_CTRL_R,0) ; 
  
   SET_BIT(NVIC_ST_CTRL_R, 1); 
   NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x00FFFFFF) | 0x40000000; 
  

  while(1)
  {
     
    

  }
  
}       