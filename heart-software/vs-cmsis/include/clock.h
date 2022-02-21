/**
 * @file clock.h
 * @author lime7git
 * @date 16.02.2022
 * @brief Clock frequency configuration header file
 */

/* File clock  */
#ifndef _CLOCK_H
#define _CLOCK_H

#include "stm32l031xx.h"
#include "stm32l0xx.h"

#define SYSTICK_FREQUENCY_HZ  1000 // 1KHz systick frequency


/**
  * @brief  Initializes clock setup and SysTick config
  * @param  None
  * @retval None
  */
void Clock_Initialization(void);

/**
  * @brief  SysTick interrupt handler
  * @param  None
  * @retval None
  */
void SysTick_Handler(void);

/**
  * @brief  Milisecond delay function
  * @param  miliseconds Time in miliseconds that function will wait
  * @retval None
  */
void Delay_Miliseconds(uint32_t miliseconds);

/**
  * @param  None
  * @retval Number of ticks (miliseconds) counted from power up
  */
uint32_t Get_Current_Milisecond_Tick(void);

#endif /* !CLOCK_H */