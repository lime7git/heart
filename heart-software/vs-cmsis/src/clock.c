/**
 * @file clock.c
 * @author lime7git
 * @date 16.02.2022
 * @brief Clock source file
 */

#include "clock.h" 

volatile static uint32_t milisecond_tick;
volatile static uint32_t delay_tick;

void Clock_Initialization(void)
{
//    MODIFY_REG(RCC->ICSCR, RCC_ICSCR_MSIRANGE, RCC_ICSCR_MSIRANGE_4); // MSI frequency 1.048 MHz

    SystemCoreClockUpdate();	
	SysTick_Config(SystemCoreClock / SYSTICK_FREQUENCY_HZ);	

    milisecond_tick = 0;
    delay_tick = 0;
}

void SysTick_Handler(void)
{
    milisecond_tick++;
    delay_tick++;
}

void Delay_Miliseconds(uint32_t miliseconds)
{
    delay_tick = 0;
    while(delay_tick < miliseconds){}
}

uint32_t Get_Current_Milisecond_Tick(void)
{
    return milisecond_tick;
}
