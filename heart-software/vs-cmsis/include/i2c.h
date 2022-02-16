/**
 * @file i2c.h
 * @author lime7git
 * @date 16.02.2022
 * @brief I2C bus header file
 */

/* File i2c  */
#ifndef I2C_H
#define I2C_H

#include "stm32l031xx.h"
#include "stm32l0xx.h"

void I2C_Initialization(void);
void GPIO_Alternative_Function_Initialization(void);

#endif /* !I2C_H */