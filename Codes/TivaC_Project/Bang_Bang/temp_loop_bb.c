#include "basic_includes.h"
#include "temp_loop_bb.h"
#include "TivaC_USB.h"

int32_t  i32error = 0;
bool brelayFlag = false, bchangeFlag = true;

void temp_loop()
{
	read_temp_loop_data();
	i32error = (g_i32SetTemp - g_i32Temp);
	
	if(g_i32SetTemp < (int32_t)g_ui32rTemp - 17)
	{
		if(brelayFlag == false)
			bchangeFlag = true;
		
		brelayFlag = true;
		
		if(i32error < -13)
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_PIN_0);
		else
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0x00);
	}
	else if(g_i32SetTemp > (int32_t)g_ui32rTemp + 17)
	{
		if(brelayFlag == true)
			bchangeFlag = true;
		
		brelayFlag = false;
		
		if(i32error > 13)
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_PIN_0);
		else
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0x00);
	}
	
	if(bchangeFlag){
		GPIOPinWrite(Relay_Port_Base, Peltier_Relay_Pin, (brelayFlag ? Peltier_Relay_Pin: 0x00));
		bchangeFlag = false;
	}
	send_temp_loop_data();
	
}

void read_temp_loop_data()
{
	// Start I2C msg
	//I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
	// SPI select ADC
	GPIOPinWrite(SPI_BASE, ADC_CS , 0x00);
	// get LM35p and LM35n 
	SSIDataPut(SSI0_BASE, ADC_MASK_LM35p);
    while(SSIBusy(SSI0_BASE));
    GPIOPinWrite(SPI_BASE, ADC_CS, ADC_CS);
    SSIDataGet(SSI0_BASE, &g_ui32tmp0);

    GPIOPinWrite(SPI_BASE, ADC_CS , 0x00);
	SSIDataPut(SSI0_BASE, ADC_MASK_LM35m);
    while(SSIBusy(SSI0_BASE));
    GPIOPinWrite(SPI_BASE, ADC_CS, ADC_CS);
    SSIDataGet(SSI0_BASE, &g_ui32tmp0);

    GPIOPinWrite(SPI_BASE, ADC_CS , 0x00);
    SSIDataPut(SSI0_BASE, ADC_MASK_RTemp);
    while(SSIBusy(SSI0_BASE));
    GPIOPinWrite(SPI_BASE, ADC_CS, ADC_CS);
    SSIDataGet(SSI0_BASE, &g_ui32tmp1);

    GPIOPinWrite(SPI_BASE, ADC_CS , 0x00);
    SSIDataPut(SSI0_BASE, 0x00);
	while(SSIBusy(SSI0_BASE));
    GPIOPinWrite(SPI_BASE, ADC_CS, ADC_CS);
    SSIDataGet(SSI0_BASE, &g_ui32rTemp);

    // Get data from FIFO
	g_ui32rTemp = 246;
	// Calculate Temperature
	g_i32Temp = g_ui32tmp0 - g_ui32tmp1;
	// I2C receive MLB
	//while(I2CMasterBusy(I2C0_BASE));
	//g_ui32tmp0 = I2CMasterDataGet(I2C0_BASE);
	//I2CMasterControl (I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
	//while(I2CMasterBusy(I2C0_BASE));
	// I2C receive LSB
	//g_ui32tmp1 = I2CMasterDataGet(I2C0_BASE);
	// Calculate Current value
	g_i32PeltCurrent = (g_ui32tmp0 << 8) | g_ui32tmp1 ;
}

void send_temp_loop_data()
{
	g_ui8tempLoopData[0] = (uint8_t)((g_i32Temp * 500) >> 12);
	//g_ui8tempLoopData[TempByte0] = uint8_t(g_ui32Temp >> 8);
	//g_ui8tempLoopData[PIByte1] = uint8_t(g_ui32PeltCurrent);
	//g_ui8tempLoopData[PIByte0] = uint8_t(g_ui32PeltCurrent >> 8);
	//g_newTempData = true;
	send_data_to_host(g_ui8tempLoopData, 1);
}

void start_temp_loop()
{
	g_ui8tempLoopData[0] = 0x22;
	g_ui8tempLoopData[1] = 0x59;
	g_ui8tempLoopData[8] = 0x00;
	g_ui8tempLoopData[9] = 0x0A;
	//g_temp_loop_active = true;
}

void init_bang_bang()
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	MAP_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0x00);
}