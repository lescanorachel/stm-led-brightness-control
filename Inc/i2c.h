#ifndef I2C_H__
#define I2C_H__

#include "stm32f4xx.h"
void i2c1_init(void);
void i2c1_burst_write(char slave_addr, int n, char * data);

#endif
