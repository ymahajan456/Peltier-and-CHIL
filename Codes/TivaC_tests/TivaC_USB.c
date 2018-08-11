
#include "basic_includes.h"
#include "TivaC_USB.h"
#include "TivaC_LED.h"

volatile uint32_t g_ui32SysTickCount = 0;
volatile uint32_t g_ui32Flags = 0;

void SysTickIntHandler(void)
{
    // Update our system tick counter.
    g_ui32SysTickCount++;
}


/*************************************************************************************************/
uint32_t TxHandler(void *pvCBData, uint32_t ui32Event, uint32_t ui32MsgValue, void *pvMsgData)
{
    // update buffer free up info
    if(ui32Event == USB_EVENT_TX_COMPLETE)
    {
        g_ui32TxCount += ui32MsgValue;
        g_bUSBSent = true;
    }
    #ifdef DEBUG
    DEBUG_PRINT("TX complete %d\n", ui32MsgValue);
    #endif
    return(0);
}
/*************************************************************************************************/
uint32_t RxHandler(void *pvCBData, uint32_t ui32Event, uint32_t ui32MsgValue, void *pvMsgData)
{
    switch(ui32Event)
    {
        //new host has connected
        case USB_EVENT_CONNECTED:
        {
            g_bUSBConfigured = true;
            SetLedColor(GREEN);
            //#ifdef DEBUG
            //UARTprintf("Host connected.\n");
            //#endif
            // Flush our buffers.
            USBBufferFlush(&g_sTxBuffer);
            USBBufferFlush(&g_sRxBuffer);
            break;
        }
        // The host has disconnected.
        case USB_EVENT_DISCONNECTED:
        {
            g_bUSBConfigured = false;
           // #ifdef DEBUG
            //UARTprintf("Host disconnected.\n");
           // #endif
            break;
        }
        // A new packet has been received.
        case USB_EVENT_RX_AVAILABLE:
        {
            tUSBDBulkDevice *psDevice;
            // Get a pointer to our instance data from the callback data parameter
            psDevice = (tUSBDBulkDevice *)pvCBData;
            // Read the new packet and echo it back to the host.
            return(get_data_from_host(psDevice, pvMsgData, ui32MsgValue));
        }
        // Ignore SUSPEND and RESUME for now.
        case USB_EVENT_SUSPEND:
        case USB_EVENT_RESUME:
        {
            break;
        }
        // Ignore all other events and return 0.
        default:
        {
            break;
        }
    }
    SetLedColor(BLUE);
    return(0);
}

