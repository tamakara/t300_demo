#include "t300/log.h"

int __io_putchar(int ch)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
  return ch;
}

int uart_getline(char *buf, int max_len)
{
  int i = 0;
  uint8_t ch;

  while (i < max_len - 1)
  {
    if (HAL_UART_Receive(&huart1, &ch, 1, HAL_MAX_DELAY) != HAL_OK)
      return EOF; // 接收失败

    if (ch == '\n') // 一行结束
      break;
    if (ch == '\r') // 忽略回车
      continue;

    buf[i++] = (char)ch;
  }

  buf[i] = '\0';
  return i;
}