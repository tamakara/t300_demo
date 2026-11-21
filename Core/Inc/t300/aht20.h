#ifndef __AHT20_H
#define __AHT20_H

#include "i2c.h"

#define AHT20_ADDR (0x38 << 1)

uint8_t AHT20_Ready(void);
uint8_t AHT20_Init(void);
uint8_t AHT20_ReadRaw(uint8_t *data);
uint8_t AHT20_Read(float *temperature, float *humidity);
#endif