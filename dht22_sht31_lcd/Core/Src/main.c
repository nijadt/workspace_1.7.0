/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lib_lcd.h"
#include "DHT22.h"
#include "STH31.h"
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
rgb_lcd DataStruct;

 SHT31_data_t data;
 uint8_t Data_T[256];
 uint8_t Data_H[256];



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

	//HAL_NVIC_DisableIRQ(DHT22_SENS_EXTI_IRQn);



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
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  MX_TIM7_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */

  	  /* INIT LCD */
 	  rgb_lcd LCD_DataStruct;
 	  LCD_DataStruct._displaycontrol = LCD_DISPLAYON;
 	  LCD_DataStruct._displayfunction = LCD_2LINE;
 	  LCD_DataStruct._displaymode = LCD_ENTRYLEFT;


 	  lcd_init(&hi2c1, &LCD_DataStruct);

 	  reglagecouleur(255,0,0);

 	  /* INIT DHT22 */
 	  DHT22_Init(&htim7, 32e6,DHT22_SENS_GPIO_Port,DHT22_SENS_Pin, DHT22_SENS_EXTI_IRQn);

 	  /* TEST : Print measures on the LCD screen */
 	  DHT22_Data DHT22_DataStruct;

	 	char text_buff7[32];
	 	sprintf(text_buff7,"  STM32 - BE");
	 	lcd_position(&hi2c1,0,0);
	 	lcd_print(&hi2c1, text_buff7);

	 	char text_buff8[32];
		sprintf(text_buff8, "DHT22/RTC/SHT31");
		lcd_position(&hi2c1,0,1);
		lcd_print(&hi2c1, text_buff8);
		HAL_Delay(1500);

	 	lcd_init(&hi2c1, &LCD_DataStruct);

	 	  //HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	 	  //HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

	 	  //HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	 	  //HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

	 		//char text_time[32];
	 	 	//sprintf(text_time,"%02d:%02d:%02d",sTime.Hours, sTime.Minutes, sTime.Seconds);
	 	 	//lcd_position(&hi2c1,0,0);
	 	 	//lcd_print(&hi2c1, text_time);

	 	 	//char text_date[32];
	 	 	//sprintf(text_date,"%02d-%02d-%02d",sDate.Date, sDate.Month, 2000+sDate.Year);
	 	 	//lcd_position(&hi2c1,0,1);
	 	 	//lcd_print(&hi2c1, text_date);

	 	 	//lcd_init(&hi2c1, &LCD_DataStruct);
	 	//HAL_Delay(2500);



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  reglagecouleur(125,0,0);

	  	HAL_Delay(500);


	  DHT22_DataStruct = DHT22_ReadData(); // lecture de la temperature et l'humidite dht22
	  data=SHT31_readData(&hi2c1); // lecture de la temperature et l'humidite sth31

	 	char text_buff1[32];
	 	sprintf(text_buff1,"T_dht22 : %.1f C",DHT22_DataStruct.T);
	 	lcd_position(&hi2c1,0,0);
	 	lcd_print(&hi2c1, text_buff1);


	 	char text_buff2[32];
		sprintf(text_buff2, "T_sht31 : %ld.%ld C ",(uint32_t )(data.T),(uint32_t )((data.T-(uint32_t )(data.T))*10));
		lcd_position(&hi2c1,0,1);
		lcd_print(&hi2c1, text_buff2);

		reglagecouleur(200,200,255);

		HAL_Delay(3000);

		char text_buff3[32];
		sprintf(text_buff3,"H_dht22 : %.1f %%",DHT22_DataStruct.RH);
		lcd_position(&hi2c1,0,0);
		lcd_print(&hi2c1, text_buff3);


		char text_buff4[32];
		sprintf(text_buff4, "H_sht31 : %ld.%ld %% ",(uint32_t )(data.H),(uint32_t )((data.H-(uint32_t )(data.H))*10));
		lcd_position(&hi2c1,0,1);
		lcd_print(&hi2c1, text_buff4);

		reglagecouleur(200,200,255);

		HAL_Delay(3000);


		float delta_T = DHT22_DataStruct.T - data.T;
		float delta_H = DHT22_DataStruct.RH - data.H;

		char text_buff5[32];
		sprintf(text_buff5,"Delta_T =%.2f C ",delta_T);
		lcd_position(&hi2c1,0,0);
		lcd_print(&hi2c1, text_buff5);


		char text_buff6[32];
		sprintf(text_buff6, "Delta_H =%.2f ",delta_H);
		lcd_position(&hi2c1,0,1);
		lcd_print(&hi2c1, text_buff6);

		reglagecouleur(200,200,255);
		HAL_Delay(3000);

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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLL_DIV3;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
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
