#include "basic_includes.h"
#include "temp_loop.h"
#include "TivaC_USB.h"

int32_t Kp = 125;
int32_t invKi = 3500;
int32_t  i32error = 0, i32pi = 0;
bool brelayFlag = false, bchangeFlag = true;

int32_t min(int32_t a, int32_t b){
    return (a > b ? b: a);
}

int32_t max(int32_t a, int32_t b){
    return (a < b ? b: a);
}

void temp_loop()
{
		read_temp_loop_data();
		i32error = (g_i32SetTemp - g_i32Temp);
		i32pi = (Kp * i32error) + (g_i32integral / invKi) ;
		g_i32integral += i32error;
		// pi 0 logic here
		
		if(g_i32SetTemp < (int32_t)g_ui32rTemp - 17)
		{
		    if(brelayFlag == false)
		        bchangeFlag = true;
            brelayFlag = true;
			PWMPulseWidthSet(PWM1_BASE, PWM_OUT_4, max(1, min(-i32pi, 4095)));
		}
		else if(g_i32SetTemp > (int32_t)g_ui32rTemp + 17)
		{
		    if(brelayFlag == true)
		        bchangeFlag = true;
			brelayFlag = false;
			PWMPulseWidthSet(PWM1_BASE, PWM_OUT_4, max(1, min(i32pi, 4095)));
		}
		else{
			//Do Nothing
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
