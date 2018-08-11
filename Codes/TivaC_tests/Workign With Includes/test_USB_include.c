#include "basic_includes.h"
#define DEFINE_VARIABLES
#include "TivaC_USB.h"
#include "TivaC_LED.h"
//#include "int_handlers.c"

int main()
{
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    PWMSetLeds(1000);
    SysCtlDelay(5000000);
    initUSB();
    SetLedColor(RED);
    SysCtlDelay(5000);
    while(1)
    {}
}
