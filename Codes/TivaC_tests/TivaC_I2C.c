/**/

#include "basic_includes.h"
#include "TivaC_I2C.h"

void initI2C(bool bFast)
{
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
    I2CMasterSlaveAddrSet(I2C0_BASE, INA_ADDRESS, true);
}

void I2CreceiveData(uint8_t* ui8RxData)
{
    uint32_t ui32tmp;
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
    while(I2CMasterBusy(I2C0_BASE));
    ui32tmp = I2CMasterDataGet(I2C0_BASE);
    ui8RxData[0] = (uint8_t)ui32tmp;
    I2CMasterControl (I2C0_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
    while(I2CMasterBusy(I2C0_BASE));
    ui32tmp = I2CMasterDataGet(I2C0_BASE);
    ui8RxData[1] = (uint8_t)ui32tmp;
}

void I2CsendData(uint8_t ui8TxData)
{
    /* change I2CMasterSlaveAddrSet before using */
    I2CMasterDataPut(I2C0_BASE, ui8TxData);
    I2CMasterControl(I2C0_BASE, I2C_MASTER_CMD_SINGLE_SEND);
    //while(I2CMasterBusy(I2C0_BASE));
}
