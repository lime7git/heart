#include "lis3dh.h"

/*
	HAL_I2C_Mem_Read(&hi2c1, LIS3DH_READ_ADDRESS, 0x0F, 1, (uint8_t*)&read_buffer, 1, HAL_MAX_DELAY);	
	
	buffer = 0x97;	
	HAL_I2C_Mem_Write(&hi2c1, LIS3DH_WRITE_ADDRESS, 0x20, 1, (uint8_t *)&buffer, sizeof(buffer), HAL_MAX_DELAY);	//  data rate, axis enable
	
	buffer = 0x38;	
	HAL_I2C_Mem_Write(&hi2c1, LIS3DH_WRITE_ADDRESS, 0x23, 1, (uint8_t *)&buffer, sizeof(buffer), HAL_MAX_DELAY);	//  high resolution
	
	buffer = 0x2A;	
	HAL_I2C_Mem_Write(&hi2c1, LIS3DH_WRITE_ADDRESS, 0x38, 1, (uint8_t *)&buffer, sizeof(buffer), HAL_MAX_DELAY);	//  double click all axes
	
	buffer = 0x38;	
	HAL_I2C_Mem_Write(&hi2c1, LIS3DH_WRITE_ADDRESS, 0x23, 1, (uint8_t *)&buffer, sizeof(buffer), HAL_MAX_DELAY);	//  high resolution full scale
	
	buffer = 0x5;	
	HAL_I2C_Mem_Write(&hi2c1, LIS3DH_WRITE_ADDRESS, 0x3A, 1, (uint8_t *)&buffer, sizeof(buffer), HAL_MAX_DELAY);	//  click threshold
	
	buffer = 0xA;	
	HAL_I2C_Mem_Write(&hi2c1, LIS3DH_WRITE_ADDRESS, 0x3B, 1, (uint8_t *)&buffer, sizeof(buffer), HAL_MAX_DELAY);	//  time limit
	
	buffer = 0xFF;	
	HAL_I2C_Mem_Write(&hi2c1, LIS3DH_WRITE_ADDRESS, 0x3C, 1, (uint8_t *)&buffer, sizeof(buffer), HAL_MAX_DELAY);	//  time latency
	
	buffer = 0xFF;	
	HAL_I2C_Mem_Write(&hi2c1, LIS3DH_WRITE_ADDRESS, 0x3D, 1, (uint8_t *)&buffer, sizeof(buffer), HAL_MAX_DELAY);	//  time window
		*/

	/*	
		volatile uint8_t click_read;
		HAL_I2C_Mem_Read(&hi2c1, LIS3DH_READ_ADDRESS, 0x39, 1, (uint8_t*)&click_read, 1, HAL_MAX_DELAY);
		
		if(click_read & 0x20) acc_interrupt = 255;
		
	
		HAL_I2C_Mem_Read(&hi2c1, LIS3DH_READ_ADDRESS, 0x28, 1, (uint8_t*)&x1, 1, HAL_MAX_DELAY);
		HAL_I2C_Mem_Read(&hi2c1, LIS3DH_READ_ADDRESS, 0x29, 1, (uint8_t*)&x2, 1, HAL_MAX_DELAY);
		accx = (int16_t)(((uint16_t)x2 << 8) + ((uint16_t)x1 << 0));
		accx_g = 48 * ((double)accx / 64000);
		
		HAL_I2C_Mem_Read(&hi2c1, LIS3DH_READ_ADDRESS, 0x2A, 1, (uint8_t*)&y1, 1, HAL_MAX_DELAY);
		HAL_I2C_Mem_Read(&hi2c1, LIS3DH_READ_ADDRESS, 0x2B, 1, (uint8_t*)&y2, 1, HAL_MAX_DELAY);
		accy = (int16_t)(((uint16_t)y2 << 8) + ((uint16_t)y1 << 0));
		accy_g = 48 * ((double)accy / 64000);
		
		HAL_I2C_Mem_Read(&hi2c1, LIS3DH_READ_ADDRESS, 0x2C, 1, (uint8_t*)&z1, 1, HAL_MAX_DELAY);
		HAL_I2C_Mem_Read(&hi2c1, LIS3DH_READ_ADDRESS, 0x2D, 1, (uint8_t*)&z2, 1, HAL_MAX_DELAY);
		accz = (int16_t)(((uint16_t)z2 << 8) + ((uint16_t)z1 << 0));
		accz_g = 48 * ((double)accz / 64000);
*/
