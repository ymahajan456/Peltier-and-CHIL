#ifndef _TIVAC_UART_H_
#define _TIVAC_UART_H_

# include "basic_includes.h"

#ifdef DEFINE_VARIABLES
#define EXTERN                  
#define INITIALIZER(...)        = __VA_ARGS__
#else
#define EXTERN                  extern
#define INITIALIZER(...)       
#endif 

extern uint32_t UARTSend(uint32_t ui32UARTBase, const uint8_t *pui8Buffer, uint32_t ui32Count);
extern void UARTIntHandler(void);
extern void initUART(uint32_t baudRate);
#endif

