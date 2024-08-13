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
//int CMD_NO = 2;
myCMDSET myCmd[] =
{
		{"LED", 2},
		{"MOTOR",2},
		{"BUZZ",2},
		{NULL,0} //NULL

};
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
/*=========================================================================
 *	문자열 처리 함수 string function
 * 1. char* Trim(char *s)
 * 2. int myStrncmp(char *s1, char *s2, int n)
 * 3. int myStrlen(char *s)
 * 4. int CheckCMD(char* s)
 * 5. int Parsing(char* s, char* p[])
 *=========================================================================
 *
 */
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
	*/
}
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
