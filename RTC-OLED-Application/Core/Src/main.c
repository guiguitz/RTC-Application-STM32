/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "ssd1306.h"
#include <stdio.h>

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
RTC_HandleTypeDef rtcHandle;
RTC_TimeTypeDef currentTime;
RTC_DateTypeDef currentDate;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void RTC_Config(void);
void RTC_TimeDate_Config(void);

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  char data[30];
  char horario[30];

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
  SSD1306_Init();
  RTC_Config();
  RTC_TimeDate_Config();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  /* USER CODE END WHILE */
		HAL_RTC_GetTime(&rtcHandle, &currentTime, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&rtcHandle, &currentDate, RTC_FORMAT_BIN);

		/* Print current date and time (date time format: yyyy-MM-dd hh:mm:ss)*/
		sprintf(data, "%0*i-%0*i-20%i", 2, currentDate.Date, 2, currentDate.Month, currentDate.Year);
		SSD1306_GotoXY (0,0);
		SSD1306_Puts (data, &Font_11x18, 1);
		SSD1306_UpdateScreen(); // update display

		sprintf(horario, "H:%0*i:%0*i:%0*i", 2, currentTime.Hours, 2, currentTime.Minutes, 2, currentTime.Seconds);
		SSD1306_GotoXY (0,30);
		SSD1306_Puts (horario, &Font_11x18, 1);
		SSD1306_UpdateScreen(); // update display
		HAL_Delay(1000);
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/**
  * @brief  RTC configuration
  * @param  None
  * @retval None
  */
void RTC_Config(void)
{
	/*## STEP 1: Configure RTC ###############################################*/
	rtcHandle.Instance          = RTC;
	rtcHandle.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
	rtcHandle.Init.OutPut       = RTC_OUTPUTSOURCE_NONE;
	if (HAL_RTC_Init(&rtcHandle) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
  * @brief  Time and date configuration
  * @param  None
  * @retval None
  */
void RTC_TimeDate_Config(void)
{
	RTC_TimeTypeDef rtcTimeStruct;
	RTC_DateTypeDef rtcDateStruct;

	/*## STEP 1: Configure time ##############################################*/
	/* Set time: 23:59:55 */
	rtcTimeStruct.Hours   = 0x23;
	rtcTimeStruct.Minutes = 0x59;
	rtcTimeStruct.Seconds = 0x48;
	if (HAL_RTC_SetTime(&rtcHandle, &rtcTimeStruct, RTC_FORMAT_BCD) != HAL_OK)
	{
		Error_Handler();
	}

	/*## STEP 2: Configure date ##############################################*/
	/* Set date: 17/02/2020 */
	rtcDateStruct.Year    = 0x20;
	rtcDateStruct.Month   = RTC_MONTH_FEBRUARY;
	rtcDateStruct.Date    = 0x17;
	rtcDateStruct.WeekDay = RTC_WEEKDAY_MONDAY;
	if(HAL_RTC_SetDate(&rtcHandle, &rtcDateStruct, RTC_FORMAT_BCD) != HAL_OK)
	{
		Error_Handler();
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
