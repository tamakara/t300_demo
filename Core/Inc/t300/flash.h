#ifndef __FLASH_H
#define __FLASH_H

#include "spi.h"

#define FLASH_CAPACITY 0x1000000U
#define FLASH_SECTOR_SIZE 4096U
#define FLASH_PAGE_SIZE 256U

#define FLASH_CS_LOW()                                                         \
  HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET)

#define FLASH_CS_HIGH()                                                        \
  HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET)

uint8_t SPI1_ReadWriteByte(uint8_t data);
void FLASH_ReadJEDECID(uint8_t *id);
uint8_t FLASH_ReadStatus(void);
void FLASH_Read(uint32_t address, uint8_t *buffer, uint32_t length);
void FLASH_WriteEnable(void);
uint8_t FLASH_WaitBusy(uint32_t timeout);
uint8_t FLASH_EraseSector(uint32_t address);
uint8_t FLASH_PageProgram(
    uint32_t address,
    const uint8_t *buffer,
    uint32_t length);
uint8_t FLASH_Write(uint32_t address, const uint8_t *buffer, uint32_t length);
#endif