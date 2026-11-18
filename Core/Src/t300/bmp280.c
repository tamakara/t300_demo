#include "t300/bmp280.h"

uint8_t BMP280_ReadCalibration(BMP280_CalibData *calib) {
  if (calib == NULL)
    return 0;

  uint8_t data[24];

  if (HAL_I2C_Mem_Read(
          &hi2c1,
          BMP280_ADDR,
          0x88,
          I2C_MEMADD_SIZE_8BIT,
          data,
          24,
          100) != HAL_OK) {
    return 0;
  }

  calib->T1 = (uint16_t)(data[1] << 8 | data[0]);
  calib->T2 = (int16_t)(data[3] << 8 | data[2]);
  calib->T3 = (int16_t)(data[5] << 8 | data[4]);

  calib->P1 = (uint16_t)(data[7] << 8 | data[6]);
  calib->P2 = (int16_t)(data[9] << 8 | data[8]);
  calib->P3 = (int16_t)(data[11] << 8 | data[10]);
  calib->P4 = (int16_t)(data[13] << 8 | data[12]);
  calib->P5 = (int16_t)(data[15] << 8 | data[14]);
  calib->P6 = (int16_t)(data[17] << 8 | data[16]);
  calib->P7 = (int16_t)(data[19] << 8 | data[18]);
  calib->P8 = (int16_t)(data[21] << 8 | data[20]);
  calib->P9 = (int16_t)(data[23] << 8 | data[22]);

  return 1;
}