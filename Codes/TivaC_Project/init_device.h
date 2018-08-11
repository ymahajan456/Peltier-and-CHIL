// Init all perpherals except USB

#ifndef _INIT_DEVICES_H_
#define _INIT_DEVICES_H_

# include "basic_includes.h"

#ifdef DEFINE_VARIABLES
#define EXTERN                  
#define INITIALIZER(...)        = __VA_ARGS__
#else
#define EXTERN                  extern
#define INITIALIZER(...)       
#endif

#define INA_avg_index 6
#define INA_conv_index 0

extern void setPWMLEDColor(uint32_t *color);
extern void initPWM_LEDs(uint32_t ui32Period);
extern void initPWM_Peltier(uint32_t ui32Period);
extern void initTimer(uint32_t ui32SetValue);
extern void initSPI(uint32_t ui32Protocol, uint32_t ui32Mode, 
						uint32_t ui32BitRate, uint32_t ui32DataWidth);
extern void initI2C(bool bFast);

// INA Averaging Modes
//float conversionTimes[8] = {0.14, 0.204, 0.332, 0.588, 1.1, 2.116, 4.156, 8.224};
//int averages[8] = {1, 4, 16, 64, 128, 256, 512, 1024};

#endif
