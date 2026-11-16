
#include "t300/flash.h"

uint8_t SPI1_ReadWriteByte(uint8_t data) {
  uint8_t receive_data = 0;

  HAL_SPI_TransmitReceive(&hspi1, &data, &receive_data, 1, HAL_MAX_DELAY);

  return receive_data;
}
