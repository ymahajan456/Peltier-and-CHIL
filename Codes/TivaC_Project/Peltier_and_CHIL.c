// main File
#define DEFINE_VARIABLES
#include "basic_includes.h"
#include "TivaC_USB.h"
#include "init_device.h"
#include "temp_loop.h"
#include "IV.h"

uint32_t g_timerState = 0;

int main()
{
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	initPWM_LEDs(4096);
	initUSB();
	setPWMLEDColor(RED);
	initPWM_Peltier(4096);
	g_bStartIV = true;
	//initI2C(false);
	MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_3);
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_3 | GPIO_PIN_6 | GPIO_PIN_7);
	initSPI(SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 100000, 16);
	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_4 );
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_4, 0x00);
	SysCtlDelay(50000);
	initTimer(500000);
	while(1)
	{
		/*if(g_bNewIVData == 1)
		{
			g_bNewIVData = 0;
			send_data_to_host(g_ui8IVData, 5);
		}*/
		/*
		if(g_bNewTempData)
		{
			g_bNewTempData = false;
			send_data_to_host(g_ui8tempLoopData,10);
		}*/
	}
}
/*
uint32_t decode_msg(uint8_t *ui8msg, uint32_t ui32dataLen)
{
	// check if message is correct
	if((ui8msg[ui32dataLen-1] != ui32dataLen) || (ui8msg[ui32dataLen-2] != 0x00)
		|| (ui8msg[0] != COMMAND))
		return ui32dataLen;
	
	switch(ui8msg[1])
	{
		case START_IV :
			g_bTempStable = false;
			g_ui32SetTemp = (ui8msg[2] << 8) | ui8msg[3];
			startIV();
			break;
			
		case SET_DEVICE :
			g_bNegativeIV = (ui8msg[2] > 0x00);
			g_ui32DeviceVmax = (ui8msg[3] << 8) | ui8msg[4] ;
			g_ui32DeviceVmin = (ui8msg[5] << 8) | ui8msg[6] ;
			g_ui32DeviceImax = (ui8msg[7] << 8) | ui8msg[8] ;
			g_ui32DeviceImin = (ui8msg[9] << 8) | ui8msg[10] ;
			break;
			
		case START_DEVICES :
			initPWM_Peltier(4000);
			initI2C(false);
			initSPI(SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 1000000, 16);
			initTimer(500000);		// 10ms
			break;
			
		case STOP_DEVICES :
			// stop Timer
			MAP_SysCtlPeripheralDisable(SYSCTL_PERIPH_TIMER1);
			//set DAC value to zero
			GPIOPinWrite(SPI_BASE, DAC_CS, 0x00);
			SSIDataPut(SSI0_BASE, DAC_MASK | 0x00);
			while(SSIBusy(SSI0_BASE));
			GPIOPinWrite(SPI_BASE, DAC_CS, DAC_CS);
			// make PWM output zero
			PWMPulseWidthSet(PWM1_BASE, PWM_OUT_4, 0);
			// wait for some time for device to set
			SysCtlDelay(50000);
			MAP_SysCtlPeripheralDisable(SYSCTL_PERIPH_SSI0);
			MAP_SysCtlPeripheralDisable(SYSCTL_PERIPH_I2C0);
			MAP_SysCtlPeripheralDisable(SYSCTL_PERIPH_GPIOA);
			MAP_SysCtlPeripheralDisable(SYSCTL_PERIPH_GPIOC);
			MAP_SysCtlPeripheralDisable(SYSCTL_PERIPH_GPIOB);

			Dont Disable D: USB is on D
			
			break;
			
		default :
			break;
	}
	send_data_to_host(g_ui8msg_acc, 4);
}
*/

uint32_t decode_msg(uint8_t *ui8msg, uint32_t ui32dataLen)
{
	g_i32SetTemp = (ui8msg[0] << 12) / 500;
	return ui32dataLen;
}

void Timer1AIntHandler(void)
{
	// Clear Timer A Timeout interrupt
	TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
	
	if(g_timerState == 0)
	{
		temp_loop();
		setPWMLEDColor(BLUE);
	}
	else
	{
		get_IV();
		setPWMLEDColor(GREEN);
	}
	g_timerState = (g_timerState + 1) % 4;
}
