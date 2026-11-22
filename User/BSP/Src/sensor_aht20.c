#include "sensor_aht20.h"
#include "i2c.h"

#define AHT20_DEV_ADDR (0x38 << 1)

static uint8_t AHT20_ReadRaw(uint8_t *data)
{
  uint8_t cmd[3] = {0xAC, 0x33, 0x00};

  if (data == NULL)
    return 0;

  // 触发测量
  if (HAL_I2C_Master_Transmit(&hi2c1, AHT20_DEV_ADDR, cmd, 3, 100) != HAL_OK)
  {
    return 0;
  }

  // 等待测量完成
  uint32_t start = HAL_GetTick();

  while (HAL_GetTick() - start < HAL_MAX_DELAY)
  {
    if (HAL_I2C_Master_Receive(&hi2c1, AHT20_DEV_ADDR, data, 7, 100) != HAL_OK)
    {
      return 0;
    }

    // bit7 = 0，表示测量完成
    if ((data[0] & 0x80) == 0)
    {
      return 1;
    }
  }

  return 0;
}

uint8_t Sensor_AHT20_Ready(void)
{
  HAL_StatusTypeDef state =
      HAL_I2C_IsDeviceReady(&hi2c1, AHT20_DEV_ADDR, 10, HAL_MAX_DELAY);
  if (state != HAL_OK)
  {
    return 0; // Not ready
  }
  return 1; // Ready
}

uint8_t Sensor_AHT20_Init(void)
{
  uint8_t status;

  if (HAL_I2C_Master_Receive(
          &hi2c1,
          AHT20_DEV_ADDR,
          &status,
          1,
          HAL_MAX_DELAY) != HAL_OK)
  {
    return 0;
  }

  // 检查校准使能状态（bit 3）
  if ((status & 0x08) == 0)
  {
    uint8_t cmd[3] = {0xBE, 0x08, 0x00};

    if (HAL_I2C_Master_Transmit(
            &hi2c1,
            AHT20_DEV_ADDR,
            cmd,
            3,
            HAL_MAX_DELAY) != HAL_OK)
    {
      return 0;
    }

    if (HAL_I2C_Master_Receive(
            &hi2c1,
            AHT20_DEV_ADDR,
            &status,
            1,
            HAL_MAX_DELAY) != HAL_OK)
    {
      return 0;
    }
  }

  return (status & 0x08) ? 1 : 0;
}

uint8_t Sensor_AHT20_Read(float *temperature, float *humidity)
{
  uint8_t data[7];

  if (temperature == NULL || humidity == NULL)
    return 0;

  if (!AHT20_ReadRaw(data))
    return 0;

  // 提取 20 位湿度原始值
  uint32_t raw_humidity = ((uint32_t)data[1] << 12) | ((uint32_t)data[2] << 4) |
                          ((uint32_t)data[3] >> 4);

  // 提取 20 位温度原始值
  uint32_t raw_temperature =
      (((uint32_t)data[3] & 0x0F) << 16) | ((uint32_t)data[4] << 8) | data[5];

  // 转换为实际湿度（%RH）
  *humidity = (raw_humidity * 100.0f) / 1048576.0f;

  // 转换为实际温度（℃）
  *temperature = (raw_temperature * 200.0f) / 1048576.0f - 50.0f;

  return 1;
}