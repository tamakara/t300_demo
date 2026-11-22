#ifndef __USER_BSP_SENSOR_BMP280_H
#define __USER_BSP_SENSOR_BMP280_H

#include <stdint.h>

uint8_t Sensor_BMP280_Ready(void);
uint8_t Sensor_BMP280_Init(void);
uint8_t Sensor_BMP280_Read(float *temperature, float *pressure);

#endif
