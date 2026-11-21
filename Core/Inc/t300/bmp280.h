#ifndef __BMP280_H
#define __BMP280_H

#include "i2c.h"
#define BMP280_ADDR (0x77 << 1)

uint8_t BMP280_ReadReg(uint8_t address, uint8_t *data, uint8_t size);
uint8_t BMP280_WriteReg(uint8_t reg, uint8_t value);
uint8_t BMP280_Init(void);
uint8_t BMP280_ReadRaw(int32_t *raw_temp, int32_t *raw_press);
uint8_t BMP280_Read(float *temperature, float *pressure);
#endif
