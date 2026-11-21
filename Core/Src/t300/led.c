#include "t300/led.h"

void LED0_On(void)
{
  HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
}

void LED0_Off(void)
{
  HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
}

void LED0_Toggle(void)
{
  HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
}

uint8_t LED0_State(void)
{
  return HAL_GPIO_ReadPin(LED0_GPIO_Port, LED0_Pin) == GPIO_PIN_RESET ? 1 : 0;
}

void LED1_On(void)
{
  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
}

void LED1_Off(void)
{
  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
}

void LED1_Toggle(void)
{
  HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
}

uint8_t LED1_State(void)
{
  return HAL_GPIO_ReadPin(LED1_GPIO_Port, LED1_Pin) == GPIO_PIN_RESET ? 1 : 0;
}
