#ifndef __USER_BSP_FLASH_RC25Q128_H
#define __USER_BSP_FLASH_RC25Q128_H

#include <stdint.h>

uint8_t Flash_RC25Q128_EraseSector(uint32_t address);
void Flash_RC25Q128_Read(uint32_t address, uint8_t *buffer, uint32_t length);
uint8_t Flash_RC25Q128_Write(
    uint32_t address,
    const uint8_t *buffer,
    uint32_t length);
#endif