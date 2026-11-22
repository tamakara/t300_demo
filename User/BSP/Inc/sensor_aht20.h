#ifndef __USER_BSP_SENSOR_AHT20_H
#define __USER_BSP_SENSOR_AHT20_H

#include <stdint.h>

uint8_t Sensor_AHT20_Ready(void);
uint8_t Sensor_AHT20_Init(void);
uint8_t Sensor_AHT20_Read(float *temperature, float *humidity);
#endif