/**
 * @file lis3dh.c
 * @author lime7git
 * @date 16.02.2022
 * @brief lis3dh source file
 */

#include "lis3dh.h"

void LIS3DH_Initialization(void)
{

}

void LIS3DH_Write(uint8_t register_address, uint8_t *data_to_transfer)
{
    I2C_Write(LIS3DH_WRITE_ADDRESS, register_address, data_to_transfer, 1);
}

void LIS3DH_Read(uint8_t register_address, uint8_t *data_received)
{
    I2C_Read(LIS3DH_READ_ADDRESS, register_address, data_received, 1);
}