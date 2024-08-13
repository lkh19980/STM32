/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
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
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim5;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM5_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int state = 0;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == B1_Pin) {
		if (state++ > 3)
			state = 0;
		Way();
	}

}
// ultra sonic================================================================
int t1 = 0, t2 = 0;
int ts = 0; //,t2=0;
double dist = 0.0;
void microDelay(int us) {
	/*
	 int t1 = htim5.Instance->CNT;
	 while(1)
	 {
	 int tmp_us = htim5.Instance->CNT - t1; // 경과?���?????????????????????
	 if(tmp_us > us) break;
	 }
	 */
	//while((htim5.Instance->CNT - t1) > us);
	htim5.Instance->CNT = 0;
	while (htim5.Instance->CNT < us)
		;
}
void Trigger() {
	//TriggerSignal

	HAL_GPIO_WritePin(Trig_GPIO_Port, Trig_Pin, 1);
	microDelay(10);	  //HAL_Delay(0.01);X
	HAL_GPIO_WritePin(Trig_GPIO_Port, Trig_Pin, 0);
	microDelay(20);
}
double Distance(int pos) {
	Trigger();
	switch(pos)
	{
	case 0 :
		// Wait util Echo Rising Edge
			while (HAL_GPIO_ReadPin(EchoL_GPIO_Port, EchoL_Pin) != 1)
				;
			t1 = htim5.Instance->CNT;
			//int t1 = htim5.Instance->CNT;
			// Wait until Echo Falling_Edge
			while (HAL_GPIO_ReadPin(EchoL_GPIO_Port, EchoL_Pin) != 0)
				;
			t2 = htim5.Instance->CNT;
			//int t2 = htim5.Instance->CNT;


			break;

	case 1:
		// Wait util Echo Rising Edge
			while (HAL_GPIO_ReadPin(EchoM_GPIO_Port, EchoM_Pin) != 1)
				;
			t1 = htim5.Instance->CNT;
			//int t1 = htim5.Instance->CNT;
			// Wait until Echo Falling_Edge
			while (HAL_GPIO_ReadPin(EchoM_GPIO_Port, EchoM_Pin) != 0)
				;
			t2 = htim5.Instance->CNT;
			//int t2 = htim5.Instance->CNT;
			break;
	case 2:
		// Wait util Echo Rising Edge
			while (HAL_GPIO_ReadPin(EchoR_GPIO_Port, EchoR_Pin) != 1)
				;
			t1 = htim5.Instance->CNT;
			//int t1 = htim5.Instance->CNT;
			// Wait until Echo Falling_Edge
			while (HAL_GPIO_ReadPin(EchoR_GPIO_Port, EchoR_Pin) != 0)
				;
			t2 = htim5.Instance->CNT;
			//int t2 = htim5.Instance->CNT;
		break;
	}

	double dist = (t2 - t1) * 0.000170; // meter
	//dist *= 100;

	//microDelay(100000);

	return dist;
}

