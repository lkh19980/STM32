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
#include "myLib.h"
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
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart6;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART6_UART_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

}
//extern myBuffer Buf;//data buffer
extern myCMDSET myCmd[];
myBuffer *Buf1,*Buf2;//null pointer
//myBuffer Buf1,Buf2;
char dum1, dum2;
int idx1 = 0, idx2 = 0;
int eol = 0;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart6) // address of handle
	{
		Buf1->v0[idx1++] = dum1;
		HAL_UART_Receive_IT(&huart6,&dum1,1);//(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)


	}
	else if(huart == &huart2) // KD input by Putty
	{
		if(dum2  == '\r') // [Enter] Key
		{
			HAL_UART_Transmit(&huart2,"\r\n",2,10); //Echo, send putty
			HAL_UART_Transmit(&huart6,Buf2,idx2,100);// redireciton to BT, take long sentence long waiting time
			switch(eol)
			{
			case 1:
				HAL_UART_Transmit(&huart6,"\r\n",2,10); // CRLF
				break;
			case 2:
				HAL_UART_Transmit(&huart6,"\n",1,10); // CR
				break;
			case 3:
				HAL_UART_Transmit(&huart6,"\0",1,10); // NULL
				break;
			case 4:
				//HAL_UART_Transmit(&huart6,"\0",1,10); // NONE
				break;
			}

			idx2 = 0;
		}
		else
		{
			HAL_UART_Transmit(&huart2,&dum2,1,10);//UART_HandleTypeDef *huart, const uint8_t *pData, uint16_t Size, uint32_t Timeout)// Echo
			Buf2->v0[idx2++] = dum2;
			Buf2->v0[idx2] = 0; //Null
		}
		HAL_UART_Receive_IT(&huart2,&dum2,1);//(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size)
	}
}
/*
char* Trim(char *s) // delete white_space("SPC \t \r \n left/right) "    ABC     " ==> "ABC"
{
	int h = 0, t = strlen(s)-1;//head, tail
	if(t<0) return NULL;
	while(*(s+h) == ' ' || *(s+h) == '\t' || *(s+h) == '\r' || *(s+h) == '\n') h++;
	while(*(s+t) == ' ' || *(s+t) == '\t' || *(s+t) == '\r' || *(s+t) == '\n') t--;

	//if(h==0, t == strlen(s)-1) return (s);//consider change
	char *ret = (char *)malloc(t-h+1+1);
	memcpy(ret,s+h, t-h +1); ret[t-h+1] = 0;//memcopy cannot copy NULL // memcopy-> count 1-start ret[t-h+1] count 0-start
	return ret;
	/*
	while(1)
	{
		if(*(s+h) == ' ' || *(s+h) == '\t') h++;//
		else break;
	}
	///
}
*/
/*
int myStrncmp(char *s1, char *s2, int n)// 0 or 1 (true or false)
{
	for(int i = 0 ; i < n ; i++)
	{
		//if(*(s1+i)|0x20 != *(s2+i)| 0x20) return 1; // not working properly in priorities
		if(((*(s1+i))|0x20) != ((*(s2+i))| 0x20)) return 1; // 0b0010 0000 //not exactly
	}
	return 0;
}
int myStrlen(char *s)//length to white space
{
	int i;

	//for(i=0;*(s+i);i++); return i;

	for(i=0;;i++)
	{
		if(*(s+i) == 0 || *(s+i) == ' ' || *(s+i) == '\t' || *(s+i) == '\r' || *(s+i) == '\n') return i;//break;
	}
	return -1;

	//return strlen(s)-1;
}
int CheckCMD(char* s)// *s = "   LED   1    ON    " ===> *b "LED   1    ON"
{
	int ret = 0;
	char *b = Trim(s), *c = NULL;
	if(*b == NULL)return 0;
	for(int i =0; myCmd[i].key; i++)
	{

		if(myStrncmp(b, myCmd[i].key, myStrlen(myCmd[i].key)) == 0)
		{
			for(int j = 0; ;j++)
			{
				int idx = myStrlen(b);//NULL included
				if(c) free(c);
				c = Trim(b + idx);//*c "LED   1    ON" *c = "1    ON"
				free(b); b = c;
				if(myStrlen(c) == 0){ret = 1; break;}



//				int n = myCmd[i].op_no;
//				while(*c){if(*c == ' ')n--; c++;}//full array search and count space' '
//				if(n == 0) return 1; else return 0;//not matched number of options
			}

		}
	}
	return ret;
}

//char p0[10],p1[10],p2[10];//pre-assigned memory
int Parsing(char* s, char* p[])//**P *s = "LED 1 ON" ===> "LED\01\0on" ret v = 3
//default split and judge
//divide and rule
//
//space -> NULL replace
{


/*
//	if(!CheckCMD(b)) return 0;
//	int n = 0; // index of p
//	p[n++] = b;


//	for(int i = 0; b[i] != 0 ;i++)//b[i] != 0 ==> 0->true
//	{
//		if(b[i] == ' ' )
//		{
			//memcpy(p0,b,i);

//			b[i] = 0; // '\0'
//			p[n++] = b+ i +1;
//		}
//	}
//	return n;
*/
/*
	//while(*b) { if(*b == ' ') { *b = '\0'; p[n++] = ++b; } }
	//p is declared in main.c don't concern it
	int ret = 0;
	char *b = Trim(s), *c = NULL;//front and back white space removed

	if(*b == NULL)return 0;
	for(int i =0;myCmd[i].key;i++)
	{

		if(myStrncmp(b, myCmd[i].key, myStrlen(myCmd[i].key)) == 0)
		{

			for(int j = 0; ;j++)
			{
				p[j]=b;
				int idx = myStrlen(b);//NULL included function
				if(strlen(b) == idx){ret = j; break;}//strlen is to '\0' mystrlen is to whitespace
				*(b+idx) = 0;//"LED " ==> "LED\0"
				//if(c) free(c);			   __
				c = Trim(b + idx + 1);//*c "LED   1    ON" *c = "1    ON"
				//free(b);
				b = c;
				if(myStrlen(c) == 0){ret = j; break;}



//				int n = myCmd[i].op_no;
//				while(*c){if(*c == ' ')n--; c++;}//full array search and count space' '
//				if(n == 0) return 1; else return 0;//not matched number of options
			}

		}
	}
	return ret;
}
*/
char* p[5]; //pointer array
/*
void LED_Control()
{
  int ln,st;
  sscanf(p[1],"%d",&ln);
  //if(strcmp(p[2],"ON") == 0) st = 1;else st = 0;
  st = (myStrncmp(p[2],"ON",2) == 0) ? 1 : (myStrncmp(p[2],"OFF",3) == 0) ? 0 : -1;
  switch(ln)
  {
  case 1:
	  HAL_GPIO_WritePin(L1_GPIO_Port,L1_Pin,st);break;
  case 2:
	  HAL_GPIO_WritePin(L2_GPIO_Port,L2_Pin,st);break;
  case 3:
	  HAL_GPIO_WritePin(L3_GPIO_Port,L3_Pin,st);break;
  }
  printf("%s %d had controlled to %d\r\n",p[0],ln, st);


}
*/
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	myBuffer b1, b2;// not cleared
	Buf1 = &b1,Buf2 = &b2;
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
  MX_USART6_UART_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  ProgramStart("UART - Bluetooth"); Cursor(1);
  printf("Select EOL \r\n 1:CRLF \r\n 2:LF\r\n 3:NULL\r\n 4:NONE\r\n");
  setvbuf(stdin, NULL, _IONBF, 0); // buffer clear
  scanf("%d",&eol);

  char *t1 = "   Test string   ";
  char *t2 = Trim(t1);
  printf("Before :Trim([%s]) = [%s]\r\n",t1,t2); free(t2);//free(ptr addr);
  printf("After :Trim([%s]) = [%s]\r\n",t1,t2);//free(ptr addr);
  printf("your select is %d. \r\n",eol);
  //Wait();
  HAL_UART_Receive_IT(&huart6, &dum1, 1);
  HAL_UART_Receive_IT(&huart2, &dum2, 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  //int i = 0;
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if(idx1)
	  {

		  //Buf1.v0[idx1] = 0;//NULL
		  Buf1->v0[idx1] = 0;
		  printf("%s\r\n", Buf1);
		  //	LED 1	On ==> Parsing(Buf1) => ret : 3 (no. of string)
		  //char a1[10], a2[5], a3[5];//array for return value
		  //char* p[5]; //pointer array
		  int r = Parsing(Buf1, p);// return val : 1:cmd 2:op1 3:op2

		  if(r > 0)
		  {
			  printf("Parsing --->	[%s]	[%s]	[%s] \r\n",p[0],p[1],p[2]);
			  if(myStrncmp(p[0], "LED", 3) == 0) LED_Control();
			  //else if(myStrncmp(p[0],"MOTOR",5) MOTOR_Control();
			  //else if(myStrncmp(p[0],"BUZZER",6) BUZZER_Control();
			  /*
			  if(myStrncmp(p[0], "LED", 3) == 0)
			  {
				  int ln,st;
				  sscanf(p[1],"%d",&ln);
				  //if(strcmp(p[2],"ON") == 0) st = 1;else st = 0;
				  st = (myStrncmp(p[2],"ON",2) == 0) ? 1 : (myStrncmp(p[2],"OFF",3) == 0) ? 0 : -1;
				  switch(ln)
				  {
				  case 1:
					  HAL_GPIO_WritePin(L1_GPIO_Port,L1_Pin,st);break;
				  case 2:
					  HAL_GPIO_WritePin(L2_GPIO_Port,L2_Pin,st);break;
				  case 3:
					  HAL_GPIO_WritePin(L3_GPIO_Port,L3_Pin,st);break;
				  }
				  printf("%s %d had controlled to %d\r\n",p[0],ln, st);
			  }*/
		  }
		  idx1=0;
/*
		  if(r != 0)
		  {

		  }
		  */

		  /*
		  if(strncmp(Buf1, "LED",3) == 0)// -1 0 1 => 0 or Not
		  {//strcmp + count n
			  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin,1); //PA5
			  //HAL_Delay(500); //500ms
			  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin,1); //PA5
			  //HAL_Delay(500); //500ms;
		  }
		  else if (strncmp(Buf1,"OFF",3) == 0)
		  {
			  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin,0);
			  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin,0);
		  }
		  */

		  //printf("%s\r\n", Buf1);

	  }
	  HAL_Delay(1000);
	  /*
	  for(i=0;i<5;i++)
	  {
		  printf("test break");
		  if(i == 2) break;
	  }*/
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
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 38400;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

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
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 9600;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, L1_Pin|L2_Pin|L3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : L1_Pin L2_Pin L3_Pin */
  GPIO_InitStruct.Pin = L1_Pin|L2_Pin|L3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

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
