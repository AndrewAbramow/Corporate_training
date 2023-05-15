/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
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
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
void Timer1_Interrupt();
void Timer2_Interrupt();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint32_t timerCount = 0;
uint16_t timer2Count = 0;

unsigned char blinkFlag = 0;

// step flags
unsigned char sFlags[8] = {0,0,0,0,0,0,0,0};

// time for each step
uint32_t timings[8] = {1000,3000,6000,8000,9000,11000,14000,16000};

// step change timer
void Timer1_Interrupt()
{
	if (timerCount == timings[7])
	{
		sFlags[7] = 1;
		timerCount = 0;
	}
	else if (timerCount == timings[6])
	{
		sFlags[6] = 1;
	}
	else if (timerCount == timings[5])
	{
		sFlags[5] = 1;
	}
	else if (timerCount == timings[4])
	{
		sFlags[4] = 1;
	}
	else if (timerCount == timings[3])
	{
		sFlags[3] = 1;
	}
	else if (timerCount == timings[2])
	{
		sFlags[2] = 1;
	}
	else if (timerCount == timings[1])
	{
		sFlags[1] = 1;
	}
	else if(timerCount == timings[0])
	{
		sFlags[0] = 1;
	}
	++timerCount;
}

// led blink timer
void Timer2_Interrupt()
{
	if (timer2Count == 500)
	{
		blinkFlag = 1;
		timer2Count = 0;
	}
	++timer2Count;
}

void Blink(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	if ((blinkFlag == 1 && sFlags[1] == 1) ||
		(blinkFlag == 1 && sFlags[5] == 1)	)
		{
			HAL_GPIO_TogglePin(GPIOx, GPIO_Pin);
		  	blinkFlag = 0;
		}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	for(int i=0; i<255; i++)
	{
		for(int j=0; j<50; j++)
		{
			  asm ("nop""\n\tnop""\n\tnop""\n\tnop""\n\tnop""\n\tnop");
			  asm ("nop""\n\tnop""\n\tnop""\n\tnop""\n\tnop""\n\tnop");
			  asm ("nop""\n\tnop""\n\tnop""\n\tnop""\n\tnop""\n\tnop");
			  asm ("nop""\n\tnop""\n\tnop""\n\tnop""\n\tnop""\n\tnop");
			  asm ("nop""\n\tnop""\n\tnop""\n\tnop""\n\tnop""\n\tnop");
			  asm ("nop""\n\tnop""\n\tnop""\n\tnop""\n\tnop""\n\tnop");
		}
	}
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
  MX_TIM1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim1);
  HAL_TIM_Base_Start_IT(&htim2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  // 2 step
	  if(sFlags[0] == 1)
	  {
		  sFlags[0] = 0;
		  HAL_GPIO_WritePin(GPIOB, RED1_Pin, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOB, RED2_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOB, YELLOW1_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOB, YELLOW2_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOA, GREEN1_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOA, GREEN2_Pin, GPIO_PIN_SET);
	  }
	  // 3 step
	  if(sFlags[1] == 1)
	  {
		  //sFlags[1] = 0;
		  HAL_GPIO_WritePin(GPIOB, RED1_Pin, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOB, RED2_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOB, YELLOW1_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOB, YELLOW2_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOA, GREEN1_Pin, GPIO_PIN_RESET);
		  Blink(GPIOA, GREEN2_Pin);
		  //HAL_GPIO_WritePin(GPIOA, GREEN2_Pin, GPIO_PIN_SET);
	  }
	  // 4 step
	  if(sFlags[2] == 1)
	  {
		  sFlags[1] = 0;
		  sFlags[2] = 0;
		  HAL_GPIO_WritePin(GPIOB, RED1_Pin, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOB, RED2_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOB, YELLOW1_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOB, YELLOW2_Pin, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOA, GREEN1_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOA, GREEN2_Pin, GPIO_PIN_RESET);
	  }
	  // 5 step
	  if(sFlags[3] == 1)
	  {
		  sFlags[3] = 0;
		  HAL_GPIO_WritePin(GPIOB, RED1_Pin, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOB, RED2_Pin, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOB, YELLOW1_Pin, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOB, YELLOW2_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOA, GREEN1_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOA, GREEN2_Pin, GPIO_PIN_RESET);
	  }
	  // 6 step
	  if(sFlags[4] == 1)
	  {
		  sFlags[4] = 0;
		  HAL_GPIO_WritePin(GPIOB, RED1_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOB, RED2_Pin, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOB, YELLOW1_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOB, YELLOW2_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOA, GREEN1_Pin, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOA, GREEN2_Pin, GPIO_PIN_RESET);
	  }
	  // 7 step
	  if(sFlags[5] == 1)
	  {
		  //sFlags[5] = 0;
		  HAL_GPIO_WritePin(GPIOB, RED1_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOB, RED2_Pin, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOB, YELLOW1_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOB, YELLOW2_Pin, GPIO_PIN_RESET);
		  //HAL_GPIO_WritePin(GPIOA, GREEN1_Pin, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOA, GREEN2_Pin, GPIO_PIN_RESET);
		  Blink(GPIOA, GREEN1_Pin);

	  }
	  // 8 step
	  if(sFlags[6] == 1)
	  {
		  sFlags[5] = 0;
		  sFlags[6] = 0;
		  HAL_GPIO_WritePin(GPIOB, RED1_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOB, RED2_Pin, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOB, YELLOW1_Pin, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOB, YELLOW2_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOA, GREEN1_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOA, GREEN2_Pin, GPIO_PIN_RESET);
	  }
	  // 9 (1) step
	  if(sFlags[7] == 1)
	  {
		  sFlags[7] = 0;
		  HAL_GPIO_WritePin(GPIOB, RED1_Pin, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOB, RED2_Pin, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOB, YELLOW1_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOB, YELLOW2_Pin, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOA, GREEN1_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(GPIOA, GREEN2_Pin, GPIO_PIN_RESET);
	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
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
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 35999;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 1;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 35999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, RED1_Pin|RED2_Pin|YELLOW1_Pin|YELLOW2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GREEN1_Pin|GREEN2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : RED1_Pin RED2_Pin YELLOW1_Pin YELLOW2_Pin */
  GPIO_InitStruct.Pin = RED1_Pin|RED2_Pin|YELLOW1_Pin|YELLOW2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : GREEN1_Pin GREEN2_Pin */
  GPIO_InitStruct.Pin = GREEN1_Pin|GREEN2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

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
  while(1)
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
