#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "unistd.h"
#include "../inc/lm75.h"
#include "../inc/i2c_sensor.h"

void lm75_init(char i2c_name[], uint8_t address_lm75, int *fd)
{
	i2c_init(i2c_name, address_lm75, fd);
}

void lm75_read(int fd, double *data_lm75)
{
	uint8_t raw_data[2] = {0};
	uint16_t data = 0;
	// double temp_value = 0.0;

	raw_data[0] = 0x00;
	i2c_write(fd, raw_data, 1);
	sleep(0.2);
	memset(raw_data, 0, sizeof(raw_data));
	i2c_read(fd, raw_data, 2);
	data = (uint16_t)(raw_data[0] << 8) | raw_data[1];
	*data_lm75 = (double)data/256.0;
	// printf("Data LM75: %f\n",*data_lm75);
}

