/*
 * lcd.c
 *
 *  Created on: Jul 31, 2024
 *      Author: user
 */
#include "main.h"
#include "myLib.h"
extern I2C_HandleTypeDef hi2c1;

i2c_scan() // 현재 연결되어 있는 I2c device의 주소를 표시하고 반환
//display and return I2C device's address which are current connected
{
	int addr, ret;
	//HAL_StatusTypeDef ret;
	for(addr = 0; addr<128 ; addr++)
	{
		if(HAL_I2C_IsDeviceReady(&hi2c1,addr<<1,1,10) == HAL_OK)//dd check
		{
			printf(" 0x%02x",addr); ret = addr;

		}
		else printf("  .  ");
		if((addr % 8) == 0) printf("\r\n");
	}
	printf("\r\n");
	return ret;
}
void lcd_command(char cmd) // cmd : abcd efgh
{
	char d1, d2,data[4];
	d1 = cmd & 0xf0; // d1 : abcd 0000
	d2 = cmd<<4;//(cmd<<4) & 0xf0; // d2 : efgh 0000,  0000 efgh ===> efgh 0000
	data[0] = d1 | 0x0C; // 0000 1100
	data[1] = d1 | 0x08; // 0000 1000
	data[2] = d2 | 0x0C; // 0000 1100
	data[3] = d2 | 0x08; // transmit timing

	HAL_I2C_Master_Transmit(&hi2c1,LCD1602_ADDR, data, 4, 10);//handle, devAddr, &addr, size, timeout
}
void lcd_data(char ch) //SINGLE CHARACTER TRANSMIT
{
	char d1, d2,data[4];
	d1 = ch & 0xf0; // d1 : abcd 0000
	d2 = ch<<4;//(cmd<<4) & 0xf0; // d2 : efgh 0000,  0000 efgh ===> efgh 0000
	data[0] = d1 | 0x0D; // 0000 1101
	data[1] = d1 | 0x09; // 0000 1001
	data[2] = d2 | 0x0D; // 0000 1101
	data[3] = d2 | 0x08; // transmit timing

	HAL_I2C_Master_Transmit(&hi2c1,LCD1602_ADDR, data, 4, 10);//handle, devAddr, &addr, size, timeout
}
void lcd_print(char *s)
{
	while(*s){
		lcd_data(*s++);
	}
}
void lcd_printEx(char *s, int ln)
{
	if(ln == 1) lcd_command(0x80);
	else if(ln == 2) lcd_command(0xc0);
	while(*s){
		lcd_data(*s++);
	}
}
void lcd_init() // 1602 LCD initializing : Disp ON, Cursor Position, etc
{

	lcd_command(0x01);// screen clear
	lcd_command(0x02);// cursor reset to the default 0x0 position
	lcd_command(0x06);// increment mode, shift off
	lcd_command(0x0f);
	lcd_command(0x14);// shift cursor, shift right
}

