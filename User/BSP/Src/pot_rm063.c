#include "pot_rm063.h"

static uint16_t RM063_ReadADC(void)
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

float POT_RM063_ReadVoltage(void)
{
  uint16_t value = RM063_ReadADC();

  return (float)value * 3.3f / 4095.0f;
}