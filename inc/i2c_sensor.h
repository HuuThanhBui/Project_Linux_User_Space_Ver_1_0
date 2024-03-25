#ifndef _I2C_SENSOR_H_
#define _I2C_SENSOR_H_

void i2c_init(char name_i2c[], uint32_t add_slave, int *fd);
void i2c_write(int fd, uint8_t data[], uint8_t leng_data);
void i2c_read(int fd, uint8_t data[], uint8_t leng_data);
void i2c_close(int fd);

#endif



