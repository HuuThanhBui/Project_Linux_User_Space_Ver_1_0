#ifndef _LCD_20_4_
#define _LCD_20_4_

#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"

void lcd_i2c_send_cmd(int fd, char cmd);
void lcd_i2c_send_data(int fd, char data);
void lcd_init(char i2c_name[], uint8_t address_lcd, int *fd);
void lcd_i2c_clear(int fd);
void lcd_i2c_put_cur(int fd, int row, int col);
void lcd_i2c_send_string(int fd, char *str);

#endif
