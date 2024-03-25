#ifndef _BH1750_
#define _BH1750_

#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"

void bh1750_init(char i2c_name[], uint8_t address_bh1750, int *fd);
void bh1750_read(int fd, double *data_bh1750);

#endif
