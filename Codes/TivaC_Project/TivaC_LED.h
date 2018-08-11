/*

    Functions to set up on-board LEDs on LauncPad board (TM4C123GH6PM)
 
    Description:
    ------------
        Pins:
        1. Red LED : GPIO = PF1; PWM = M1PWM5 Gen 2
        2. Green Led : GPIO = PF2; PWM = M1PWM6 GEN 3
        3. Blue Led : GPIO = PF3; PWM = M1PWM7 GEN 3 
*/

#ifndef _TIVAC_LED_H_
#define _TIVAC_LED_H_

#include "basic_includes.h"

#define PWM_RED PWM_OUT_5
#define PWM_GREEN PWM_OUT_6
#define PWM_BLUE PWM_OUT_7 

#define RED_LED GPIO_PIN_1
#define GREEN_LED GPIO_PIN_2
#define BLUE_LED GPIO_PIN_3

#define ON 0x1;
#define OFF 0x0;

#define PWM_LED_BASE PWM1_BASE
#define GPIO_LED_BASE GPIO_PORTF_BASE

#ifdef DEFINE_VARIABLES
#define EXTERN                  
#define INITIALIZER(...)        = __VA_ARGS__
#else
#define EXTERN                  extern
#define INITIALIZER(...)       
#endif 

// if colors are going to be used multiple times define as const to save data memory
EXTERN uint32_t RED[3] INITIALIZER({0,1,1});
EXTERN uint32_t BLUE[3] INITIALIZER({1,0,1});
EXTERN uint32_t GREEN[3] INITIALIZER({1,1,0});
EXTERN uint32_t WHITE[3] INITIALIZER({0,0,0});
EXTERN uint32_t BLACK[3] INITIALIZER({1,1,1});
 
EXTERN void PWMSetLeds(uint32_t ui32Period);
EXTERN void SetLeds();
EXTERN void SetLedColor(uint32_t *color);

#endif
