#ifndef __BMP280_H
#define __BMP280_H

#include "i2c.h"
#define BMP280_ADDR (0x77 << 1)

typedef struct {
  uint16_t T1;
  int16_t T2;
  int16_t T3;

  uint16_t P1;
  int16_t P2;
  int16_t P3;
  int16_t P4;
  int16_t P5;
  int16_t P6;
  int16_t P7;
  int16_t P8;
  int16_t P9;

} BMP280_CalibData;

uint8_t BMP280_ReadCalibration(BMP280_CalibData *calib);

#endif
