#ifndef _TIVAC_SPI_H_
#define _TIVAC_SPI_H_

# include "basic_includes.h"

#ifdef DEFINE_VARIABLES
#define EXTERN                  
#define INITIALIZER(...)        = __VA_ARGS__
#else
#define EXTERN                  extern
#define INITIALIZER(...)       
#endif 

extern void initSPI(uint32_t ui32Protocol, uint32_t ui32Mode, 
                    uint32_t ui32BitRate, uint32_t ui32DataWidth);
extern uint32_t SPIexchangeData(uint32_t ui32TxData = 0x0000);
extern uint32_t SPIGetData(void);
extern void SPIsendData(uint32_t ui32TxData);

#endif

