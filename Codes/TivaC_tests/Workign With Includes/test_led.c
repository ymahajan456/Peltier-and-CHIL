/*
*/

#include "basic_includes.h"
#include "TivaC_LED.h"

#define SYSTICKS_PER_SECOND     100
#define SYSTICK_PERIOD_MS       (1000 / SYSTICKS_PER_SECOND)

volatile uint32_t g_ui32SysTickCount = 0;
void
SysTickIntHandler(void)
{
    //
    // Update our system tick counter.
    //
    g_ui32SysTickCount++;
}

int main(void)
{
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
    PWMSetLeds(1000);
    uint32_t color[3] = {1000,5,5};
    uint32_t decColour, incColour, i;
    while(1){
        for (decColour = 0; decColour < 3; decColour += 1)
        {
            incColour = decColour == 2 ? 0 : decColour + 1;

            // cross-fade the two colours.
            for(i = 5; i < 1000; i += 5)
            {
                color[decColour] -= 5;
                color[incColour] += 5;
                SetLedColor(color);
                //PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5,color[0]);
                //PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,color[1]);
                //PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7,color[2]);
                SysCtlDelay(50000);
            }
        }
    }
}

