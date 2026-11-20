#include "t300/debug.h"

extern UART_HandleTypeDef huart1;

#define UART_TX_BUFFER_SIZE 256

static uint8_t tx_buffer[UART_TX_BUFFER_SIZE];
static volatile uint8_t tx_busy = 0;

int _write(int file, char *ptr, int len)
{
  (void)file;

  if (len <= 0 || len > UART_TX_BUFFER_SIZE)
  {
    return -1;
  }

  // 等待上一次 DMA 发送完成
  while (tx_busy)
  {
  }

  memcpy(tx_buffer, ptr, len);

  tx_busy = 1;

  if (HAL_UART_Transmit_DMA(&huart1, tx_buffer, len) != HAL_OK)
  {
    tx_busy = 0;
    return -1;
  }

  return len;
}

void Debug_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
    tx_busy = 0;
  }
}