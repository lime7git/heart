/**
 * @file i2c.h
 * @author lime7git
 * @date 16.02.2022
 * @brief I2C bus header file
 */

/* File i2c  */
#ifndef _I2C_H
#define _I2C_H

#include "stm32l031xx.h"
#include "stm32l0xx.h"

#include "clock.h"

#define I2C_TRANSFER_DIRECTION_WIRTE (0x0 << I2C_CR2_RD_WRN_Pos)
#define I2C_TRANSFER_DIRECTION_READ (0x1 << I2C_CR2_RD_WRN_Pos)

/**
  * @brief  I2C1 peripheral initialization 
  * @param  None
  * @retval None
  */
void I2C_Initialization(void);

/**
  * @brief  GPIO (PA9, PA10) alternative function for I2C1 bus
  * @param  None
  * @retval None
  */
void GPIO_Alternative_Function_Initialization(void);

/**
  * @brief  I2C bus write to register
  * @param  device_address address of I2C slave device
  * @param  register_address address of I2C slave's register to write to
  * @param  data_to_transfer pointer to data to transfer
  * @param  data_to_transfer_size size of data (bytes)
  * @retval None
  */
void I2C_Write(uint8_t device_address, uint8_t register_address, uint8_t *data_to_transfer, uint8_t data_to_transfer_size);

/**
  * @brief  I2C bus read from register
  * @param  device_address address of I2C slave device
  * @param  register_address address of I2C slave's register to read from
  * @param  data_received pointer to received data
  * @param  data_received_size size of data (bytes)
  * @retval None
  */
void I2C_Read(uint8_t device_address, uint8_t register_address, uint8_t *data_received, uint8_t data_received_size);

#endif /* !I2C_H */