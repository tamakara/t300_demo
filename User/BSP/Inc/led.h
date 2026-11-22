#ifndef __USER_BSP_LED_H
#define __USER_BSP_LED_H

#include <stdint.h>

void LED0_On(void);
void LED0_Off(void);
void LED0_Toggle(void);
uint8_t LED0_State(void);

void LED1_On(void);
void LED1_Off(void);
void LED1_Toggle(void);
uint8_t LED1_State(void);

#endif