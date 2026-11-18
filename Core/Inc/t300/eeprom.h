#ifndef __EEPROM_H
#define __EEPROM_H

#include "i2c.h"

#define EEPROM_ADDR (0x50 << 1)

uint8_t EEPROM_Ready();
uint8_t EEPROM_ReadByte(uint8_t address, uint8_t *data);
uint8_t EEPROM_WriteByte(uint8_t address, uint8_t data);

#endif
