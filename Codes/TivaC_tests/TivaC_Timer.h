#ifndef _TIVAC_UART_H_
#define _TIVAC_UART_H_

#include "basic_includes.h"

#ifdef DEFINE_VARIABLES
#define EXTERN                  
#define INITIALIZER(...)        = __VA_ARGS__
#else
#define EXTERN                  extern
#define INITIALIZER(...)       
#endif 

EXTERN uint32_t g_ui32Timer1AStatus INITIALIZER(0x00);

void initTimer1A(uint32_t ui32SetValue);
void Timer1AIntHandler();
void Timer1BIntHandler();

#endif

