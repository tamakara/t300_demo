#ifndef __USER_BSP_EPPROM_AT24C02_H
#define __USER_BSP_EPPROM_AT24C02_H

#include <stdint.h>

uint8_t EEPROM_AT24C02_Ready(void);
uint8_t EEPROM_AT24C02_ReadByte(uint8_t address, uint8_t *data);
uint8_t EEPROM_AT24C02_WriteByte(uint8_t address, uint8_t data);

#endif
