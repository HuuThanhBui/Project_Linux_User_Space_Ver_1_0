#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "unistd.h"
#include "../inc/bh1750.h"
#include "../inc/i2c_sensor.h"

void bh1750_init(char i2c_name[], uint8_t address_bh1750, int *fd)
{
	i2c_init(i2c_name, address_bh1750, fd);
}

void bh1750_read(int fd, double *data_bh1750)
{
	uint8_t raw_data[2] = {0};
	uint16_t data = 0;
	// double light_value = 0.0;

	raw_data[0] = 0x10;
	i2c_write(fd, raw_data, 1);
	sleep(0.2);
	memset(raw_data, 0, sizeof(raw_data));
	i2c_read(fd, raw_data, 2);
	data = (uint16_t)(raw_data[0] << 8) | raw_data[1];
	*data_bh1750 = (double)data/1.2;
	// printf("Data BH1750: %f\n",*data_bh1750);
}
