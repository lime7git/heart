/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "rtc.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "tca6416a.h"
#include "accelerometer.h"
#include "sequencies.h"

#include "stdbool.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
	bool button_is_pressed = false;
	bool button_was_pressed = false;
	uint32_t button_pressed_time = 0;

	int previous_random;
	int random_number = 0;

	sHEART_STATE state = POWER_UP;
	
	reset_cause_t reset_cause;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */
void reset_detection_update(void);
reset_cause_t reset_cause_get(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	state = POWER_UP;
	reset_cause = reset_cause_get();
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_RTC_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */
	PWR_PVDTypeDef PVD_Configuration;
 
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_NVIC_SetPriority(PVD_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(PVD_IRQn);
	
  PVD_Configuration.PVDLevel = PWR_PVDLEVEL_2;
  PVD_Configuration.Mode = PWR_PVD_MODE_IT_RISING;
 
  HAL_PWR_ConfigPVD(&PVD_Configuration);
	HAL_PWR_EnablePVD();
	
	//SystemCoreClockUpdate();
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
		switch(state)
		{
			case NONE:
				
			break;
			
			case FAULT:
				
			break;
			
			case POWER_UP:
				
				TCA6416A_Initialization();
				TCA6416A_Disable_All_LEDs();
				
				Sequency_Initialization();
				Set_Sequency(SEQUENCY_INIT);
				Run_Current_Sequency();

				if(button_is_pressed)
				{
					state = INIT_TOUCH_BUTTON;
				}
				else state = INIT_ACCELEROMETER;
			
			break;
			
			case INIT_ACCELEROMETER:
				
				accelerometer_init();
			
				TCA6416A_Disable_All_LEDs();
				TCA6416A_Write(TCA6416A_OUTPUT_PORT_1_ADDRESS, 254);
				HAL_Delay(500);
				TCA6416A_Disable_All_LEDs();
				HAL_Delay(500);
				TCA6416A_Write(TCA6416A_OUTPUT_PORT_1_ADDRESS, 254);
				HAL_Delay(500);
				TCA6416A_Disable_All_LEDs();
			
				state = IDLE_MODE_ACCELEROMETER;
			
			break;
			
			case INIT_TOUCH_BUTTON:
				
				accelerometer_enter_low_power();
			
				TCA6416A_Disable_All_LEDs();
				TCA6416A_Write(TCA6416A_OUTPUT_PORT_1_ADDRESS, 253);
				HAL_Delay(500);
				TCA6416A_Disable_All_LEDs();
				HAL_Delay(500);
				TCA6416A_Write(TCA6416A_OUTPUT_PORT_1_ADDRESS, 253);
				HAL_Delay(500);
				TCA6416A_Disable_All_LEDs();
			
				button_is_pressed = false;
				button_was_pressed = false;
				button_pressed_time = 0;
				state = IDLE_MODE_TOUCH_BUTTON;
			
			break;
			
			case IDLE_MODE_ACCELEROMETER:
				
				//reset_detection_update();
			
				if(accelerometer_shake_update())
				{
					srand(accelerometer_get_total());
					do{
						random_number = rand() % ((SEQUENCY_LAST - 1) - SEQUENCY_1 + 1) + SEQUENCY_1;
					}while(random_number == previous_random);
					
					previous_random = random_number;
					
					Set_Sequency((SEQUENCIES)random_number);
				}
				
				if(accelerometer_double_tap_update())
				{
					srand(accelerometer_get_total());
					do{
						random_number = rand() % ((SEQUENCY_LAST - 1) - SEQUENCY_1 + 1) + SEQUENCY_1;
					}while(random_number == previous_random);
					
					previous_random = random_number;
					
					Set_Sequency((SEQUENCIES)random_number);
					state = SEQUENCY_RUNNING;
					Run_Current_Sequency();
					if(state != LOW_BATTERY) state = IDLE_MODE_ACCELEROMETER;
				}
		
			break;
		
			case IDLE_MODE_TOUCH_BUTTON:
					
				//reset_detection_update();
			
				if(button_was_pressed)
				{
					srand(button_pressed_time);
					do{
							random_number = rand() % ((SEQUENCY_LAST - 1) - SEQUENCY_1 + 1) + SEQUENCY_1;
					}while(random_number == previous_random);
					
					previous_random = random_number;
					
					Set_Sequency((SEQUENCIES)random_number);
					state = SEQUENCY_RUNNING;
					Run_Current_Sequency();
					if(state != LOW_BATTERY) state = IDLE_MODE_TOUCH_BUTTON;
					
					button_was_pressed = false;
				}
				
			break;
			
			case SEQUENCY_RUNNING:
				
			break;
			
			case SLEEP:
				
			break;
			
			case LOW_BATTERY:
				
				TCA6416A_Disable_All_LEDs();
				TCA6416A_Write(TCA6416A_OUTPUT_PORT_0_ADDRESS, 127);
				HAL_Delay(500);
				TCA6416A_Disable_All_LEDs();
				HAL_Delay(500);
				TCA6416A_Write(TCA6416A_OUTPUT_PORT_0_ADDRESS, 127);
				HAL_Delay(500);
				TCA6416A_Disable_All_LEDs();
				
				HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 0x61A8, RTC_WAKEUPCLOCK_RTCCLK_DIV16);
				
				HAL_SuspendTick();
				
				accelerometer_enter_low_power();
				//HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON,PWR_STOPENTRY_WFI); // w sleep modzie na takiej konfiguracji bylo zuzycie na poziomie 50uA
				HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON,PWR_STOPENTRY_WFI); // 25uA
				
				HAL_ResumeTick();
				HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
				SystemClock_Config();
			
			break;
			
			default: 
				
			break;
		}
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_2;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_RTC;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* PVD_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(PVD_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(PVD_IRQn);
}

