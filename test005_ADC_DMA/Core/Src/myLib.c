/*
 * myLib.c
 *
 *  Created on: Jul 4, 2024
 *      Author: user
 */

#include "main.h"

extern UART_HandleTypeDef huart2;

int __io_putchar(int ch)//char
{
	HAL_UART_Transmit(&huart2,&ch,1,10); //10ms
	return ch;
}
int __io_getchar(void)
{
	char ch;
	while(HAL_UART_Receive(&huart2, &ch, 1, 10)!= HAL_OK);
	//{
	//	if(HAL_UART_Receive(&huart2, &ch, 1, 10)== HAL_OK)break;
	//}
	HAL_UART_Transmit(&huart2,&ch,1,10); // 10ms echo
	if(ch == '\r') HAL_UART_Transmit(&huart2, "\n",1, 10);

	return ch;
}
void cls()
{
	printf("\033[2J\033[1;1H\n");// screen clear & move cur to (1,1)
}
void Wait()
{
	while(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) != 0); //B1 for start
}
void ProgramStart(char *name)
{
	  //printf("\033[2J\033[1;1H\n");// y;xH : (x,y)move axis
	cls();
	printf("Program(%s) ready. Press Blue button to start\r\n",name);
	  Wait();
}
