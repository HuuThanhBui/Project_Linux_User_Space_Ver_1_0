#ifndef _GPIO_LED_H_
#define _GPIO_LED_H_

#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"

typedef enum{
	in,
	out,
	none
}gpio_direction_t;

void gpio_led_init(uint8_t gpio_name, gpio_direction_t gpio_direction);
void gpio_led_uninit(uint8_t gpio_name);
void gpio_led_write(uint8_t gpio_name, uint8_t value);
int8_t gpio_led_read(uint8_t gpio_name);

#endif