void Way(void){
	switch (state) //N1 = 1, N2 = 0 forward
			{
			case 0:
				HAL_GPIO_WritePin(N1_GPIO_Port, N1_Pin, 1);
				HAL_GPIO_WritePin(N2_GPIO_Port, N2_Pin, 1);
				HAL_GPIO_WritePin(N3_GPIO_Port, N3_Pin, 1);
				HAL_GPIO_WritePin(N4_GPIO_Port, N4_Pin, 1);
				HAL_GPIO_WritePin(N5_GPIO_Port, N5_Pin, 1);
				HAL_GPIO_WritePin(N6_GPIO_Port, N6_Pin, 1);
				HAL_GPIO_WritePin(N7_GPIO_Port, N7_Pin, 1);
				HAL_GPIO_WritePin(N8_GPIO_Port, N8_Pin, 1);
				printf("0\r\n");
				break;
			case 1:
				HAL_GPIO_WritePin(N1_GPIO_Port, N1_Pin, 1);
				HAL_GPIO_WritePin(N2_GPIO_Port, N2_Pin, 0);
				HAL_GPIO_WritePin(N3_GPIO_Port, N3_Pin, 1);
				HAL_GPIO_WritePin(N4_GPIO_Port, N4_Pin, 0);
				HAL_GPIO_WritePin(N5_GPIO_Port, N5_Pin, 1);
				HAL_GPIO_WritePin(N6_GPIO_Port, N6_Pin, 0);
				HAL_GPIO_WritePin(N7_GPIO_Port, N7_Pin, 1);
				HAL_GPIO_WritePin(N8_GPIO_Port, N8_Pin, 0);
				printf("1\r\n");
				break;
			case 2:
				HAL_GPIO_WritePin(N1_GPIO_Port, N1_Pin, 0);
				HAL_GPIO_WritePin(N2_GPIO_Port, N2_Pin, 1);
				HAL_GPIO_WritePin(N3_GPIO_Port, N3_Pin, 0);
				HAL_GPIO_WritePin(N4_GPIO_Port, N4_Pin, 1);
				HAL_GPIO_WritePin(N5_GPIO_Port, N5_Pin, 0);
				HAL_GPIO_WritePin(N6_GPIO_Port, N6_Pin, 1);
				HAL_GPIO_WritePin(N7_GPIO_Port, N7_Pin, 0);
				HAL_GPIO_WritePin(N8_GPIO_Port, N8_Pin, 1);
				printf("2\r\n");
				break;
			case 3: //left
				HAL_GPIO_WritePin(N1_GPIO_Port, N1_Pin, 1);
				HAL_GPIO_WritePin(N2_GPIO_Port, N2_Pin, 0);
				HAL_GPIO_WritePin(N3_GPIO_Port, N3_Pin, 0);
				HAL_GPIO_WritePin(N4_GPIO_Port, N4_Pin, 1);
				HAL_GPIO_WritePin(N5_GPIO_Port, N5_Pin, 0);
				HAL_GPIO_WritePin(N6_GPIO_Port, N6_Pin, 1);
				HAL_GPIO_WritePin(N7_GPIO_Port, N7_Pin, 1);
				HAL_GPIO_WritePin(N8_GPIO_Port, N8_Pin, 0);
				printf("3\r\n");
				break;
			case 4: //right
				HAL_GPIO_WritePin(N1_GPIO_Port, N1_Pin, 0);
				HAL_GPIO_WritePin(N2_GPIO_Port, N2_Pin, 1);
				HAL_GPIO_WritePin(N3_GPIO_Port, N3_Pin, 1);
				HAL_GPIO_WritePin(N4_GPIO_Port, N4_Pin, 0);
				HAL_GPIO_WritePin(N5_GPIO_Port, N5_Pin, 1);
				HAL_GPIO_WritePin(N6_GPIO_Port, N6_Pin, 0);
				HAL_GPIO_WritePin(N7_GPIO_Port, N7_Pin, 0);
				HAL_GPIO_WritePin(N8_GPIO_Port, N8_Pin, 1);
				printf("4\r\n");
				break;
			}
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

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
	MX_USART2_UART_Init();
	MX_TIM1_Init();
	MX_TIM5_Init();
	/* USER CODE BEGIN 2 */
	ProgramStart("DC_PWM");
	HAL_TIM_Base_Start(&htim5);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
	htim1.Instance->CCR1 = 80;
	htim1.Instance->CCR3 = 80;
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		//printf("%d\r\n", htim1.Instance->CCR1);


		double d = Distance(0) * 100;
		printf("\033[?25l");  //cursor off
		printf("\033[5;10H\033#3DistanceL : %.2f cm    ", d);
		printf("\033[6;10H\033#4DistanceL : %.2f cm    \033[9;10H\n", d);
		HAL_Delay(10);
		d = Distance(1) * 100;
		printf("\033[?25l");  //cursor off
		printf("\033[10;10H\033#3DistanceM : %.2f cm    ", d);
		printf("\033[11;10H\033#4DistanceM : %.2f cm    \033[9;10H\n", d);
		HAL_Delay(10);
		d = Distance(2) * 100;
		printf("\033[?25l");  //cursor off
		printf("\033[15;10H\033#3DistanceR : %.2f cm    ", d);
		printf("\033[16;10H\033#4DistanceR : %.2f cm    \033[9;10H\n", d);
		HAL_Delay(10);
		/*
		 if(htim1.Instance->CCR1>100)
		 {
		 htim1.Instance->CCR1 = 0;
		 htim1.Instance->CCR3 = 0;
		 }
		 htim1.Instance->CCR1++;
		 htim1.Instance->CCR3++;
		 */
		/*
		 //for (int i = 70; i < 100; i++)
		 {
		 //htim1.Instance -> CCR1 = i;
		 //htim1.Instance -> CCR3 = i;
		 //printf("%d\r\n" ,i);
		 HAL_GPIO_WritePin(N1_GPIO_Port, N1_Pin,1);
		 HAL_GPIO_WritePin(N3_GPIO_Port, N3_Pin,0);
		 HAL_GPIO_WritePin(N4_GPIO_Port, N4_Pin,1);
		 HAL_Delay(1000); //500ms
		 /*HAL_GPIO_WritePin(N3_GPIO_Port, N3_Pin,1);
		 HAL_GPIO_WritePin(N4_GPIO_Port, N4_Pin,0);
		 HAL_Delay(1000); //500ms
		 }*/
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 16;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
	RCC_OscInitStruct.PLL.PLLQ = 4;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief TIM1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM1_Init(void) {

	/* USER CODE BEGIN TIM1_Init 0 */

	/* USER CODE END TIM1_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };
	TIM_OC_InitTypeDef sConfigOC = { 0 };
	TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = { 0 };

	/* USER CODE BEGIN TIM1_Init 1 */

	/* USER CODE END TIM1_Init 1 */
	htim1.Instance = TIM1;
	htim1.Init.Prescaler = 840 - 1;
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.Period = 100 - 1;
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.RepetitionCounter = 0;
	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim1) != HAL_OK) {
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_TIM_PWM_Init(&htim1) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1)
			!= HAL_OK) {
		Error_Handler();
	}
	sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
	sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
	sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
	sBreakDeadTimeConfig.DeadTime = 0;
	sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
	sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
	sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
	if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM1_Init 2 */

	/* USER CODE END TIM1_Init 2 */
	HAL_TIM_MspPostInit(&htim1);

}

