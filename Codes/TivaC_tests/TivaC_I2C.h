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

#define INA_ADDRESS 0x40

extern void initI2CMaster(bool bfast);
extern void I2CsendData(uint8_t ui8TxData);
extern void I2CreceiveData(uint8_t *ui8RxData);

#endif


