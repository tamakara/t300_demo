#ifndef __BMP280_H
#define __BMP280_H

#include "i2c.h"
#define BMP280_ADDR (0x77 << 1)

typedef struct {
  uint16_t dig_T1;
  int16_t dig_T2;
  int16_t dig_T3;

  uint16_t dig_P1;
  int16_t dig_P2;
  int16_t dig_P3;
  int16_t dig_P4;
  int16_t dig_P5;
  int16_t dig_P6;
  int16_t dig_P7;
  int16_t dig_P8;
  int16_t dig_P9;
} BMP280_Calib_t;

static BMP280_Calib_t bmp280_calib;
static int32_t bmp280_t_fine;

uint8_t BMP280_ReadReg(uint8_t address, uint8_t *data, uint8_t size);
uint8_t BMP280_WriteReg(uint8_t reg, uint8_t value);
uint8_t BMP280_Init();
static uint8_t BMP280_ReadCalibration();
uint8_t BMP280_ReadRaw(int32_t *raw_temp, int32_t *raw_press);
uint8_t BMP280_ReadTemperature(float *temperature);
uint8_t BMP280_ReadPressure(float *pressure);
uint8_t BMP280_ReadData(float *temperature, float *pressure);
#endif
