#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "unistd.h"
#include "fcntl.h"
#include "../inc/uart_module_rf.h"

#define BUFFER_MAX_SIZE	1024

int8_t uart_write(char uart_name[], char data[])
{
	char buffer[BUFFER_MAX_SIZE] = {0};
	uint32_t byte_write = 0;
	int32_t fd = 0;

	byte_write = sprintf(buffer, "/dev/%s", uart_name);

	fd = open(buffer, O_RDWR);		//O_RDWR, O_WRONLY
	if(fd == -1)
	{
		printf("\nCan't open file serial %s!\n",uart_name);
		return (-1);
	}
	memset(buffer, 0, sizeof(buffer));

	byte_write = sprintf(buffer, "%s", data);
	write(fd, buffer, byte_write);
	fflush(stdout);
	close(fd);
	return 0;
}

int8_t uart_read(char uart_name[], char data[])
{
	char buffer[BUFFER_MAX_SIZE] = {0};
	uint32_t byte_write = 0;
	int32_t fd = 0;

	byte_write = sprintf(buffer, "/dev/%s", uart_name);

	fd = open(buffer, O_RDONLY);		//O_RDWR, O_WRONLY
	if(fd == -1)
	{
		printf("\nCan't open file serial %s!\n",uart_name);
		return (-1);
	}
	memset(buffer, 0, sizeof(buffer));

	read(fd, data, BUFFER_MAX_SIZE);
	fflush(stdin);
	close(fd);
	return 0;
}


