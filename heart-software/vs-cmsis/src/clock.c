#include "clock.h" 

volatile static uint32_t milisecond_tick;

void Clock_Initialization(void)
{
    SystemCoreClockUpdate();

    RCC->ICSCR |= (0x4 << RCC_ICSCR_MSIRANGE_Pos);  // MSI frequency around 1.048 MHz

    SystemCoreClockUpdate();		
	SysTick_Config(SystemCoreClock / SYSTICK_FREQUENCY_HZ);	
}

void SysTick_Handler(void)
{
    milisecond_tick++;
}

void Delay_Miliseconds(uint32_t miliseconds)
{
    milisecond_tick = 0;
    while(milisecond_tick < miliseconds){}
}