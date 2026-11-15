#ifndef __EEPROM_H
#define __EEPROM_H

#include "i2c.h"

#define EEPROM_DEVICE_ADDRESS 0x50 << 1

uint8_t eeprom_ready();
uint8_t eeprom_read_byte(uint8_t address, uint8_t *data);
uint8_t eeprom_write_byte(uint8_t address, uint8_t data);

#endif
