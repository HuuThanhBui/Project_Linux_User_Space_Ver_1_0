#ifndef _MAIN_H_
#define _MAIN_H_

#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "pthread.h"

typedef struct {
	int fd_lm75;
	int fd_bh1750;
	int fd_lcd20_4;
	double data_sensor_lm75;
	double data_sensor_bh1750;
}data_self_t;

// typedef struct {
// 	uint16_t data_lm75;
// 	uint8_t byte_1_lm75;
// 	uint8_t byte_2_lm75;
// 	uint16_t data_bh1750;
// 	uint8_t byte_1_bh1750;
// 	uint8_t byte_2_bh1750;
// 	uint8_t data_led[8];
// }data_zone_1_t;

// typedef struct {
// 	uint16_t data_temp_dht11;
// 	uint8_t byte_1_temp_dht11;
// 	uint8_t byte_2_temp_dht11;
// 	uint16_t data_humi_dht11;
// 	uint8_t byte_1_humi_dht11;
// 	uint8_t byte_2_humi_dht11;
// 	uint16_t data_lm35;
// 	uint8_t byte_1_lm35;
// 	uint8_t byte_2_lm35;
// 	uint8_t data_led[8];
// }data_zone_2_t;

// typedef struct {
// 	uint16_t data_pir;
// 	uint8_t byte_1_pir;
// 	uint8_t byte_2_pir;
// 	uint16_t data_mq7;
// 	uint8_t byte_1_mq7;
// 	uint8_t byte_2_mq7;
// 	uint8_t data_led[8];
// }data_zone_3_t;

typedef struct {
	char zone_1[1024];
	char zone_2[1024];
}data_zigbee_t;

typedef struct {
	char zone_1[1024];
	char zone_2[1024];
	char zone_3[1024];
}data_ble_t;

typedef struct {
	pthread_t read_ble;
	pthread_t read_zigbee;
	pthread_t read_wifi;
	pthread_t write_ble;
	pthread_t write_zigbee;
	pthread_t write_wifi;
	void *(*func_handle_read_ble)(void *arg);
	void *(*func_handle_read_zigbee)(void *arg);
	void *(*func_handle_read_wifi)(void *arg);
	void *(*func_handle_write_wifi)(void *arg);
	void *(*func_handle_write_ble)(void *arg);
	void *(*func_handle_write_zigbee)(void *arg);
}data_thread_t;

typedef struct {
	data_self_t data_self;
	data_zigbee_t data_zigbee;
	data_ble_t data_ble;
	data_thread_t data_thread;
}data_project_t;

#endif


