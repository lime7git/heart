#include "i2c.h"

void I2C_Initialization(void)
{
    GPIO_Alternative_Function_Initialization();

    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN; // I2C1 clock enable

    I2C1->CR1 &= ~I2C_CR1_PE;   
    I2C1->TIMINGR = 0x00000202; // timing configuration - value taken from stm32cubemx
    I2C1->CR1 |= I2C_CR1_PE;
}

void GPIO_Alternative_Function_Initialization(void)
{
    RCC->IOPENR |= RCC_IOPENR_GPIOAEN;  // I/O port A clock enable
	
	GPIOA->MODER |= (0x2 << GPIO_MODER_MODE10_Pos) | (0x2 << GPIO_MODER_MODE9_Pos);     // PA9 | PA10 - Alternate function mode
	GPIOA->AFR[1] |= (0x1 << GPIO_AFRH_AFSEL10_Pos) | (0x1 << GPIO_AFRH_AFSEL9_Pos);    // PA9 - I2C1_SCL | PA10 - I2C1_SDA
}