#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "unistd.h"
#include "fcntl.h"
#include "sys/ioctl.h"
#include "linux/i2c.h"
#include "linux/i2c-dev.h"
#include "../inc/i2c_sensor.h"

#define BUFFER_MAX_SIZE	100

void i2c_init(char name_i2c[], uint32_t add_slave, int *fd)
{
	char buffer[BUFFER_MAX_SIZE] = {0};
	uint32_t byte_write = 0;

	byte_write = sprintf(buffer, "/dev/%s", name_i2c);

	*fd = open(buffer, O_RDWR);		//O_RDWR, O_WRONLY
	if(*fd == -1)
	{
		printf("\nCan't open file to init i2c\n");
		return;
	}

	if(ioctl(*fd, I2C_SLAVE, add_slave) < 0)
	{
		printf("\nFailed to connect to the sensor\n");
		return;
	}
}

void i2c_write(int fd, uint8_t data[], uint8_t leng_data)
{
	if(write(fd, data, leng_data) != leng_data)
	{
		printf("\nFailed to write data to sensor !\n");
		return;
	}
}

void i2c_read(int fd, uint8_t data[], uint8_t leng_data)
{
	if(read(fd, data, leng_data) != leng_data)
	{
		printf("\nFailed to read in the buffer\n");
		return;
	}
}

void i2c_close(int fd)
{
	close(fd);
}