/**
 * @brief TIM5 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM5_Init(void) {

	/* USER CODE BEGIN TIM5_Init 0 */

	/* USER CODE END TIM5_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };

	/* USER CODE BEGIN TIM5_Init 1 */

	/* USER CODE END TIM5_Init 1 */
	htim5.Instance = TIM5;
	htim5.Init.Prescaler = 84 - 1;
	htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim5.Init.Period = 4294967295;
	htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim5) != HAL_OK) {
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM5_Init 2 */

	/* USER CODE END TIM5_Init 2 */

}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void) {

	/* USER CODE BEGIN USART2_Init 0 */

	/* USER CODE END USART2_Init 0 */

	/* USER CODE BEGIN USART2_Init 1 */

	/* USER CODE END USART2_Init 1 */
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart2) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART2_Init 2 */

	/* USER CODE END USART2_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	/* USER CODE BEGIN MX_GPIO_Init_1 */
	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, LD2_Pin | N8_Pin | N7_Pin | N4_Pin,
			GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, N3_Pin | N2_Pin | N1_Pin | N6_Pin | Trig_Pin,
			GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(N5_GPIO_Port, N5_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin : B1_Pin */
	GPIO_InitStruct.Pin = B1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : LD2_Pin N8_Pin N7_Pin N4_Pin */
	GPIO_InitStruct.Pin = LD2_Pin | N8_Pin | N7_Pin | N4_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : N3_Pin N2_Pin N1_Pin N6_Pin
	 Trig_Pin */
	GPIO_InitStruct.Pin = N3_Pin | N2_Pin | N1_Pin | N6_Pin | Trig_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin : N5_Pin */
	GPIO_InitStruct.Pin = N5_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(N5_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : EchoM_Pin */
	GPIO_InitStruct.Pin = EchoM_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(EchoM_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : EchoL_Pin EchoR_Pin */
	GPIO_InitStruct.Pin = EchoL_Pin | EchoR_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

	/* USER CODE BEGIN MX_GPIO_Init_2 */
	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
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
