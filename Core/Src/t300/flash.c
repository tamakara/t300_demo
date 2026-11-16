
#include "t300/flash.h"

uint8_t SPI1_ReadWriteByte(uint8_t data) {
  uint8_t receive_data = 0;

  HAL_SPI_TransmitReceive(&hspi1, &data, &receive_data, 1, HAL_MAX_DELAY);

  return receive_data;
}

void FLASH_ReadJEDECID(uint8_t *id) {
  FLASH_CS_LOW();

  SPI1_ReadWriteByte(0x9F);

  id[0] = SPI1_ReadWriteByte(0xFF);
  id[1] = SPI1_ReadWriteByte(0xFF);
  id[2] = SPI1_ReadWriteByte(0xFF);

  FLASH_CS_HIGH();
}

uint8_t FLASH_ReadStatus(void) {
  uint8_t status;

  FLASH_CS_LOW();

  SPI1_ReadWriteByte(0x05);
  status = SPI1_ReadWriteByte(0xFF);

  FLASH_CS_HIGH();

  return status;
}
void FLASH_Read(uint32_t address, uint8_t *buffer, uint32_t length) {
  if (buffer == NULL || length == 0) {
    return;
  }

  // 检查 24 位地址范围
  if (address >= 0x1000000 || length > 0x1000000 - address) {
    return;
  }

  FLASH_CS_LOW();

  SPI1_ReadWriteByte(0x03);

  SPI1_ReadWriteByte((address >> 16) & 0xFF);
  SPI1_ReadWriteByte((address >> 8) & 0xFF);
  SPI1_ReadWriteByte(address & 0xFF);

  for (uint32_t i = 0; i < length; i++) {
    buffer[i] = SPI1_ReadWriteByte(0xFF);
  }

  FLASH_CS_HIGH();
}

void FLASH_WriteEnable(void) {
  FLASH_CS_LOW();

  SPI1_ReadWriteByte(0x06);

  FLASH_CS_HIGH();
}

uint8_t FLASH_WaitBusy(uint32_t timeout) {
  uint32_t start_tick = HAL_GetTick();

  while (FLASH_ReadStatus() & 0x01) {
    if (HAL_GetTick() - start_tick >= timeout) {
      return 0; // 超时
    }
  }

  return 1; // 操作完成
}

uint8_t FLASH_EraseSector(uint32_t address) {
  // 1. 检查地址范围
  if (address >= FLASH_CAPACITY) {
    return 0;
  }

  // 2. 对齐到扇区起始地址
  address &= ~(FLASH_SECTOR_SIZE - 1);

  // 3. 等待 Flash 空闲
  if (!FLASH_WaitBusy(HAL_MAX_DELAY)) {
    return 0;
  }

  // 4. 开启写使能
  FLASH_WriteEnable();

  // 5. 检查 WEL
  if ((FLASH_ReadStatus() & 0x02) == 0) {
    return 0;
  }

  // 6. 发送扇区擦除指令
  FLASH_CS_LOW();

  SPI1_ReadWriteByte(0x20);

  SPI1_ReadWriteByte((address >> 16) & 0xFF);
  SPI1_ReadWriteByte((address >> 8) & 0xFF);
  SPI1_ReadWriteByte(address & 0xFF);

  FLASH_CS_HIGH();

  // 7. 等待擦除完成
  return FLASH_WaitBusy(HAL_MAX_DELAY);
}

uint8_t FLASH_PageProgram(
    uint32_t address,
    const uint8_t *buffer,
    uint32_t length) {
  // 1. 参数检查
  if (buffer == NULL || length == 0) {
    return 0;
  }

  // 2. 地址范围检查
  if (address >= FLASH_CAPACITY || length > FLASH_CAPACITY - address) {
    return 0;
  }

  // 3. 检查是否跨越 Page
  uint32_t page_remain = FLASH_PAGE_SIZE - (address % FLASH_PAGE_SIZE);

  if (length > page_remain) {
    return 0;
  }

  // 4. 等待 Flash 空闲
  if (!FLASH_WaitBusy(HAL_MAX_DELAY)) {
    return 0;
  }

  // 5. 写使能
  FLASH_WriteEnable();

  // 6. 检查 WEL
  if ((FLASH_ReadStatus() & 0x02) == 0) {
    return 0;
  }

  // 7. 发送 Page Program 指令和地址
  FLASH_CS_LOW();

  SPI1_ReadWriteByte(0x02);

  SPI1_ReadWriteByte((address >> 16) & 0xFF);
  SPI1_ReadWriteByte((address >> 8) & 0xFF);
  SPI1_ReadWriteByte(address & 0xFF);

  // 8. 发送数据
  for (uint32_t i = 0; i < length; i++) {
    SPI1_ReadWriteByte(buffer[i]);
  }

  FLASH_CS_HIGH();

  // 9. 等待编程完成
  return FLASH_WaitBusy(HAL_MAX_DELAY);
}

uint8_t FLASH_Write(uint32_t address, const uint8_t *buffer, uint32_t length) {
  if (buffer == NULL || length == 0) {
    return 0;
  }

  if (address >= FLASH_CAPACITY || length > FLASH_CAPACITY - address) {
    return 0;
  }

  while (length > 0) {
    // 当前 Page 剩余空间
    uint32_t page_remain = FLASH_PAGE_SIZE - (address % FLASH_PAGE_SIZE);

    // 本次实际写入长度
    uint32_t write_length = (length < page_remain) ? length : page_remain;

    // 写入当前 Page
    if (!FLASH_PageProgram(address, buffer, write_length)) {
      return 0;
    }

    // 更新地址、数据指针和剩余长度
    address += write_length;
    buffer += write_length;
    length -= write_length;
  }

  return 1;
}