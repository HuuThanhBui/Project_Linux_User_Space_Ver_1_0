#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "unistd.h"
#include "pthread.h"
#include "malloc.h"
#include "../inc/string_handler.h"
#include "../inc/main.h"
#include "../inc/uart_module_rf.h"
#include "../inc/gpio_led.h"
#include "../inc/lcd_20_4.h"
#include "../inc/bh1750.h"
#include "../inc/lm75.h"
#include "../inc/i2c_sensor.h"

#define ADDR_LM75			0x48
#define ADDR_BH1750			0x23
#define ADDR_LCD_I2C_20_4	0x27

void lcd_display(int fd, double data_lm75, double data_bh1750)
{
	char data_lm75_str[20] = {0}, data_bh1750_str[20] = {0};
	sprintf(data_lm75_str,"TEMP: %f",(float)data_lm75);
	sprintf(data_bh1750_str,"LUX: %f",(float)data_bh1750);
	lcd_i2c_clear(fd);
	lcd_i2c_put_cur(fd, 0, 0);
	lcd_i2c_send_string(fd, data_lm75_str);
	lcd_i2c_put_cur(fd, 1, 0);
	lcd_i2c_send_string(fd, data_bh1750_str);
}

void *thread_read_data_ble(void *arg)
{
	char data_ble[1024] = {0};
	int count = 1;
	data_project_t *data = (data_project_t *)arg;
	while(1)
	{
		memset(data_ble, 0, sizeof(data_ble));
		uart_read("ttyS2", data_ble);
		// printf("\nData_BLE: %s",data_ble);
		switch(count)
		{
			case 1:
			{
				memset(data->data_ble.zone_1, 0, sizeof(data->data_ble.zone_1));
				strcpy(data->data_ble.zone_1, data_ble);
				count = 2;
				break;
			} 
			case 2:
			{
				memset(data->data_ble.zone_2, 0, sizeof(data->data_ble.zone_2));
				strcpy(data->data_ble.zone_2, data_ble);
				count = 3;
				break;
			} 
			case 3:
			{
				memset(data->data_ble.zone_3, 0, sizeof(data->data_ble.zone_3));
				strcpy(data->data_ble.zone_3, data_ble);
				count = 1;
				break;
			} 
		}
	}
	pthread_detach(pthread_self());
}

void *thread_read_data_zigbee(void *arg)
{
	char data_zigbee[1024] = {0};
	int count = 1;
	data_project_t *data = (data_project_t *)arg;
	while(1)
	{
		memset(data_zigbee, 0, sizeof(data_zigbee));
		uart_read("ttyS4", data_zigbee);
		printf("\nData_Zigbee: %s\n",data_zigbee);
		switch(count)
		{
			case 1:
			{
				memset(data->data_zigbee.zone_1, 0, sizeof(data->data_zigbee.zone_1));
				strcpy(data->data_zigbee.zone_1, data_zigbee);
				count = 2;
				break;
			}
			case 2:
			{
				memset(data->data_zigbee.zone_2, 0, sizeof(data->data_zigbee.zone_2));
				strcpy(data->data_zigbee.zone_2, data_zigbee);
				count = 1;
				break;
			}
		}
	}
	pthread_detach(pthread_self());
}

void *thread_read_data_wifi(void *arg)
{
	typedef enum{
		Zigbee = 1,
		BLE
	}device_type_t;
	char data_wifi[1024] = {0}, data_send_to_device[50] = {0};
	uint8_t arr_int[5] = {0};
	data_project_t *data = (data_project_t *)arg;
	while(1)
	{
		memset(data_wifi, 0, sizeof(data_wifi));
		uart_read("ttyS5", data_wifi);
		printf("\nData_Wifi: %s\n",data_wifi);
		change_arr_str_to_int(data_wifi, arr_int);
		
		switch((device_type_t)arr_int[0])
		{
			case Zigbee:
			{
				memset(data_send_to_device, 0, sizeof(data_send_to_device));
				// printf("Data wifi to Zigbee device after handler: %d %d %d\n",arr_int[1],arr_int[2],arr_int[3]);
				sprintf(data_send_to_device,"%d %d %d",arr_int[1],arr_int[2],arr_int[3]);
				if(strlen(data_send_to_device) < 10)
				{
					printf("\nData_Before_Send_To_Device_Zigbee: %s\n",data_send_to_device);
					uart_write("ttyS4",data_send_to_device);
					uart_write("ttyS4","\n");
				}
				break;
			}
			case BLE:
			{
				memset(data_send_to_device, 0, sizeof(data_send_to_device));
				// printf("Data wifi to BLE device after handler: %d %d %d\n",arr_int[1],arr_int[2],arr_int[3]);
				sprintf(data_send_to_device,"%d %d %d",arr_int[1],arr_int[2],arr_int[3]);
				if(strlen(data_send_to_device) < 10)
				{
					printf("\nData_Before_Send_To_Device_BLE: %s\n",data_send_to_device);
					uart_write("ttyS2",data_send_to_device);
					uart_write("ttyS2","\n");
				}
				break;
			}
		}
	}
	pthread_detach(pthread_self());
}

