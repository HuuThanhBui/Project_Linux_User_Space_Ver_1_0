TOOLCHAIN := /home/os/Desktop/Learn_Embedded_Linux/toolchain/12-2022/gcc-linaro-12.2.1-2022.12-x86_64_arm-linux-gnueabihf/bin
PREFIX := arm-linux-gnueabihf-

ifeq (${ARCH}, arm)
	CC := ${TOOLCHAIN}/${PREFIX}gcc
endif

all:
	${CC} -o main_project src/main.c src/string_handler.c src/gpio_led.c src/uart_module_rf.c src/i2c_sensor.c src/lcd_20_4.c src/bh1750.c src/lm75.c -I inc/ -pthread

clean:
	rm -rf main_project
