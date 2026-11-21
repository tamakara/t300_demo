#include "t300/bmp280.h"

uint8_t BMP280_ReadReg(uint8_t address, uint8_t *data, uint8_t size)
{
  return HAL_I2C_Mem_Read(
             &hi2c1,
             BMP280_ADDR,
             address,
             I2C_MEMADD_SIZE_8BIT,
             data,
             size,
             100) == HAL_OK;
}

uint8_t BMP280_WriteReg(uint8_t reg, uint8_t value)
{
  return HAL_I2C_Mem_Write(
             &hi2c1,
             BMP280_ADDR,
             reg,
             I2C_MEMADD_SIZE_8BIT,
             &value,
             1,
             100) == HAL_OK;
}

typedef struct
{
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
} calib_t;

static calib_t calib;

static uint8_t BMP280_ReadCalibration(void)
{
  uint8_t data[24];

  if (HAL_I2C_Mem_Read(
          &hi2c1,
          BMP280_ADDR,
          0x88,
          I2C_MEMADD_SIZE_8BIT,
          data,
          24,
          100) != HAL_OK)
  {
    return 0;
  }
  calib.T1 = (uint16_t)(data[1] << 8 | data[0]);
  calib.T2 = (int16_t)(data[3] << 8 | data[2]);
  calib.T3 = (int16_t)(data[5] << 8 | data[4]);

  calib.P1 = (uint16_t)(data[7] << 8 | data[6]);
  calib.P2 = (int16_t)(data[9] << 8 | data[8]);
  calib.P3 = (int16_t)(data[11] << 8 | data[10]);
  calib.P4 = (int16_t)(data[13] << 8 | data[12]);
  calib.P5 = (int16_t)(data[15] << 8 | data[14]);
  calib.P6 = (int16_t)(data[17] << 8 | data[16]);
  calib.P7 = (int16_t)(data[19] << 8 | data[18]);
  calib.P8 = (int16_t)(data[21] << 8 | data[20]);
  calib.P9 = (int16_t)(data[23] << 8 | data[22]);

  // P1 为 0 时，气压补偿公式无法正常计算
  if (calib.P1 == 0)
  {
    return 0;
  }

  return 1;
}
uint8_t BMP280_Init(void)
{
  // 软复位
  if (!BMP280_WriteReg(0xE0, 0xB6))
  {
    return 0;
  }

  HAL_Delay(10);

  // 读取出厂校准参数
  if (!BMP280_ReadCalibration())
  {
    return 0;
  }

  // 配置滤波器
  if (!BMP280_WriteReg(0xF5, 0x08))
  {
    return 0;
  }

  // 温度 x2，气压 x16，Normal 模式
  if (!BMP280_WriteReg(0xF4, 0x57))
  {
    return 0;
  }

  return 1;
}

uint8_t BMP280_ReadRaw(int32_t *raw_temp, int32_t *raw_press)
{
  if (raw_temp == NULL || raw_press == NULL)
    return 0;

  uint8_t data[6];

  if (!BMP280_ReadReg(0xF7, data, 6))
  {
    return 0;
  }

  *raw_press = ((int32_t)data[0] << 12) | ((int32_t)data[1] << 4) |
               ((int32_t)data[2] >> 4);

  *raw_temp = ((int32_t)data[3] << 12) | ((int32_t)data[4] << 4) |
              ((int32_t)data[5] >> 4);

  return 1;
}

uint8_t BMP280_Read(float *temperature, float *pressure)
{
  int32_t adc_T;
  int32_t adc_P;

  int32_t var1_T;
  int32_t var2_T;
  int32_t t;
  int32_t t_fine;

  int64_t var1_P;
  int64_t var2_P;
  int64_t p;

  // 1. 一次性读取温度和气压原始数据
  if (!BMP280_ReadRaw(&adc_T, &adc_P))
  {
    return 0;
  }

  // 2. 温度补偿
  var1_T =
      ((((adc_T >> 3) - ((int32_t)calib.T1 << 1))) * ((int32_t)calib.T2)) >> 11;

  var2_T = (((((adc_T >> 4) - ((int32_t)calib.T1)) *
              ((adc_T >> 4) - ((int32_t)calib.T1))) >>
             12) *
            ((int32_t)calib.T3)) >>
           14;

  t_fine = var1_T + var2_T;

  if (temperature != NULL)
  {
    t = (t_fine * 5 + 128) >> 8;
    *temperature = t / 100.0f;
  }

  // 3. 气压补偿
  var1_P = ((int64_t)t_fine) - 128000;

  var2_P = var1_P * var1_P * (int64_t)calib.P6;

  var2_P = var2_P + ((var1_P * (int64_t)calib.P5) << 17);

  var2_P = var2_P + (((int64_t)calib.P4) << 35);

  var1_P = ((var1_P * var1_P * (int64_t)calib.P3) >> 8) +
           ((var1_P * (int64_t)calib.P2) << 12);

  var1_P = (((((int64_t)1) << 47) + var1_P) * (int64_t)calib.P1) >> 33;

  if (var1_P == 0)
  {
    return 0;
  }

  p = 1048576 - adc_P;

  p = (((p << 31) - var2_P) * 3125) / var1_P;

  var1_P = ((int64_t)calib.P9 * (p >> 13) * (p >> 13)) >> 25;

  var2_P = ((int64_t)calib.P8 * p) >> 19;

  p = ((p + var1_P + var2_P) >> 8) + (((int64_t)calib.P7) << 4);

  *pressure = (float)p / 256.0f;

  return 1;
}