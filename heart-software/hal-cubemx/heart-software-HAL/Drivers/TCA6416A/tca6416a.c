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

void TCA6416A_Write(uint8_t register_address, uint8_t data_to_transfer)
{
	HAL_I2C_Mem_Write(&hi2c1, TCA6416A_WRITE_ADDRESS, register_address, 1, (uint8_t *)&data_to_transfer, 1, HAL_MAX_DELAY);
}

void TCA6416A_Read(uint8_t register_address, uint8_t *data_received)
{
	
}

void TCA6416A_Set_Port_0_As_Output(void)
{	
	TCA6416A_Write(TCA6416A_CONFIGURATION_0_ADDRESS, 0x0);
}

void TCA6416A_Set_Port_1_As_Output(void)
{
	TCA6416A_Write(TCA6416A_CONFIGURATION_1_ADDRESS, 0x0);
}

void TCA6416A_Disable_All_LEDs(void)
{
	TCA6416A_Write(TCA6416A_OUTPUT_PORT_0_ADDRESS, 0xFF);
	TCA6416A_Write(TCA6416A_OUTPUT_PORT_1_ADDRESS, 0xFF);
}

void TCA6416A_Enable_All_LEDs(void)
{
	TCA6416A_Write(TCA6416A_OUTPUT_PORT_0_ADDRESS, 0x0);
	TCA6416A_Write(TCA6416A_OUTPUT_PORT_1_ADDRESS, 0x0);
}
