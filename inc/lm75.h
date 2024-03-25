#ifndef _LM75_
#define _LM75_

#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"

void lm75_init(char i2c_name[], uint8_t address_lm75, int *fd);
void lm75_read(int fd, double *data_lm75);

#endif
