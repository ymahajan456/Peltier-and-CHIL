#include "basic_includes.h"
#include "TivaC_SPI.h"
#define NON_BLOCKING_SPI

void initSPI(uint32_t ui32Protocol, uint32_t ui32Mode, uint32_t ui32BitRate, 
             uint32_t ui32DataWidth)
{
    // TODO : Chage peripheral and pins accordingly
    // Enable peripheral
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
    // Enable port
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);  
    // Configure Pin mux
    MAP_GPIOPinConfigure(GPIO_PA2_SSI0CLK);
    MAP_GPIOPinConfigure(GPIO_PA3_SSI0FSS);
    MAP_GPIOPinConfigure(GPIO_PA4_SSI0RX);
    MAP_GPIOPinConfigure(GPIO_PA5_SSI0TX);
    // Configure Pins
    GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3 | 
            GPIO_PIN_2);
    // TODO : Change Base
    MAP_SSIConfigSetExpClk(SSI0_BASE, MAP_SysCtlClockGet(), ui32Protocol,
                           ui32Mode, ui32BitRate, ui32DataWidth);
    MAP_SSIEnable(SSI0_BASE);
    //Empty receive buffer on strtup
    while(SSIDataGetNonBlocking(SSI0_BASE, &ui32Mode))
    {
    }
}

uint32_t SPIexchangeData(uint32_t ui32TxData = 0x0000)
{
    uint32_t ui32RxData;
    bool bTransmitted;
    #ifdef NON_BLOCKING_SPI
        bTransmitted = SSIDataPutNonBlocking(SSI0_BASE, ui32TxData);
        if(bTransmitted)
            while(SSIBusy(SSI0_BASE));
        bTransmitted = SSIDataGetNonBlocking(SSI0_BASE, &ui32RxData);
    #else
        SSIDataPut(SSI0_BASE, ui32TxData);
        while(SSIBusy(SSI0_BASE));
        SSIDataGet(SSI0_BASE, &ui32RxData);
    #endif
    return ui32RxData;
}

uint32_t SPIGetData(void)
{
    int32_t ui32RxData = 0x0000;
    bool bTransmitted;
    #ifdef NON_BLOCKING_SPI
        bTransmitted = SSIDataGetNonBlocking(SSI0_BASE, &ui32RxData);
        if(bTransmitted)
            while(SSIBusy(SSI0_BASE));
    #else
        SSIDataGet(SSI0_BASE, &ui32RxData);
    #endif
    return ui32RxData;
}

void SPIsendData(uint32_t ui32TxData)
{
    bool bTransmitted;
    #ifdef NON_BLOCKING_SPI
        bTransmitted = SSIDataPutNonBlocking(SSI0_BASE, ui32TxData);
        if(bTransmitted)
            while(SSIBusy(SSI0_BASE));
    #else
        SSIDataPut(SSI0_BASE, ui32TxData);
    #endif
}