/***********************************************************************************************/
static uint32_t EchoNewDataToHost(tUSBDBulkDevice *psDevice, uint8_t *pui8Data, uint32_t ui32NumBytes)
{
    uint32_t ui32Loop, ui32Space, ui32Count;
    uint32_t ui32ReadIndex;
    uint32_t ui32WriteIndex;
    tUSBRingBufObject sTxRing;
    // Get the current buffer information to allow us to write directly to
    // the transmit buffer (we already have enough information from the
    // parameters to access the receive buffer directly).
    USBBufferInfoGet(&g_sTxBuffer, &sTxRing);
    // How much space is there in the transmit buffer?
    ui32Space = USBBufferSpaceAvailable(&g_sTxBuffer);
    // How many characters can we process this time round?
    ui32Loop = (ui32Space < ui32NumBytes) ? ui32Space : ui32NumBytes;
    ui32Count = ui32Loop;
    // Update our receive counter.
    g_ui32RxCount += ui32NumBytes;
    // Dump a debug message.
    //DEBUG_PRINT("Received %d bytes\n", ui32NumBytes);
    // Set up to process the characters by directly accessing the USB buffers.
    ui32ReadIndex = (uint32_t)(pui8Data - g_pui8USBRxBuffer);
    ui32WriteIndex = sTxRing.ui32WriteIndex;

    while(ui32Loop)
    {
        // Copy from the receive buffer to the transmit buffer converting character case on the way.
        // Is this a lower case character?
        if((g_pui8USBRxBuffer[ui32ReadIndex] >= 'a') &&
           (g_pui8USBRxBuffer[ui32ReadIndex] <= 'z'))
        {
            g_pui8USBTxBuffer[ui32WriteIndex] =
                (g_pui8USBRxBuffer[ui32ReadIndex] - 'a') + 'A';
        }
        else
        {
            if((g_pui8USBRxBuffer[ui32ReadIndex] >= 'A') &&
               (g_pui8USBRxBuffer[ui32ReadIndex] <= 'Z'))
            {
                g_pui8USBTxBuffer[ui32WriteIndex] =
                    (g_pui8USBRxBuffer[ui32ReadIndex] - 'Z') + 'z';
            }
            else
            {
                // Copy the received character to the transmit buffer.
                g_pui8USBTxBuffer[ui32WriteIndex] =
                        g_pui8USBRxBuffer[ui32ReadIndex];
            }
        }
        // Move to the next character taking care to adjust the pointer for the buffer wrap if necessary.
        ui32WriteIndex++;
        ui32WriteIndex = (ui32WriteIndex == BULK_BUFFER_SIZE) ?
                         0 : ui32WriteIndex;

        ui32ReadIndex++;
        ui32ReadIndex = (ui32ReadIndex == BULK_BUFFER_SIZE) ?
                        0 : ui32ReadIndex;

        ui32Loop--;
    }
    // We've processed the data in place so now send the processed data back to the host.
    USBBufferDataWritten(&g_sTxBuffer, ui32Count);

    //DEBUG_PRINT("Wrote %d bytes\n", ui32Count);
    // We processed as much data as we can directly from the receive buffer so
    // we need to return the number of bytes to allow the lower layer to
    // update its read pointer appropriately.
    SetLedColor(BLUE);
    return(ui32Count);
}
/************************************************************************************************/
uint32_t send_data_to_host(uint8_t *ui8Data, uint32_t ui32MsgLen)
{
    //SetLedColor(GREEN);
    uint32_t ui32Loop, ui32Space, ui32Count, ui32ReadIndex = 0;
    uint32_t ui32WriteIndex;
    uint32_t ui32DataLen = ui32MsgLen;
    tUSBRingBufObject sTxRing;
    while(ui32MsgLen)
    {
        //get current buffer info
        USBBufferInfoGet(&g_sTxBuffer, &sTxRing);
        ui32Space = USBBufferSpaceAvailable(&g_sTxBuffer);
        ui32Loop = (ui32Space < ui32MsgLen) ? ui32Space : ui32MsgLen; 
        //DEBUG_PRINT("Sending %d bytes\n", ui32Loop);
        ui32MsgLen -= ui32Loop;
        ui32Count = ui32Loop;
        ui32WriteIndex = sTxRing.ui32WriteIndex;
        SetLedColor(GREEN);
        //SysCtlDelay(500);
        while(ui32Loop)
        {
            g_pui8USBTxBuffer[ui32WriteIndex] = ui8Data[ui32ReadIndex];
            ui32ReadIndex++;
            ui32WriteIndex++;
            ui32WriteIndex = (ui32WriteIndex == BULK_BUFFER_SIZE) ? 0 : ui32WriteIndex;
            ui32Loop--;
        }
        USBBufferDataWritten(&g_sTxBuffer,ui32Count);
        g_bUSBSent = false;
        if(ui32MsgLen)
        {
            while(!(g_bUSBSent))
            {}
            g_bUSBSent = false;
        }
    }
    return(ui32DataLen);
}
/*************************************************************************************************/
uint32_t get_data_from_host(tUSBDBulkDevice *psDevice, uint8_t *pui8Data, 
                            uint32_t ui32NumBytes)
{

    //SysCtlDelay(500);
    uint8_t newData[64];
    //SetLedColor(GREEN);
    uint32_t ui32index = 0x0;
    uint32_t ui32MsgLen = ui32NumBytes;
    g_ui32RxCount += ui32NumBytes;
    uint32_t ui32ReadIndex = (uint32_t)(pui8Data-g_pui8USBRxBuffer);
    while(ui32MsgLen)
    {
        newData[ui32index] = g_pui8USBRxBuffer[ui32ReadIndex];
        SetLedColor(WHITE);
        ui32ReadIndex ++;
        ui32index++;
        ui32ReadIndex = (ui32ReadIndex == BULK_BUFFER_SIZE) ? 0 : ui32ReadIndex;
        ui32MsgLen --;
    }
    SetLedColor(WHITE);
    return(send_data_to_host(newData,ui32NumBytes));
}
/*************************************************************************************************/

void initUSB()
{
    // Enable lazy stacking for interrupt handlers.  This allows floating-point instructions to be used within interrupt handlers, but at the expense of extra stack usage.
    ROM_FPULazyStackingEnable();
    g_bUSBConfigured = false;
    // Enable the GPIO peripheral used for USB, and configure the USB pins.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    ROM_GPIOPinTypeUSBAnalog(GPIO_PORTD_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    // Enable the system tick.
    ROM_SysTickPeriodSet(ROM_SysCtlClockGet() / SYSTICKS_PER_SECOND);
    ROM_SysTickIntEnable();
    ROM_SysTickEnable();
    // Initialize the transmit and receive buffers.
    USBBufferInit(&g_sTxBuffer);
    USBBufferInit(&g_sRxBuffer);
    // Set the USB stack mode to Device mode with VBUS monitoring.
    USBStackModeSet(0, eUSBModeForceDevice, 0);
    // Pass our device information to the USB library and place the device on the bus.
    USBDBulkInit(0, &g_sBulkDevice);
    g_bUSBSent = false;
    SetLedColor(GREEN);
}
