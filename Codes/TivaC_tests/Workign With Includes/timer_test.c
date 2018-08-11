/**/
#define DEFINE_VARIABLES
//#include "TivaC_UART.h"
#include "basic_includes.h"
#include "TivaC_USB.h"
#include "TivaC_LED.h"
#include "TivaC_Timer.h"
uint8_t sendmain[3] = {'M','A','N'};
int main()
{
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    PWMSetLeds(1000);
    //initUART(115200);
    initUSB();
    //SetLedColor(RED);
    //while(!g_bUSBConfigured);
    initTimer1A(256,62500);
    while(1)
    {
        //SysCtlDelay(25000000);
        //send_data_to_host(sendmain,3);
        //SetLedColor(GREEN);
        //SysCtlDelay(25000000);
        //SetLedColor(BLACK);
    }
}
