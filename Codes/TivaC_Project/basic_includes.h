/*
//Include all the board related headres in on efile only
*/

#ifndef _BASIC_INCLUDES_H_
#define _BASIC_INCLUDES_H_
// define target
#define TARGET_IS_TM4C123_RA1

#ifdef DEFINE_VARIABLES
#define EXTERN
#define INITIALIZER(...)        = __VA_ARGS__
#else
#define EXTERN                  extern
#define INITIALIZER(...)
#endif

// basic data types
#include <string.h>
#include <stdint.h>   // Standard integer Library
#include <stdbool.h>  // Boolean Library
#include <math.h>
// Pin mapping and basic functions
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"  // Base memory address for all pins
#include "inc/hw_types.h"
#include "inc/hw_uart.h"
#include "inc/hw_i2c.h"
#include "inc/hw_gpio.h"   //  unlock the pins that are locked to specific functions on the microcontroller 
// USB 
#include "driverlib/usb.h"
#include "usblib/usblib.h"
#include "usblib/usb-ids.h"
#include "usblib/device/usbdevice.h"
#include "usblib/device/usbdbulk.h"
#include "usb_bulk_structs.h"
// UART
#include "utils/uartstdio.h"
#include "utils/ustdlib.h"
#include "driverlib/uart.h"

// device specific
#include "driverlib/gpio.h"   // device libraries
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/timer.h"
#include "driverlib/fpu.h"
#include "driverlib/rom_map.h"
#include "driverlib/uart.h"
#include "driverlib/i2c.h"
#include "driverlib/ssi.h"
#include "driverlib/interrupt.h"
#include "driverlib/adc.h"

//#include "driverlib/debug.h"   // uncomment to debug code

// defined project interrupt handlers
//#includes "int_handlers.c"

// defined to work with usb
#define SYSTICKS_PER_SECOND     100
#define SYSTICK_PERIOD_MS       (1000 / SYSTICKS_PER_SECOND)

#define COMMAND_PACKET_RECEIVED 0x00000001
#define COMMAND_STATUS_UPDATE   0x00000002

#define INA_ADDRESS 0x40

#define ADC_MASK_LM35p	0x1000	// channel2
#define ADC_MASK_LM35m	0x1800	// channel3
#define ADC_MASK_IVp	0x0000	//channel0
#define ADC_MASK_IVm	0x0100	//channel1
#define ADC_MASK_RTemp 	0x2000 // channel 4

#define Relay_Port_Base		GPIO_PORTC_BASE
#define IV_Relay_Pin		GPIO_PIN_4
#define Peltier_Relay_Pin	GPIO_PIN_5

#define Peltier_PWM_Pin	GPIO_PIN_0
#define PWM_BASE			GPIO_PORTF_BASE

#define SPI_BASE		GPIO_PORTA_BASE
#define ADC_CS			GPIO_PIN_3
#define DAC_CS			GPIO_PIN_6
#define DigiPot_CS		GPIO_PIN_7

#define DAC_MASK	0x3000

// Temp loop data array
// Temperature 2 bytes + Current 2 bytes + PWM 2 bytes
EXTERN uint8_t g_ui8tempLoopData[10] ;
#define TempByte0 2
#define TempByte1 3
#define PIByte0 4
#define PIByte1 5
#define PWMByte0 6
#define PWMByte1 7

#define COMMAND 0x21
#define DATA 0x22

// command packet headers
#define START_IV 0x81
#define SET_DEVICE 0x66
#define START_DEVICES 0x99
#define STOP_DEVICES 0x55
#define COMMAND_ACC 0xFF

// IV loop data
// Devicep 2 bytes + Devicem 2 bytes
EXTERN uint8_t g_ui8IVData[9];

EXTERN uint32_t RED[3] INITIALIZER({0,1,1});
EXTERN uint32_t BLUE[3] INITIALIZER({1,0,1});
EXTERN uint32_t GREEN[3] INITIALIZER({1,1,0});
EXTERN uint32_t WHITE[3] INITIALIZER({0,0,0});
EXTERN uint32_t BLACK[3] INITIALIZER({1,1,1});

EXTERN int32_t g_i32SetTemp INITIALIZER(328);				// 12 bits
EXTERN uint32_t g_ui32PeltPWMduty;			// 12 bits
EXTERN int32_t g_i32PeltCurrent;			// 16 bits
EXTERN int32_t g_i32PeltMaxCurrent;		// 16 bits
EXTERN int32_t g_i32Temp;					// 12 bits
EXTERN uint32_t g_ui32rTemp INITIALIZER(246);				// 12 bits
EXTERN uint32_t g_ui32PeltMaxPWMduty;		// 12 bits
EXTERN uint32_t g_ui32tmp0, g_ui32tmp1;
EXTERN uint32_t g_ui32DeviceVmax INITIALIZER(0xFFE);
EXTERN uint32_t g_ui32DeviceVmin INITIALIZER(0xFFE);
EXTERN uint32_t g_ui32DeviceImax INITIALIZER(1802);
EXTERN uint32_t g_ui32DeviceImin INITIALIZER(1802);
EXTERN uint32_t g_ui32DACValue INITIALIZER(0x0000);
EXTERN bool g_bTempStable INITIALIZER(false);
//EXTERN bool g_tempLoopActive INITIALIZER(false);
EXTERN bool g_bNewTempData INITIALIZER(false);
EXTERN bool g_bNewIVData INITIALIZER(false);
EXTERN bool g_bNegativeIV INITIALIZER(true);
EXTERN bool g_bStartIV INITIALIZER(false);
EXTERN bool g_bIVComplete INITIALIZER(false);

EXTERN uint8_t g_ui8msg_acc[4] INITIALIZER({0x21, 0xFF, 0x00, 0x04});

EXTERN int32_t g_i32integral INITIALIZER(0);

#endif

