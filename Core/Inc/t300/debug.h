#ifndef __DEBUG_H
#define __DEBUG_H

#include "usart.h"
#include <stdio.h>
#include <string.h>

#define UART_TX_BUFFER_SIZE 256

int _write(int file, char *ptr, int len);
void Debug_UART_TxCpltCallback(UART_HandleTypeDef *huart);

#endif