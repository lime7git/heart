/**
 * @file tca6416a.c
 * @author lime7git
 * @date 16.02.2022
 * @brief tca6416a source file
 */

#include "tca6416a.h"

void TCA6416A_Initialization(void)
{
    TCA6416A_Set_Port_0_As_Output();
    TCA6416A_Set_Port_1_As_Output();
}

void TCA6416A_Write(uint8_t register_address, uint8_t *data_to_transfer)
{
    I2C_Write(TCA6416A_WRITE_ADDRESS, register_address, data_to_transfer, 1);
}

void TCA6416A_Read(uint8_t register_address, uint8_t *data_received)
{
    I2C_Read(TCA6416A_READ_ADDRESS, register_address, data_received, 1);
}

void TCA6416A_Set_Port_0_As_Output(void)
{
    TCA6416A_Write(TCA6416A_CONFIGURATION_0_ADDRESS, (uint8_t *)0x0);
}

void TCA6416A_Set_Port_1_As_Output(void)
{
    TCA6416A_Write(TCA6416A_CONFIGURATION_1_ADDRESS, (uint8_t *)0x0);
}