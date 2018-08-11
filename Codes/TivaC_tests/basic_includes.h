/*
//Include all the board related headres in on efile only
*/

#ifndef _BASIC_INCLUDES_H_
#define _BASIC_INCLUDES_H_
// define target
#define TARGET_IS_TM4C123_RA1
// basic data types
#include <string.h>
#include <stdint.h>   // Standard integer Library
#include <stdbool.h>  // Boolean Library
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

#endif