/* USER CODE BEGIN 4 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){ //interrupt handler
    
	if(GPIO_Pin == GPIO_PIN_2)
	{ //check interrupt for specific pin                 
    if(!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2))
		{
			button_is_pressed = true;
      button_pressed_time = HAL_GetTick();
    }
		if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2))
		{ 
      button_pressed_time = HAL_GetTick() - button_pressed_time;
			button_is_pressed = false;
			button_was_pressed = true;
    }
  }
	
}

void reset_detection_update(void)
{
	if(button_was_pressed)
	{
		if(button_pressed_time > 10000)
		{
			HAL_NVIC_SystemReset();
		}
	}
}

reset_cause_t reset_cause_get(void)
{
    reset_cause_t reset_cause;

    if (__HAL_RCC_GET_FLAG(RCC_FLAG_LPWRRST))
    {
        reset_cause = RESET_CAUSE_LOW_POWER_RESET;
    }
    else if (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST))
    {
        reset_cause = RESET_CAUSE_WINDOW_WATCHDOG_RESET;
    }
    else if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST))
    {
        reset_cause = RESET_CAUSE_INDEPENDENT_WATCHDOG_RESET;
    }
    else if (__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST))
    {
        // This reset is induced by calling the ARM CMSIS 
        // `NVIC_SystemReset()` function!
        reset_cause = RESET_CAUSE_SOFTWARE_RESET; 
    }
    else if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST))
    {
        reset_cause = RESET_CAUSE_POWER_ON_POWER_DOWN_RESET;
    }
    else if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST))
    {
        reset_cause = RESET_CAUSE_EXTERNAL_RESET_PIN_RESET;
    }
    // Needs to come *after* checking the `RCC_FLAG_PORRST` flag in order to
    // ensure first that the reset cause is NOT a POR/PDR reset. See note
    // below. 
  //  else if (__HAL_RCC_GET_FLAG(RCC_FLAG_))
 //   {
  //      reset_cause = RESET_CAUSE_BROWNOUT_RESET;
  //  }
    else
    {
        reset_cause = RESET_CAUSE_UNKNOWN;
    }

    // Clear all the reset flags or else they will remain set during future
    // resets until system power is fully removed.
    __HAL_RCC_CLEAR_RESET_FLAGS();

    return reset_cause; 
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
