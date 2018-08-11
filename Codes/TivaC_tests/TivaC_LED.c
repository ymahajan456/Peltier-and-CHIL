/*
    Define all LED related functions 
*/

#include "basic_includes.h"
#include "TivaC_LED.h"

void PWMSetLeds(uint32_t ui32Period)
{
    //Enable Peripherals
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!(ROM_SysCtlPeripheralReady(SYSCTL_PERIPH_PWM1) & ROM_SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)))
    {}
    
    //remove GPIO Lock
    ROM_SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    
    //Configure Pin MUX
    ROM_GPIOPinConfigure(GPIO_PF1_M1PWM5);
    ROM_GPIOPinConfigure(GPIO_PF2_M1PWM6);
    ROM_GPIOPinConfigure(GPIO_PF3_M1PWM7);
    
    //configure PIn types
    ROM_GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    
    //configure PWMM generation modules
    ROM_PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
    ROM_PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
    
    // Set PWM period 1000 steps at 50MHz --> 50KHz
    ROM_PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, ui32Period);
    ROM_PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, ui32Period);
    
    //set default vaule to nearly zero
    SetLedColor(BLACK);
    //ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, 1);
    //ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, 1);
    //ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, 1);
    
    // Enable PWM generation
    ROM_PWMGenEnable(PWM1_BASE, PWM_GEN_2);
    ROM_PWMGenEnable(PWM1_BASE, PWM_GEN_3);

    // Enable output to pins
    ROM_PWMOutputState(PWM1_BASE, (PWM_OUT_5_BIT | PWM_OUT_6_BIT | PWM_OUT_7_BIT ), true);
}

void SetLeds()
{
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);  
}

void SetLedColor(uint32_t *color)
{
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, color[0]);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, color[1]);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, color[2]);
}
