#include "basic_includes.h"
#define DEFINE_VARIABLES
#include "TivaC_USB.h"
#include "TivaC_LED.h"
#include "TivaC_UART.h"
#include "TivaC_I2C.h"

uint8_t ui8INAdata[3];
int main()
{
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    PWMSetLeds(1000);
    ui8INAdata[3] = "\n";
    initUSB();
    initI2C(false);
    SetLedColor(RED);
    SysCtlDelay(50000);
    while(1)
    {
        while(g_bUSBConfigured)
        {
            I2CreceiveData(ui8INAdata);
            send_data_to_host(ui8INAdata,3);
            SetLedColor(GREEN);
            SysCtlDelay(5000000);
            SetLedColor(BLUE);
        }
    }
}
