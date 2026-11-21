#include "t300/eeprom.h"

uint8_t EEPROM_Ready(void)
{
  HAL_StatusTypeDef state =
      HAL_I2C_IsDeviceReady(&hi2c1, EEPROM_DEV_ADDR, 10, HAL_MAX_DELAY);
  if (state != HAL_OK)
  {
    return 0; // Not ready
  }
  return 1; // Ready
}

uint8_t EEPROM_ReadByte(uint8_t address, uint8_t *data)
{
  HAL_StatusTypeDef state = HAL_I2C_Mem_Read(
      &hi2c1,
      EEPROM_DEV_ADDR,
      address,
      I2C_MEMADD_SIZE_8BIT,
      data,
      1,
      HAL_MAX_DELAY);
  if (state != HAL_OK)
  {
    return 1;
  }
  return 0;
}

uint8_t EEPROM_WriteByte(uint8_t address, uint8_t data)
{
  HAL_StatusTypeDef state = HAL_I2C_Mem_Write(
      &hi2c1,
      EEPROM_DEV_ADDR,
      address,
      I2C_MEMADD_SIZE_8BIT,
      &data,
      1,
      HAL_MAX_DELAY);
  if (state != HAL_OK)
  {
    return 1;
  }
  return 0;
}
