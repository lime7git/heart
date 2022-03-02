/**
 * @file i2c.c
 * @author lime7git
 * @date 16.02.2022
 * @brief i2c source file
 */

#include "i2c.h"

void I2C_Initialization(void)
{
    GPIO_Alternative_Function_Initialization();

	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN; // I2C1 clock enable

    I2C1->CR1 &= ~I2C_CR1_PE;   
    I2C1->TIMINGR = (uint32_t)0x00000708; // timing configuration - value taken from stm32cubemx
    I2C1->CR1 |= I2C_CR1_PE;
}

void GPIO_Alternative_Function_Initialization(void)
{
    RCC->IOPENR |= RCC_IOPENR_GPIOAEN;  													// I/O port A clock enable

	MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODE9, 0x2 << GPIO_MODER_MODE9_Pos);				// PA9 - Alternate function mode
	MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODE10, 0x2 << GPIO_MODER_MODE10_Pos);				// PA10 - Alternate function mode

	MODIFY_REG(GPIOA->OSPEEDR, GPIO_OSPEEDER_OSPEED9, 0x3 << GPIO_OSPEEDER_OSPEED9_Pos);	// PA9 - Very high speed
	MODIFY_REG(GPIOA->OSPEEDR, GPIO_OSPEEDER_OSPEED10, 0x3 << GPIO_OSPEEDER_OSPEED10_Pos);	// PA10 - Very high speed

	MODIFY_REG(GPIOA->PUPDR, GPIO_PUPDR_PUPD9, 0x1 << GPIO_PUPDR_PUPD9_Pos);				// PA9 - Pull up
	MODIFY_REG(GPIOA->PUPDR, GPIO_PUPDR_PUPD10, 0x1 << GPIO_PUPDR_PUPD10_Pos);				// PA10 - Pull up
  		 
	MODIFY_REG(GPIOA->AFR[1], (GPIO_AFRH_AFSEL9 | GPIO_AFRH_AFSEL10), ((0x1 << GPIO_AFRH_AFSEL10_Pos) | (0x1 << GPIO_AFRH_AFSEL9_Pos))); // PA9 - I2C1_SCL | PA10 - I2C1_SDA
}

void I2C_Write(uint8_t device_address, uint8_t register_address, uint8_t *data_to_transfer, uint8_t data_to_transfer_size)
{
    I2C1->CR2 = (device_address << I2C_CR2_SADD_Pos) | I2C_TRANSFER_DIRECTION_WIRTE | ((0x1 + data_to_transfer_size) << I2C_CR2_NBYTES_Pos) | I2C_CR2_AUTOEND | I2C_CR2_START;

	while((I2C1->ISR & I2C_ISR_TXIS) == RESET){}
	I2C1->TXDR = register_address;
	
	while((I2C1->ISR & I2C_ISR_TXIS) == RESET){}
	I2C1->TXDR = *data_to_transfer;	
}

void I2C_Read(uint8_t device_address, uint8_t register_address, uint8_t *data_received, uint8_t data_received_size)
{
    I2C1->CR2 |= (device_address << I2C_CR2_SADD_Pos) | I2C_TRANSFER_DIRECTION_WIRTE | (0x1 << I2C_CR2_NBYTES_Pos) | I2C_CR2_AUTOEND;
	I2C1->CR2 |= I2C_CR2_START;

	while((I2C1->ISR & I2C_ISR_TXIS) == RESET){}
	I2C1->TXDR = register_address;

    I2C1->CR2 |= (device_address << I2C_CR2_SADD_Pos) | I2C_TRANSFER_DIRECTION_READ | (data_received_size << I2C_CR2_NBYTES_Pos) | I2C_CR2_AUTOEND;
	I2C1->CR2 |= I2C_CR2_START;
	
	while((I2C1->ISR & I2C_ISR_RXNE) == RESET){}
	*data_received = I2C1->RXDR;	
}