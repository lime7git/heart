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
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define TCA6416A_ADDRESS 0x20

#define TCA6416A_READ_ADDRESS 0x41
#define TCA6416A_WRITE_ADDRESS 0x40

#define TCA6416A_INPUT_PORT_0_ADDRESS 0x00
#define TCA6416A_INPUT_PORT_1_ADDRESS 0x01

#define TCA6416A_OUTPUT_PORT_0_ADDRESS 0x02
#define TCA6416A_OUTPUT_PORT_1_ADDRESS 0x03

#define TCA6416A_POLARITY_PORT_0_ADDRESS 0x04
#define TCA6416A_POLARITY_PORT_1_ADDRESS 0x05

#define TCA6416A_CONFIGURATION_0_ADDRESS 0x06
#define TCA6416A_CONFIGURATION_1_ADDRESS 0x07

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
	volatile uint8_t buffer = 0;
	volatile uint8_t mode = 0;
	volatile uint8_t is_sequence_running = 0;
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
  /* USER CODE BEGIN 2 */
	buffer = 0x0;
	HAL_I2C_Mem_Write(&hi2c1, TCA6416A_WRITE_ADDRESS, TCA6416A_CONFIGURATION_0_ADDRESS, 1, (uint8_t *)&buffer, sizeof(buffer), HAL_MAX_DELAY);
	HAL_I2C_Mem_Write(&hi2c1, TCA6416A_WRITE_ADDRESS, TCA6416A_CONFIGURATION_1_ADDRESS, 1, (uint8_t *)&buffer, sizeof(buffer), HAL_MAX_DELAY);
			
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	while(!is_sequence_running)
	{
		if(HAL_GPIO_ReadPin(TOUCH_KEY_GPIO_Port,TOUCH_KEY_Pin) == RESET)
		{
			mode++;
			is_sequence_running = 1;
			
			if(mode > 5) mode = 0;
			
			HAL_Delay(250);
		}
	}

	
	while(is_sequence_running)
	{
		if(mode == 0)
		{
			
				buffer = 0xFF;
					HAL_I2C_Mem_Write(&hi2c1, TCA6416A_WRITE_ADDRESS, TCA6416A_OUTPUT_PORT_0_ADDRESS, 1, (uint8_t *)&buffer, sizeof(buffer), HAL_MAX_DELAY);
					HAL_I2C_Mem_Write(&hi2c1, TCA6416A_WRITE_ADDRESS, TCA6416A_OUTPUT_PORT_1_ADDRESS, 1, (uint8_t *)&buffer, sizeof(buffer), HAL_MAX_DELAY);
		}
		else if(mode == 1)
		{
				for(uint8_t i = 0; i < 8; i++)
				{
					buffer = 0xFF & ~(1 << i);
					HAL_I2C_Mem_Write(&hi2c1, TCA6416A_WRITE_ADDRESS, TCA6416A_OUTPUT_PORT_1_ADDRESS, 1, (uint8_t *)&buffer, sizeof(buffer), HAL_MAX_DELAY);
					HAL_Delay(25);
				}
				buffer = 0xFF;
					HAL_I2C_Mem_Write(&hi2c1, TCA6416A_WRITE_ADDRESS, TCA6416A_OUTPUT_PORT_1_ADDRESS, 1, (uint8_t *)&buffer, sizeof(buffer), HAL_MAX_DELAY);
				for(uint8_t i = 0; i < 8; i++)
				{
					buffer = 0xFF & ~(1 << i);
					HAL_I2C_Mem_Write(&hi2c1, TCA6416A_WRITE_ADDRESS, TCA6416A_OUTPUT_PORT_0_ADDRESS, 1, (uint8_t *)&buffer, sizeof(buffer), HAL_MAX_DELAY);
					HAL_Delay(25);
				}
				buffer = 0xFF;
					HAL_I2C_Mem_Write(&hi2c1, TCA6416A_WRITE_ADDRESS, TCA6416A_OUTPUT_PORT_0_ADDRESS, 1, (uint8_t *)&buffer, sizeof(buffer), HAL_MAX_DELAY);
		}
		else if(mode == 2)
		{
				for(int i = 7; i >= 0; i--)
				{
					buffer = 0xFF & ~(1 << i);
					HAL_I2C_Mem_Write(&hi2c1, TCA6416A_WRITE_ADDRESS, TCA6416A_OUTPUT_PORT_0_ADDRESS, 1, (uint8_t *)&buffer, sizeof(buffer), HAL_MAX_DELAY);
					HAL_Delay(25);
				}
				buffer = 0xFF;
					HAL_I2C_Mem_Write(&hi2c1, TCA6416A_WRITE_ADDRESS, TCA6416A_OUTPUT_PORT_0_ADDRESS, 1, (uint8_t *)&buffer, sizeof(buffer), HAL_MAX_DELAY);
				
				for(int i = 7; i >= 0; i--)
				{
					buffer = 0xFF & ~(1 << i);
					HAL_I2C_Mem_Write(&hi2c1, TCA6416A_WRITE_ADDRESS, TCA6416A_OUTPUT_PORT_1_ADDRESS, 1, (uint8_t *)&buffer, sizeof(buffer), HAL_MAX_DELAY);
					HAL_Delay(25);
				}
				buffer = 0xFF;
					HAL_I2C_Mem_Write(&hi2c1, TCA6416A_WRITE_ADDRESS, TCA6416A_OUTPUT_PORT_1_ADDRESS, 1, (uint8_t *)&buffer, sizeof(buffer), HAL_MAX_DELAY);
		}
		if(mode == 3)
		{
				buffer = 0x0;
					HAL_I2C_Mem_Write(&hi2c1, TCA6416A_WRITE_ADDRESS, TCA6416A_OUTPUT_PORT_0_ADDRESS, 1, (uint8_t *)&buffer, sizeof(buffer), HAL_MAX_DELAY);
					HAL_I2C_Mem_Write(&hi2c1, TCA6416A_WRITE_ADDRESS, TCA6416A_OUTPUT_PORT_1_ADDRESS, 1, (uint8_t *)&buffer, sizeof(buffer), HAL_MAX_DELAY);
		}
		if(mode == 4)
		{
				for(uint8_t i = 0; i < 8; i++)
				{
					buffer = 0xFF & ~(1 << i);
					HAL_I2C_Mem_Write(&hi2c1, TCA6416A_WRITE_ADDRESS, TCA6416A_OUTPUT_PORT_0_ADDRESS, 1, (uint8_t *)&buffer, sizeof(buffer), HAL_MAX_DELAY);
					HAL_I2C_Mem_Write(&hi2c1, TCA6416A_WRITE_ADDRESS, TCA6416A_OUTPUT_PORT_1_ADDRESS, 1, (uint8_t *)&buffer, sizeof(buffer), HAL_MAX_DELAY);
					HAL_Delay(75);
				}
				buffer = 0xFF;
					HAL_I2C_Mem_Write(&hi2c1, TCA6416A_WRITE_ADDRESS, TCA6416A_OUTPUT_PORT_0_ADDRESS, 1, (uint8_t *)&buffer, sizeof(buffer), HAL_MAX_DELAY);
					HAL_I2C_Mem_Write(&hi2c1, TCA6416A_WRITE_ADDRESS, TCA6416A_OUTPUT_PORT_1_ADDRESS, 1, (uint8_t *)&buffer, sizeof(buffer), HAL_MAX_DELAY);
				for(uint8_t i = 0; i < 8; i++)
				{
					buffer = 0xFF & ~(1 << i);
					HAL_I2C_Mem_Write(&hi2c1, TCA6416A_WRITE_ADDRESS, TCA6416A_OUTPUT_PORT_0_ADDRESS, 1, (uint8_t *)&buffer, sizeof(buffer), HAL_MAX_DELAY);
					HAL_I2C_Mem_Write(&hi2c1, TCA6416A_WRITE_ADDRESS, TCA6416A_OUTPUT_PORT_1_ADDRESS, 1, (uint8_t *)&buffer, sizeof(buffer), HAL_MAX_DELAY);
					HAL_Delay(75);
				}
				buffer = 0xFF;
					HAL_I2C_Mem_Write(&hi2c1, TCA6416A_WRITE_ADDRESS, TCA6416A_OUTPUT_PORT_0_ADDRESS, 1, (uint8_t *)&buffer, sizeof(buffer), HAL_MAX_DELAY);
					HAL_I2C_Mem_Write(&hi2c1, TCA6416A_WRITE_ADDRESS, TCA6416A_OUTPUT_PORT_1_ADDRESS, 1, (uint8_t *)&buffer, sizeof(buffer), HAL_MAX_DELAY);
				for(int i = 7; i >= 0; i--)
				{
					buffer = 0xFF & ~(1 << i);
					HAL_I2C_Mem_Write(&hi2c1, TCA6416A_WRITE_ADDRESS, TCA6416A_OUTPUT_PORT_0_ADDRESS, 1, (uint8_t *)&buffer, sizeof(buffer), HAL_MAX_DELAY);
					HAL_I2C_Mem_Write(&hi2c1, TCA6416A_WRITE_ADDRESS, TCA6416A_OUTPUT_PORT_1_ADDRESS, 1, (uint8_t *)&buffer, sizeof(buffer), HAL_MAX_DELAY);
					HAL_Delay(25);
				}
				buffer = 0xFF;
					HAL_I2C_Mem_Write(&hi2c1, TCA6416A_WRITE_ADDRESS, TCA6416A_OUTPUT_PORT_0_ADDRESS, 1, (uint8_t *)&buffer, sizeof(buffer), HAL_MAX_DELAY);
					HAL_I2C_Mem_Write(&hi2c1, TCA6416A_WRITE_ADDRESS, TCA6416A_OUTPUT_PORT_1_ADDRESS, 1, (uint8_t *)&buffer, sizeof(buffer), HAL_MAX_DELAY);
				for(int i = 7; i >= 0; i--)
				{
					buffer = 0xFF & ~(1 << i);
					HAL_I2C_Mem_Write(&hi2c1, TCA6416A_WRITE_ADDRESS, TCA6416A_OUTPUT_PORT_0_ADDRESS, 1, (uint8_t *)&buffer, sizeof(buffer), HAL_MAX_DELAY);
					HAL_I2C_Mem_Write(&hi2c1, TCA6416A_WRITE_ADDRESS, TCA6416A_OUTPUT_PORT_1_ADDRESS, 1, (uint8_t *)&buffer, sizeof(buffer), HAL_MAX_DELAY);
					HAL_Delay(25);
				}
				buffer = 0xFF;
					HAL_I2C_Mem_Write(&hi2c1, TCA6416A_WRITE_ADDRESS, TCA6416A_OUTPUT_PORT_0_ADDRESS, 1, (uint8_t *)&buffer, sizeof(buffer), HAL_MAX_DELAY);
					HAL_I2C_Mem_Write(&hi2c1, TCA6416A_WRITE_ADDRESS, TCA6416A_OUTPUT_PORT_1_ADDRESS, 1, (uint8_t *)&buffer, sizeof(buffer), HAL_MAX_DELAY);
		}
		if(mode == 5)
		{
			for(int i = 0; i < 3; i++)
			{			
				buffer = 0x0;
					HAL_I2C_Mem_Write(&hi2c1, TCA6416A_WRITE_ADDRESS, TCA6416A_OUTPUT_PORT_0_ADDRESS, 1, (uint8_t *)&buffer, sizeof(buffer), HAL_MAX_DELAY);
					HAL_I2C_Mem_Write(&hi2c1, TCA6416A_WRITE_ADDRESS, TCA6416A_OUTPUT_PORT_1_ADDRESS, 1, (uint8_t *)&buffer, sizeof(buffer), HAL_MAX_DELAY);
				
				HAL_Delay(500);
				
				buffer = 0xFF;
					HAL_I2C_Mem_Write(&hi2c1, TCA6416A_WRITE_ADDRESS, TCA6416A_OUTPUT_PORT_0_ADDRESS, 1, (uint8_t *)&buffer, sizeof(buffer), HAL_MAX_DELAY);
					HAL_I2C_Mem_Write(&hi2c1, TCA6416A_WRITE_ADDRESS, TCA6416A_OUTPUT_PORT_1_ADDRESS, 1, (uint8_t *)&buffer, sizeof(buffer), HAL_MAX_DELAY);
				
				HAL_Delay(500);
			}	
				
				
				mode = 0;
		}
		
		is_sequence_running = 0;
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_3;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
