#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "unistd.h"
#include "../inc/lcd_20_4.h"
#include "../inc/i2c_sensor.h"

void lcd_i2c_send_cmd(int fd, char cmd)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd&0xf0);
	data_l = ((cmd<<4)&0xf0);
	data_t[0] = data_u|0x0C;  //en=1, rs=0
	data_t[1] = data_u|0x08;  //en=0, rs=0
	data_t[2] = data_l|0x0C;  //en=1, rs=0
	data_t[3] = data_l|0x08;  //en=0, rs=0
	// i2c_write(ADDR_LCD_I2C, 0x00, data_t, 4);
	i2c_write(fd, data_t, 4);
}

void lcd_i2c_send_data(int fd, char data)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  //en=1, rs=0
	data_t[1] = data_u|0x09;  //en=0, rs=0
	data_t[2] = data_l|0x0D;  //en=1, rs=0
	data_t[3] = data_l|0x09;  //en=0, rs=0
	// i2c_write(ADDR_LCD_I2C, 0x00, data_t, 4);
	i2c_write(fd, data_t, 4);
}

void lcd_init(char i2c_name[], uint8_t address_lcd, int *fd)
{
	i2c_init(i2c_name, address_lcd, fd);
	// 4 bit initialisation
	sleep(0.05);  // wait for >40ms
	lcd_i2c_send_cmd(*fd, 0x30);
	sleep(0.005);  // wait for >4.1ms
	lcd_i2c_send_cmd(*fd, 0x30);
	sleep(0.001);  // wait for >100us
	lcd_i2c_send_cmd(*fd, 0x30);
	sleep(0.01);
	lcd_i2c_send_cmd(*fd, 0x20);  // 4bit mode
	sleep(0.01);

  // dislay initialisation
	lcd_i2c_send_cmd(*fd, 0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	sleep(0.001);
	lcd_i2c_send_cmd(*fd, 0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
	sleep(0.001);
	lcd_i2c_send_cmd(*fd, 0x01);  // clear display
	sleep(0.002);
	lcd_i2c_send_cmd(*fd, 0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	sleep(0.001);
	lcd_i2c_send_cmd(*fd, 0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
}

void lcd_i2c_clear(int fd)
{
	int i = 0;
	lcd_i2c_send_cmd(fd, 0x80);
	for(i=0; i<70; i++)
	{
		lcd_i2c_send_data(fd, ' ');
	}
}

void lcd_i2c_put_cur(int fd, int row, int col)
{
	switch (row)
	{
			case 0:
					col |= 0x80;
					break;
			case 1:
					col |= 0xC0;
					break;
			case 2:
					col |= 0x94;
					break;
			case 3:
					col |= 0xD4;
					break;
	}
	lcd_i2c_send_cmd(fd, col);
}

void lcd_i2c_send_string(int fd, char *str)
{
	while(*str) 
	{
		lcd_i2c_send_data(fd, *str++);
	}
}

