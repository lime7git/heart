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
#include "lis3dh_reg.h"
#include "sequencies.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
	
	
static int16_t data_raw_acceleration[3];
static int16_t data_raw_temperature;
static float acceleration_mg[3];
static float temperature_degC;
static uint8_t whoamI;

uint8_t count = 0;
float meanX = 0.0f;
float meanY = 0.0f;
float meanZ = 0.0f;
float total_accelerometer;

float roll_threshold = 1500;
uint8_t shake_detected = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
static int32_t platform_write(void *handle, uint8_t reg, const uint8_t *bufp,
                              uint16_t len);
static int32_t platform_read(void *handle, uint8_t reg, uint8_t *bufp,
                             uint16_t len);
static void platform_delay(uint32_t ms);
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
		
		if(button_pressed_time > 1250) 
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
	

	// ACC INIT
	
	stmdev_ctx_t dev_ctx;
	lis3dh_ctrl_reg3_t ctrl_reg3;
  lis3dh_click_cfg_t click_cfg;
  dev_ctx.write_reg = platform_write;
  dev_ctx.read_reg = platform_read;
  dev_ctx.handle = &hi2c1;

	lis3dh_device_id_get(&dev_ctx, &whoamI);

  if (whoamI != LIS3DH_ID) {
    while (1) {
      /* manage here device not found */
    }
  }
	
	 /* Enable Block Data Update. */
  lis3dh_block_data_update_set(&dev_ctx, PROPERTY_ENABLE);
  /* Set Output Data Rate to 1Hz. */
  lis3dh_data_rate_set(&dev_ctx, LIS3DH_ODR_1Hz);
  /* Set full scale to 2g. */
  lis3dh_full_scale_set(&dev_ctx, LIS3DH_2g);
  /* Enable temperature sensor. */
  lis3dh_aux_adc_set(&dev_ctx, LIS3DH_AUX_ON_TEMPERATURE);
  /* Set device in continuous mode with 12 bit resol. */
  lis3dh_operating_mode_set(&dev_ctx, LIS3DH_HR_12bit);
	
	/* Set click threshold to 12h -> 0.281 g
   * 1 LSB = full scale/128
   *
   * Set TIME_LIMIT to 20h -> 80 ms
   * Set TIME_LATENCY to 20h -> 80 ms
   * Set TIME_WINDOW to 30h -> 120 ms
   * 1 LSB = 1/ODR */
	lis3dh_tap_threshold_set(&dev_ctx, 0x60);
  lis3dh_shock_dur_set(&dev_ctx, 0x20);
  lis3dh_quiet_dur_set(&dev_ctx, 0x20);
  lis3dh_double_tap_timeout_set(&dev_ctx, 0x60);
  /* Enable Click interrupt on INT pin 1 */
  lis3dh_pin_int1_config_get(&dev_ctx, &ctrl_reg3);
  ctrl_reg3.i1_click = PROPERTY_ENABLE;
  lis3dh_pin_int1_config_set(&dev_ctx, &ctrl_reg3);
  lis3dh_int1_gen_duration_set(&dev_ctx, 0);
  /* Enable double click on all axis */
  lis3dh_tap_conf_get(&dev_ctx, &click_cfg);
  click_cfg.xd = PROPERTY_ENABLE;
  click_cfg.yd = PROPERTY_ENABLE;
  click_cfg.zd = PROPERTY_ENABLE;
  lis3dh_tap_conf_set(&dev_ctx, &click_cfg);
	  /* Set Output Data Rate.
   * The recommended accelerometer ODR for single and
   * double-click recognition is 400 Hz or higher. */
  lis3dh_data_rate_set(&dev_ctx, LIS3DH_ODR_400Hz);

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
		//	Set_Sequency(SEQUENCY_5);
		//	Run_Current_Sequency();
		}
		
		if(button_was_pressed == true)
		{
				button_was_pressed = false;
				//Set_Sequency((SEQUENCIES)random_number);
				//Run_Current_Sequency();
		}
		
		
		lis3dh_reg_t reg;
    /* Read output only if new value available */
    lis3dh_xl_data_ready_get(&dev_ctx, &reg.byte);

    if (reg.byte) {
      /* Read accelerometer data */
      memset(data_raw_acceleration, 0x00, 3 * sizeof(int16_t));
      lis3dh_acceleration_raw_get(&dev_ctx, data_raw_acceleration);
      acceleration_mg[0] =
        lis3dh_from_fs2_hr_to_mg(data_raw_acceleration[0]);
      acceleration_mg[1] =
        lis3dh_from_fs2_hr_to_mg(data_raw_acceleration[1]);
      acceleration_mg[2] =
        lis3dh_from_fs2_hr_to_mg(data_raw_acceleration[2]);
    }

    lis3dh_temp_data_ready_get(&dev_ctx, &reg.byte);

    if (reg.byte) {
      /* Read temperature data */
      memset(&data_raw_temperature, 0x00, sizeof(int16_t));
      lis3dh_temperature_raw_get(&dev_ctx, &data_raw_temperature);
      temperature_degC =
        lis3dh_from_lsb_hr_to_celsius(data_raw_temperature);
    }
		
		if(count >= 50)
		{
			meanX /= count;
			meanY /= count;
			meanZ /= count;
			
			total_accelerometer = sqrt((meanX * meanX) + (meanY * meanY) + (meanZ * meanZ));
			
			if(total_accelerometer > roll_threshold) 
			{
				shake_detected = 1;
				
				srand(total_accelerometer);
				random_number = rand() % (10 + 1 - 0) + 0;
				
			//	Set_Sequency((SEQUENCIES)random_number);
			//	if(button_is_pressed) Run_Current_Sequency();
			}
			else 
			{
				shake_detected = 0;
			}
			
			count = 0;		
		}
		else
		{
			meanX += acceleration_mg[0];
			meanY += acceleration_mg[1];
			meanZ += acceleration_mg[2];
			
			count++;
		}
		
		lis3dh_click_src_t src;
    /* Check double tap event */
    lis3dh_tap_source_get(&dev_ctx, &src);

    if (src.dclick) {
			
			if(acceleration_mg[2] > 900) 
			{
				srand(total_accelerometer);
				random_number = rand() % (10 + 1 - 0) + 0;
				
				Set_Sequency((SEQUENCIES)random_number);
				Run_Current_Sequency();
			}
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

static int32_t platform_write(void *handle, uint8_t reg, const uint8_t *bufp, uint16_t len)
{
	reg |= 0x80;
  HAL_I2C_Mem_Write(handle, LIS3DH_I2C_ADD_L, reg, I2C_MEMADD_SIZE_8BIT, (uint8_t*) bufp, len, 1000);
	
	return 0;
}
static int32_t platform_read(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len)
{
	reg |= 0x80;
  HAL_I2C_Mem_Read(handle, LIS3DH_I2C_ADD_L, reg,
                   I2C_MEMADD_SIZE_8BIT, bufp, len, 1000);
	
	return 0;
}
static void platform_delay(uint32_t ms)
{
	HAL_Delay(ms);
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

