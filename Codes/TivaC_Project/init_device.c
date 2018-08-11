// 

#include "basic_includes.h"
#include "init_device.h" 

void setPWMLEDColor(uint32_t *color)
{
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, color[0]);
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, color[1]);
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, color[2]);
}

void initPWM_LEDs(uint32_t ui32Period)
{
	//Enable Peripherals
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
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
	// Set PWM period 4000 steps at 50MHz --> 50KHz
	ROM_PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, ui32Period);
	ROM_PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, ui32Period);
	//set default vaule to nearly zero
	//SetLedColor(BLACK);
	//ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, 1);
	//ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, 1);
	//ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, 1);
	// Enable PWM generation
	ROM_PWMGenEnable(PWM1_BASE, PWM_GEN_2);
	ROM_PWMGenEnable(PWM1_BASE, PWM_GEN_3);
	// Enable output to pins
	ROM_PWMOutputState(PWM1_BASE, (PWM_OUT_5_BIT | PWM_OUT_6_BIT | PWM_OUT_7_BIT ), true);
}

void initPWM_Peltier(uint32_t ui32Period)
{
	//Enable Peripherals
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	//remove GPIO Lock
	ROM_SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
	//Configure Pin MUX
	ROM_GPIOPinConfigure(GPIO_PF0_M1PWM4);
	//configure PIn types
	ROM_GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_0);
	//configure PWMM generation modules
	ROM_PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	// Set PWM period 4000 steps at 50MHz --> 50KHz
	ROM_PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, ui32Period);
	//set default vaule to nearly zero
	ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_4, 1);
	// Enable PWM generation
	ROM_PWMGenEnable(PWM1_BASE, PWM_GEN_2);
	// Enable output to pins
	ROM_PWMOutputState(PWM1_BASE, PWM_OUT_4_BIT , true);
}

void initI2C(bool bFast)
{
	// enable I2C peripheral
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
	// I2C0 on port B
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	// Configure Pin Mux
	MAP_GPIOPinConfigure(GPIO_PB2_I2C0SCL);
	MAP_GPIOPinConfigure(GPIO_PB3_I2C0SDA);
	GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
	GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);
	// Set clock 
	MAP_I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), bFast);
	// True = Read from slave    // False = Write to slave
	
	// Setup INA averaging mode
	I2CMasterSlaveAddrSet(I2C0_BASE, INA_ADDRESS, false);
	// start communication
	I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
	// select config registrer
	I2CMasterDataPut(I2C0_BASE, 0x00);
	while(I2CMasterBusy(I2C0_BASE));
	// MSB
	I2CMasterDataPut(I2C0_BASE, (0b110 << 4) | (INA_avg_index << 1) | 0x01);
	while(I2CMasterBusy(I2C0_BASE));
	// Last Byte of transfer
	I2CMasterControl (I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
	// LSB
	I2CMasterDataPut(I2C0_BASE,(INA_conv_index << 3) | 0b101);
	while(I2CMasterBusy(I2C0_BASE));
	
	// set I2C module for data receiving
	I2CMasterSlaveAddrSet(I2C0_BASE, INA_ADDRESS, true);
}

void initSPI(uint32_t ui32Protocol, uint32_t ui32Mode, 
						uint32_t ui32BitRate, uint32_t ui32DataWidth)
{
	// TODO : Chage peripheral and pins accordingly
	// Enable peripheral
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
	// Enable port
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); 
	// set CS-bar pin high
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_3 | GPIO_PIN_6 | GPIO_PIN_7);
	GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3 | GPIO_PIN_6 | GPIO_PIN_7 , 
				 GPIO_PIN_3 | GPIO_PIN_6 | GPIO_PIN_7);
	// Configure Pin mux
	MAP_GPIOPinConfigure(GPIO_PA2_SSI0CLK);
	/// FSS is useless
	//MAP_GPIOPinConfigure(GPIO_PA3_SSI0FSS);
	MAP_GPIOPinConfigure(GPIO_PA4_SSI0RX);
	MAP_GPIOPinConfigure(GPIO_PA5_SSI0TX);
	// Configure Pins
	// TODO : Add GPIO_PIN_3 to enable FSS
	GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_4 | 
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

void initTimer(uint32_t ui32SetValue)
{
	// Enable Timer1
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
	//ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	// Enable the GPIO pins for the LED (PF1 & PF2).
	//ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_1);
	/* Timer colock at 50MHz -> System clock*/
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x02);
	// Enable Master interrupt
	MAP_IntMasterEnable();
	// Periodic mode
	MAP_TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
	// Set Prescalar
	// MAP_TimerPrescaleSet(TIMER1_BASE, TIMER_A,ui32PreScalar);
	// Set load Value
	MAP_TimerLoadSet(TIMER1_BASE, TIMER_A, ui32SetValue);
	// MAP_TimerLoadSet(TIMER1_BASE, TIMER_A, ROM_SysCtlClockGet());
	// Enable timer interrupt
	MAP_IntEnable(INT_TIMER1A);
	// Set interrupt condition
	MAP_TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
	// Start Timer1A
	MAP_TimerEnable(TIMER1_BASE, TIMER_A);
}
