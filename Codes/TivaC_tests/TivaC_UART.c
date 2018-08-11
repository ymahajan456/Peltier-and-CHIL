/**/

#include "basic_includes.h"
#include "TivaC_UART.h"

uint32_t UARTSend(uint32_t ui32UARTBase, const uint8_t *pui8Buffer, uint32_t ui32Count)
{
    while(ui32Count--)
    {
        #ifdef NON_BLOCKING_UART
            if(!MAP_UARTCharPutNonBlocking(ui32UARTBase, *pui8Buffer++))
                break;
        #else
            MAP_UARTCharPut(ui32UARTBase, *pui8Buffer++);
        #endif
    }
    return(ui32Count);
}

void initUART(uint32_t baudRate)
{  
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);  // use internal Clock
    UARTStdioConfig(0, baudRate, 16000000);
    
    IntMasterEnable(); //enable processor interrupts
    IntEnable(INT_UART0); //enable the UART interrupt
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT); //only enable RX and RT interrupts
}

void UARTIntHandler(void)
{
    UARTIntClear(UART0_BASE, UARTIntStatus(UART0_BASE,true));
    while(UARTCharsAvail(UART0_BASE)) //loop while there are chars
    {
        UARTCharPutNonBlocking(UART0_BASE, UARTCharGetNonBlocking(UART0_BASE)); //echo character
    }
}



