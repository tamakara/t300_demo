#ifndef __RM063_H
#define __RM063_H

#include "adc.h"

float RM063_ReadVoltage(void);
float RM063_ReadVoltage_Average(uint8_t times);

#endif