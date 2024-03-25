#ifndef _UART_MODULE_RF_H_
#define _UART_MODULE_RF_H_

#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"

int8_t uart_write(char uart_name[], char data[]);
int8_t uart_read(char uart_name[], char data[]);

#endif




