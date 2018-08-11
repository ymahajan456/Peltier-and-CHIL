#include "basic_includes.h"
#include "IV.h"
#include "TivaC_USB.h"

bool negativeIV = false;
void get_IV()
{
	if(g_bTempStable && g_bStartIV)
	{
		//set DAC value
		GPIOPinWrite(SPI_BASE, DAC_CS, 0x00);
		SSIDataPut(SSI0_BASE, DAC_MASK | g_ui32DACValue);
		while(SSIBusy(SSI0_BASE));
		GPIOPinWrite(SPI_BASE, DAC_CS, DAC_CS);
		// wait for some time for device to set
		SSIDataGet(SSI0_BASE, &g_ui32tmp0);
		SysCtlDelay(50000);
		
		// Read DeviceP Voltage
		GPIOPinWrite(SPI_BASE, ADC_CS, 0x00);
		SSIDataPut(SSI0_BASE, ADC_MASK_IVm);
		while(SSIBusy(SSI0_BASE));
		GPIOPinWrite(SPI_BASE, ADC_CS, ADC_CS);
		SSIDataGet(SSI0_BASE, &g_ui32tmp0);
		
		// Read DeviceM Voltage
		GPIOPinWrite(SPI_BASE, ADC_CS, 0x00);
		SSIDataPut(SSI0_BASE, ADC_MASK_IVp);
		while(SSIBusy(SSI0_BASE));
		GPIOPinWrite(SPI_BASE, ADC_CS, ADC_CS);
		SSIDataGet(SSI0_BASE, &g_ui32tmp1);
		
		// calculate ABS(DeviceV)
		g_ui32tmp0 = g_ui32tmp0 * 11 / 2 - g_ui32tmp1;
		
		// update IV data buffer
		if(negativeIV)
			g_ui8IVData[0] = 0x1;
		else
			g_ui8IVData[0] = 0x0;
		
		g_ui8IVData[1] = g_ui32tmp0 >> 8;
		g_ui8IVData[2] = g_ui32tmp0;
		g_ui8IVData[3] = g_ui32tmp1 >> 8;
		g_ui8IVData[4] = g_ui32tmp1;
		send_data_to_host(g_ui8IVData, 5);
		
		// check device conditions
		// stopping IV conditions for negative bias
		if(negativeIV && ((g_ui32tmp0 > g_ui32DeviceVmin) || (g_ui32tmp1 > g_ui32DeviceImin)))
		{
			negativeIV = false;
			g_ui32DACValue = 0x00;
		}
		// stopping IV conditions for positive bias
		else if((g_ui32tmp0 >= g_ui32DeviceVmax) || (g_ui32tmp1 > g_ui32DeviceImax))
		{
			g_bStartIV = false;
			g_bIVComplete = true;
		}
		else if((!negativeIV) && (g_ui32DACValue == 0x01))
		{
			GPIOPinWrite(Relay_Port_Base, IV_Relay_Pin, 0x00);
		}
		
		g_ui32DACValue += 1;
		g_bNewIVData = true;
	}
	else if(g_bStartIV)
	{
	    g_ui32DACValue = 0;
	    GPIOPinWrite(SPI_BASE, DAC_CS, 0x00);
        SSIDataPut(SSI0_BASE, DAC_MASK);
        while(SSIBusy(SSI0_BASE));
        GPIOPinWrite(SPI_BASE, DAC_CS, DAC_CS);
        // wait for some time for device to set
        SSIDataGet(SSI0_BASE, &g_ui32tmp0);
	}
}

void start_IV()
{
	g_ui8IVData[0] = 0x22;
	g_ui8IVData[1] = 0x53;
	g_ui8IVData[7] = 0x00;
	g_ui8IVData[8] = 0x09;
	if(g_bNegativeIV)
	{
		GPIOPinWrite(Relay_Port_Base, IV_Relay_Pin, IV_Relay_Pin);
	}
	negativeIV = g_bNegativeIV;
	g_bStartIV = true;
	g_bIVComplete = false;
}
