/**
 * @file clock.h
 * @author lime7git
 * @date 16.02.2022
 * @brief Clock frequency configuration header file
 */

/* File clock  */
#ifndef CLOCK_H
#define CLOCK_H

#include "stm32l031xx.h"
#include "stm32l0xx.h"

#define SYSTICK_FREQUENCY_HZ  1000 // 1KHz systick frequency

void Clock_Initialization(void);
void SysTick_Handler(void);             // SysTick interrupt handler
void Delay_Miliseconds(uint32_t ms);

#endif /* !CLOCK_H */