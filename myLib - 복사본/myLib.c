/*
 * myLib.c
 *
 *  Created on: Jul 4, 2024
 *      Author: user
 */

#include "main.h"
#include "myLib.h"

extern UART_HandleTypeDef huart2;

myBuffer Buf;
int lm = 0;

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
void Cursor(int OnOff)
{
	if(OnOff) printf("\033[?25h\n"); else printf("\033[?25l\n");
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
void Dump(int Mode) // 1,2,4...(byte)
{
	int max_row = 20;
	int max_col = 16/Mode;
	char str[10];
	sprintf(str,"%%%02dX ",Mode*2); //printf format sring %02x %04x %08x

	printf("\033[4;17H\n");
	printf("Memory dump mode - %d         \r\n",Mode);
	//printf("\033[5;0H\n");
	for (int i = 0; i < max_row; i++) //ROW index
	{
		for (int j = 0; j < max_col; j++) // COULUMN index
		{
			unsigned int v =
					(Mode == 1)? Buf.v0[(i * 16) + j]:
					(Mode == 2)? Buf.v1[(i * 8) + j] :
					(Mode == 4)? Buf.v2[(i * 4) + j] : -1;
			printf(str, v);
			if (j == (8/Mode)-1){
				printf("  ");
			}

		}
		printf("                       \r\n");
	}
	HAL_Delay(1000);
	/*
	switch (Mode) {
			case 1: //byte dump
				if (lm != Mode) {
					cls();
					lm = Mode;
				}
				//printf("\033[5;0H");
				/*
				 for(j=0;j<20;j++){
				 for(i=0;i<16;i++)
				 {
				 printf("%02X ",buf.v0[16*j+i]);
				 }
				 printf("\r\n");

				 }
				 */
				/*
				 for(i=0;i<320;i++)
				 {
				 printf("%02X ",buf.v0[i]);
				 if((i-7)%8 == 0) printf(" ");
				 if(!((i-15)%16)){
				 printf("\r\n");
				 }
				 }
				 */
				/*
				printf("\033[4;17H\n");
				printf("Memory dump mode         \r\n");
				printf("\033[5;0H\n");
				for (int i = 0; i < 20; i++) //ROW index
						{
					for (int j = 0; j < (16); j++) // COULUMN index
							{
						printf("%02X ", Buf.v0[(i * 16) + j]);
						if (j == 7)
							printf("  ");
					}
					printf("\r\n");
				}
				HAL_Delay(1000);
				break;
			case 2: //short dump
				if (lm != Mode) {
					cls();
					lm = Mode;
				}
				printf("\033[4;17H\n");
				printf("Memory dump mode Short     \r\n");
				for (int i = 0; i < max_row; i++) //ROW index
						{
					for (int j = 0; j < 8; j++) // COULUMN index
							{
						printf("%04X ", Buf.v1[(i * 8) + j]);
						if (j == 3)
							printf("  ");
					}
					printf("\r\n");
				}
				HAL_Delay(1000);
				break;
			case 4: //int dump
				if (lm != Mode) {
					cls();
					lm = Mode;
				}
				printf("\033[4;17H\n");
				printf("Memory dump mode int        \r\n");
				for (int i = 0; i < max_row; i++) //ROW index
						{
					for (int j = 0; j < 4; j++) // COULUMN index
							{
						printf("%08x ", Buf.v2[(i * 4) + j]);
						if (j == 1)
							printf("  ");
					}
					printf("\r\n");
				}
				HAL_Delay(1000);
				break;
			}
			*/
}
int head = 0, tail = 0;
	int GetBuffer(char* b) // b : char array pointer for destination //return data numbers
	{
		int len = 0;
		char* s = &Buf;//.v0[0];
		tail = MAX_BUF - huart2.hdmarx->Instance ->NDTR;
		if(tail > head)
		{

			memcpy(b,s + head ,tail-head);//destination,source,length //from head to tail

			len = tail -head;
		}
		else if(tail < head)
		{

			memcpy(b,s+head,MAX_BUF - head); // from head to End
			memcpy(b + MAX_BUF - head,s,tail); // from Start to Tell
			len = MAX_BUF -head + tail;
		}
		else //tail == head
		{
			len = 0;
		}
		*(b + len) = NULL;
		head = tail;
		return len;

	}