void *thread_write_data_to_wifi(void *arg)
{
	typedef enum{
		Zigbee,
		BLE
	}type_device_send;

	typedef enum{
		ZONE_1,
		ZONE_2,
		ZONE_3
	}no_zone;
	no_zone no_zone_ble = ZONE_1;
	no_zone no_zone_zigbee = ZONE_1;
	data_project_t *data = (data_project_t*)arg;
	type_device_send type_device = Zigbee;
	while(1)
	{
		switch(type_device)
		{
			case Zigbee:
			{
				// uart_write("ttyS5", );
				switch(no_zone_zigbee)
				{
					case ZONE_1:
					{
						printf("\nData_Zigbee_Ready_To_Send: %s\n",data->data_zigbee.zone_1);
						no_zone_zigbee = ZONE_2;
						uart_write("ttyS5",data->data_zigbee.zone_1);
						break;
					}
					case ZONE_2:
					{
						printf("\nData_Zigbee_Ready_To_Send: %s\n",data->data_zigbee.zone_2);
						no_zone_zigbee = ZONE_1;
						uart_write("ttyS5",data->data_zigbee.zone_2);
						type_device = BLE;
						break;
					}
				}
				break;
			}
			case BLE:
			{
				switch(no_zone_ble)
				{
					case ZONE_1:
					{
						printf("\nData_BLE_Ready_To_Send: %s\n",data->data_ble.zone_1);
						no_zone_ble = ZONE_2;
						uart_write("ttyS5",data->data_ble.zone_1);
						break;
					}
					case ZONE_2:
					{
						printf("\nData_BLE_Ready_To_Send: %s\n",data->data_ble.zone_2);
						no_zone_ble = ZONE_3;
						uart_write("ttyS5",data->data_ble.zone_2);
						break;
					}
					case ZONE_3:
					{
						printf("\nData_BLE_Ready_To_Send: %s\n",data->data_ble.zone_3);
						no_zone_ble = ZONE_1;
						uart_write("ttyS5",data->data_ble.zone_3);
						type_device = Zigbee;
						break;
					}
				}
				break;
			}
		}
		sleep(1);
		
	}
	pthread_detach(pthread_self());
}

void thread_init(data_project_t *data)
{
	if(pthread_create(&data->data_thread.read_ble, NULL, data->data_thread.func_handle_read_ble, (void *)data))
	{
		printf("pthread_create for task read BLE error !\n");
		return;
	}

	if(pthread_create(&data->data_thread.read_zigbee, NULL, data->data_thread.func_handle_read_zigbee, (void *)data))
	{
		printf("pthread_create for task read Zigbee error !\n");
		return;
	}

	if(pthread_create(&data->data_thread.read_wifi, NULL, data->data_thread.func_handle_read_wifi, (void *)data))
	{
		printf("pthread_create for task read Wifi error !\n");
		return;
	}

	if(pthread_create(&data->data_thread.write_wifi, NULL, data->data_thread.func_handle_write_wifi, (void *)data))
	{
		printf("pthread_create for task write Wifi error !\n");
		return;
	}
}

int main(void)
{
	data_project_t *data_project;
	data_project = (data_project_t *)malloc(sizeof(data_project_t));
	
	lm75_init("i2c-1", ADDR_LM75, &data_project->data_self.fd_lm75);
	bh1750_init("i2c-1", ADDR_BH1750, &data_project->data_self.fd_bh1750);
	lcd_init("i2c-1", ADDR_LCD_I2C_20_4, &data_project->data_self.fd_lcd20_4);

	data_project->data_thread.func_handle_read_ble = thread_read_data_ble;
	data_project->data_thread.func_handle_read_zigbee = thread_read_data_zigbee;
	data_project->data_thread.func_handle_read_wifi = thread_read_data_wifi;
	data_project->data_thread.func_handle_write_wifi = thread_write_data_to_wifi;

	thread_init(data_project);
	while(1)
	{
		bh1750_read(data_project->data_self.fd_bh1750, &data_project->data_self.data_sensor_bh1750);
		lm75_read(data_project->data_self.fd_lm75, &data_project->data_self.data_sensor_lm75);
		lcd_display(data_project->data_self.fd_lcd20_4, data_project->data_self.data_sensor_lm75, data_project->data_self.data_sensor_bh1750);
		sleep(1);
	}
	return 0;
}


