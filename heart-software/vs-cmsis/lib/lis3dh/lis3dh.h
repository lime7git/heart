/**
 * @file lis3dh.h
 * @author lime7git
 * @date 16.02.2022
 * @brief ST LIS3DH 3-axsis accelerometer header file
 */

/* File lis3dh  */
#ifndef _LIS3DH_H
#define _LIS3DH_H

#include "stm32l031xx.h"
#include "stm32l0xx.h"

#include "i2c.h"

#define LIS3DH_WRITE_ADDRESS    0x30
#define LIS3DH_READ_ADDRESS     0x31

#define LIS3DH_WHO_AM_I_REGISTER    0x0F    // Device identification register by default - 0x33

void LIS3DH_Initialization(void);

void LIS3DH_Write(uint8_t register_address, uint8_t *data_to_transfer);
void LIS3DH_Read(uint8_t register_address, uint8_t *data_received);

#endif /* !LIS3DH_H */