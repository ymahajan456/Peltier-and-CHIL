#include "basic_includes.h"
#include "TivaC_Timer.h"
#include "TivaC_LED.h"

void initTimer1A(uint32_t ui32SetValue)
{
    // Enable Timer1  
    // Clock source s ssytem timer only -> 50MHz
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    // Enable Master interrupt
    MAP_IntMasterEnable();
    // Periodic mode
    MAP_TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
    // Set load Value
    //MAP_TimerLoadSet(TIMER1_BASE, TIMER_A, ui32SetValue);
    MAP_TimerLoadSet(TIMER1_BASE, TIMER_A, ui32SetValue);
    // Enable timer interrupt
    MAP_IntEnable(INT_TIMER1A);
    // Set interrupt condition
    MAP_TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    // Start Timer1A
    MAP_TimerEnable(TIMER1_BASE, TIMER_A);
}

void Timer1AIntHandler()
{
    // Clear Timer A Timeout interrupt
    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    ROM_IntMasterDisable();
    // Do task
    if(g_ui32Timer1AStatus)
    {
        g_ui32Timer1AStatus = 0;
        SetLedColor(BLACK);
    }
    else
    {
        g_ui32Timer1AStatus = 1;
        SetLedColor(GREEN);
    }
    ROM_IntMasterEnable();
}

void Timer1BIntHandler()
{
    // Clear Timer A Timeout interrupt
    TimerIntClear(TIMER1_BASE, TIMER_TIMB_TIMEOUT);   
}
