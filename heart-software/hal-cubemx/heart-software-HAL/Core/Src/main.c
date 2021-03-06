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
#include "lis3dh.h"
#include "sequencies.h"

#include <stdio.h>
#include <stdlib.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum HEART_STATE{
	NONE,
	FAULT,
	INIT,
	INIT_WITH_RTC,
	INIT_WITHOUT_RTC,
	IDLE,
	SEQUENCY_RUNNING,
	SLEEP,
	
} sHEART_STATE;
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

	int random_number = 0;

	sHEART_STATE STATE;
	
	RTC_TimeTypeDef rtc_time;
	RTC_DateTypeDef rtc_date;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

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
	STATE = INIT;
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
  /* USER CODE BEGIN 2 */
	STATE = INIT;
	
	SystemCoreClockUpdate();
	
	TCA6416A_Initialization();
	TCA6416A_Disable_All_LEDs();
			
	Sequency_Initialization();
			
	Set_Sequency(SEQUENCY_INIT);
	Run_Current_Sequency();	
	
	STATE = INIT_WITHOUT_RTC;
		
	if(button_was_pressed == true)
	{
		button_was_pressed = false;
		
		if(button_pressed_time > 1500) 
		{
			STATE = INIT_WITH_RTC;
			
			HAL_RTC_GetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN);
			HAL_RTC_GetDate(&hrtc, &rtc_date, RTC_FORMAT_BIN);
			
			uint8_t tmp_value = 1;
			uint8_t value_counter = 0;
			
			
			TCA6416A_Disable_All_LEDs();
			TCA6416A_Write(TCA6416A_OUTPUT_PORT_0_ADDRESS, 254);
			
			while(STATE == INIT_WITH_RTC)
			{
				if(button_was_pressed == true)
				{
					button_was_pressed = false;
					
					if(button_pressed_time < 500) 
					{
						tmp_value++;
						
						if(value_counter == 0 && tmp_value == 32) tmp_value = 1;
						else if(value_counter == 1 && tmp_value == 13) tmp_value = 1;
						else if(value_counter == 2 && tmp_value == 100) tmp_value = 1;
						else if(value_counter == 3 && tmp_value == 8) tmp_value = 1;
						else if(value_counter == 4 && tmp_value == 24) tmp_value = 0;
						else if(value_counter == 5 && tmp_value == 61) tmp_value = 0;
						
						
						TCA6416A_Write(TCA6416A_OUTPUT_PORT_0_ADDRESS, 255 - tmp_value);
					}
					else if(button_pressed_time > 2000) 
					{
						if(value_counter == 0){	rtc_date.Date = tmp_value; tmp_value = 1; }
						else if(value_counter == 1){	rtc_date.Month = tmp_value; tmp_value = 1; }
						else if(value_counter == 2){	rtc_date.Year = tmp_value; tmp_value = 1; }
						else if(value_counter == 3){	rtc_date.WeekDay = tmp_value; tmp_value = 0; }
						else if(value_counter == 4){	rtc_time.Hours = tmp_value; tmp_value = 0; }
						else if(value_counter == 5)	
						{
							rtc_time.Minutes = tmp_value;
							
							rtc_time.Seconds = 0;
							rtc_time.SubSeconds = 0;
							HAL_RTC_SetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN);
							HAL_RTC_SetDate(&hrtc, &rtc_date, RTC_FORMAT_BIN);
							STATE = IDLE;
							TCA6416A_Enable_All_LEDs();
							HAL_Delay(500);
							TCA6416A_Disable_All_LEDs();
							break;
						}
						
						TCA6416A_Enable_All_LEDs();
						HAL_Delay(500);
						TCA6416A_Disable_All_LEDs();
						TCA6416A_Write(TCA6416A_OUTPUT_PORT_0_ADDRESS, 255 - tmp_value);
						
						
						value_counter++;
					}
				}
			}
			
		}
	}
	




  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
		HAL_RTC_GetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &rtc_date, RTC_FORMAT_BIN);
		
		if(rtc_time.Minutes == 1 && rtc_time.Seconds == 7 && rtc_date.Date == 13)
		{
			Set_Sequency(SEQUENCY_5);
			Run_Current_Sequency();
		}
		
		if(button_was_pressed == true)
		{
				button_was_pressed = false;
				Set_Sequency((SEQUENCIES)random_number);
				Run_Current_Sequency();
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
			
			srand(button_pressed_time);
				
			int random_tmp;
			random_tmp = rand() % (10 + 1 - 0) + 0;
				
			while(random_number == random_tmp)
			{
				random_tmp = rand() % (10 + 1 - 0) + 0;
			}
				
				random_number = random_tmp;
				
			button_was_pressed = true;
    }
  }
	
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

