/*
*/

#include "basic_includes.h"
#ifndef _TIVAC_USB_H_
#define _TIVAC_USB_H_

#ifdef DEFINE_VARIABLES
#define EXTERN                  
#define INITIALIZER(...)        = __VA_ARGS__
#else
#define EXTERN                  extern
#define INITIALIZER(...)       
#endif

EXTERN volatile uint32_t g_ui32TxCount INITIALIZER(0);
EXTERN volatile uint32_t g_ui32RxCount INITIALIZER(0);
EXTERN volatile bool g_bUSBSent INITIALIZER(false);
EXTERN volatile bool g_bUSBConfigured INITIALIZER(false);
//EXTERN volatile uint32_t g_ui32Flags INITIALIZER(0);


//volatile uint32_t g_ui32USBTxSpace = 128;
//volatile uint32_t g_ui32USBRxLen = 0;
/*************************************************************************************************/

extern void initUSB();
extern uint32_t TxHandler(void *pvCBData, uint32_t ui32Event, uint32_t ui32MsgValue, void *pvMsgData);
extern uint32_t RxHandler(void *pvCBData, uint32_t ui32Event, uint32_t ui32MsgValue, void *pvMsgData);
static uint32_t EchoNewDataToHost(tUSBDBulkDevice *psDevice, uint8_t *pui8Data, uint32_t ui32NumBytes);
extern uint32_t send_data_to_host(uint8_t *ui8Data, uint32_t ui32MsgLen);
extern uint32_t get_data_from_host(tUSBDBulkDevice *psDevice, uint8_t *pui8Data, uint32_t ui32NumBytes);

#endif
