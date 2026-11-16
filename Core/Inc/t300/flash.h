#ifndef __FLASH_H
#define __FLASH_H

#include "spi.h"

/* Flash 片选控制 */
#define FLASH_CS_LOW()                                                         \
  HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET)

#define FLASH_CS_HIGH()                                                        \
  HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET)

/* SPI 单字节收发 */
uint8_t SPI1_ReadWriteByte(uint8_t data);

#endif