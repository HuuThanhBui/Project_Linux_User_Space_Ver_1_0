#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "unistd.h"
#include "fcntl.h"
#include "../inc/gpio_led.h"

#define BUFFER_MAX_SIZE	50

static void gpio_export(uint8_t gpio_name)
{
	char buffer[BUFFER_MAX_SIZE] = {0};
	uint32_t byte_write = 0;
	int32_t fd = 0;

	fd = open("/sys/class/gpio/export", O_WRONLY);		//O_RDWR, O_WRONLY
	if(fd == -1)
	{
		printf("\nFailed open file to export gpio\n");
		return;
	}
	byte_write = sprintf(buffer, "%d", gpio_name);
	write(fd, buffer, byte_write);
	close(fd);
}

static uint8_t gpio_set_direction(uint8_t gpio_name, gpio_direction_t gpio_direction)
{
	char buffer[BUFFER_MAX_SIZE] = {0};
	uint32_t byte_write = 0;
	int32_t fd = 0;

	byte_write = sprintf(buffer, "/sys/class/gpio/gpio%d/direction", gpio_name);

	fd = open(buffer, O_WRONLY);		//O_RDWR, O_WRONLY
	if(fd == -1)
	{
		printf("\nCan't open file to set direction gpio\n");
		return 1;
	}
	memset(buffer, 0, sizeof(buffer));
	if(gpio_direction == in)
	{
		byte_write = sprintf(buffer, "%s", "in");
	}
	else if(gpio_direction == out)
	{
		byte_write = sprintf(buffer, "%s", "out");
	}
	else if(gpio_direction == none)
	{

	}
	write(fd, buffer, byte_write);
	close(fd);
	return 0;
}

void gpio_led_init(uint8_t gpio_name, gpio_direction_t gpio_direction)
{
	gpio_export(gpio_name);
	gpio_set_direction(gpio_name, gpio_direction);
}

void gpio_led_uninit(uint8_t gpio_name)
{
	char buffer[BUFFER_MAX_SIZE] = {0};
	uint32_t byte_write = 0;
	int32_t fd = 0;

	fd = open("/sys/class/gpio/unexport", O_WRONLY);		//O_RDWR, O_WRONLY
	if(fd == -1)
	{
		printf("\nFailed open file to export gpio\n");
		return;
	}
	byte_write = sprintf(buffer, "%d", gpio_name);
	write(fd, buffer, byte_write);
	close(fd);
}

void gpio_led_write(uint8_t gpio_name, uint8_t value)
{
	char buffer[BUFFER_MAX_SIZE] = {0};
	uint32_t byte_write = 0;
	int32_t fd = 0;

	byte_write = sprintf(buffer, "/sys/class/gpio/gpio%d/value", gpio_name);

	fd = open(buffer, O_WRONLY);		//O_RDWR, O_WRONLY
	if(fd == -1)
	{
		printf("\nCan't open file to set value gpio\n");
		return;
	}
	memset(buffer, 0, sizeof(buffer));
	byte_write = sprintf(buffer, "%d", value);
	write(fd, buffer, byte_write);
	close(fd);
}

int8_t gpio_led_read(uint8_t gpio_name)
{
	char buffer[BUFFER_MAX_SIZE] = {0};
	uint32_t byte_write = 0;
	int32_t fd = 0;

	byte_write = sprintf(buffer, "/sys/class/gpio/gpio%d/value", gpio_name);

	fd = open(buffer, O_RDONLY);		//O_RDWR, O_WRONLY
	if(fd == -1)
	{
		printf("\nCan't open file to set value gpio\n");
		return (-1);
	}
	memset(buffer, 0, sizeof(buffer));
	read(fd, buffer, BUFFER_MAX_SIZE);
	close(fd);
	return atoi(buffer);
}



