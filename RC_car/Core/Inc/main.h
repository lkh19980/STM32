/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define B1_EXTI_IRQn EXTI15_10_IRQn
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define N8_Pin GPIO_PIN_6
#define N8_GPIO_Port GPIOA
#define N7_Pin GPIO_PIN_7
#define N7_GPIO_Port GPIOA
#define N3_Pin GPIO_PIN_10
#define N3_GPIO_Port GPIOB
#define N5_Pin GPIO_PIN_7
#define N5_GPIO_Port GPIOC
#define N4_Pin GPIO_PIN_9
#define N4_GPIO_Port GPIOA
#define EchoM_Pin GPIO_PIN_10
#define EchoM_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define EchoL_Pin GPIO_PIN_3
#define EchoL_GPIO_Port GPIOB
#define N2_Pin GPIO_PIN_4
#define N2_GPIO_Port GPIOB
#define N1_Pin GPIO_PIN_5
#define N1_GPIO_Port GPIOB
#define N6_Pin GPIO_PIN_6
#define N6_GPIO_Port GPIOB
#define Trig_Pin GPIO_PIN_8
#define Trig_GPIO_Port GPIOB
#define EchoR_Pin GPIO_PIN_9
#define EchoR_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
