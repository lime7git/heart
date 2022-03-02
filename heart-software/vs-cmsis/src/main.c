/**
 * @file main.c
 * @author lime7git
 * @date 16.02.2022
 * @brief Main source file
 */

#include "main.h"

int main(void)
{
    Clock_Initialization();
    I2C_Initialization();

		I2C1->CR2 = 0x0;
		I2C1->CR2 = (0x40 << I2C_CR2_SADD_Pos) | (2 << I2C_CR2_NBYTES_Pos) | I2C_CR2_AUTOEND | I2C_CR2_START;

		while((I2C1->ISR & I2C_ISR_TXIS) == RESET){}

		if((I2C1->ISR & I2C_ISR_TXE) == (I2C_ISR_TXE))
		{
		I2C1->TXDR = 0x07;
		}
		
		while((I2C1->ISR & I2C_ISR_TXIS) == RESET){}

		if((I2C1->ISR & I2C_ISR_TXE) == (I2C_ISR_TXE))
		{
		I2C1->TXDR = 0x0;
		}

		Delay_Miliseconds(10);

		I2C1->CR2 = 0x0;
		I2C1->CR2 = (0x40 << I2C_CR2_SADD_Pos) | (2 << I2C_CR2_NBYTES_Pos) | I2C_CR2_AUTOEND | I2C_CR2_START;

		while((I2C1->ISR & I2C_ISR_TXIS) == RESET){}

		if((I2C1->ISR & I2C_ISR_TXE) == (I2C_ISR_TXE))
		{
		I2C1->TXDR = 0x03;
		}

		while((I2C1->ISR & I2C_ISR_TXIS) == RESET){}

		if((I2C1->ISR & I2C_ISR_TXE) == (I2C_ISR_TXE))
		{
		I2C1->TXDR = 0xFF;
		}

    while(true)
    {



    }
}
