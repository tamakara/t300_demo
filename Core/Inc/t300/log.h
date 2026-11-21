#ifndef __LOG_H
#define __LOG_H

#include "usart.h"
#include <stdio.h>
#include <string.h>

int uart_getline(char *buf, int max_len);

#endif