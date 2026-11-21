#include "t300/rm063.h"

uint16_t RM063_ReadADC(void)
{
  uint16_t value = 0;

  HAL_ADC_Start(&hadc1);

  if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)
  {
    value = (uint16_t)HAL_ADC_GetValue(&hadc1);
  }

  HAL_ADC_Stop(&hadc1);

  return value;
}

uint16_t RM063_ReadADC_Average(uint8_t times)
{
  if (times == 0)
  {
    return 0;
  }

  uint32_t total = 0;

  for (uint8_t i = 0; i < times; i++)
  {
    total += RM063_ReadADC();
  }

  return (uint16_t)(total / times);
}

float RM063_ReadVoltage(void)
{
  uint16_t value = RM063_ReadADC();

  return (float)value * 3.3f / 4095.0f;
}

float RM063_ReadVoltage_Average(uint8_t times)
{
  uint16_t value = RM063_ReadADC_Average(times);

  return (float)value * 3.3f / 4095.0f;
}