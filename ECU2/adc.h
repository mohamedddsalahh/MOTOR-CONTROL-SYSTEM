#ifndef _ADC_H_
#define _ADC_H_


#include "tm4c123gh6pm.h"
#include "types.h"
#include "bitwise_operation.h"


#define PORTB   1
#define PORTD   3
#define PORTE   4

#define PIN0    0
#define PIN1    1
#define PIN2    2
#define PIN3    3
#define PIN4    4
#define PIN5    5
#define PIN6    6
#define PIN7    7

#define ADC0_PORT       PORTE
#define ADC1_PORT       PORTE
#define ADC2_PORT       PORTE
#define ADC3_PORT       PORTE
#define ADC4_PORT       PORTD
#define ADC5_PORT       PORTD
#define ADC6_PORT       PORTD
#define ADC7_PORT       PORTD
#define ADC8_PORT       PORTE
#define ADC9_PORT       PORTE
#define ADC10_PORT      PORTB
#define ADC11_PORT      PORTB

#define ADC0_PIN        PIN3
#define ADC1_PIN        PIN2
#define ADC2_PIN        PIN1
#define ADC3_PIN        PIN0
#define ADC4_PIN        PIN3
#define ADC5_PIN        PIN2        
#define ADC6_PIN        PIN1
#define ADC7_PIN        PIN0
#define ADC8_PIN        PIN5
#define ADC9_PIN        PIN4
#define ADC10_PIN       PIN4
#define ADC11_PIN       PIN5




void LM35_Init();
uint32 LM35_Value();



void REF_init();
float32 REF_Value();




#endif 