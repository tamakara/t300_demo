/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "lwip.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "t300/aht20.h"
#include "t300/bmp280.h"
#include "t300/eeprom.h"
#include "t300/flash.h"
#include "t300/lan8720.h"
#include "t300/log.h"
#include "t300/rm063.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void I2C_Scan(void)
{
  printf("I2C Scan Start\r\n");

  for (uint8_t addr = 1; addr < 127; addr++)
  {
    if (HAL_I2C_IsDeviceReady(&hi2c1, addr << 1, 3, HAL_MAX_DELAY) == HAL_OK)
    {
      printf("Device Found: 0x%02X\r\n", addr);
    }
  }

  printf("I2C Scan Complete\r\n");
}

void eeprom_main(void)
{
  char line[20], operation[20];

  uint8_t address, data;

  while (1)
  {
    if (uart_getline(line, sizeof(line)) == EOF)
      continue;

    sscanf(line, "%s", operation);
    if (strcmp(operation, "read") == 0)
    {
      sscanf(line, "%s %hhx", operation, &address);
      if (EEPROM_ReadByte(address, &data) == 0)
      {
        printf("Read EPPROM[0x%02X]=0x%02X\r\n", address, data);
      }
      else
      {
        printf("Failed to read.\r\n");
      }
    }
    else if (strcmp(operation, "write") == 0)
    {
      sscanf(line, "%s %hhx %hhx", operation, &address, &data);
      if (EEPROM_WriteByte(address, data) == 0)
      {
        printf("Write EPPROM[0x%02X]=0x%02X\r\n", address, data);
      }
      else
      {
        printf("Failed to write.\r\n");
      }
    }
    else
    {
      printf("Invalid command. \r\n");
    }
  }
}

void flash_main()
{
  char line[20], operation[20];
  uint32_t address;
  uint8_t data;

  while (1)
  {
    if (uart_getline(line, sizeof(line)) == EOF)
      continue;

    sscanf(line, "%s", operation);
    if (strcmp(operation, "read") == 0)
    {
      sscanf(line, "%s %x", operation, &address);
      FLASH_Read(address, &data, 1);
      printf("Read FLASH[0x%06X]=0x%02X\r\n", address, data);
    }
    else if (strcmp(operation, "write") == 0)
    {
      sscanf(line, "%s %x %hhx", operation, &address, &data);
      FLASH_Write(address, &data, 1);
      printf("Write FLASH[0x%06x]=0x%02x\r\n", address, data);
    }
    else if (strcmp(operation, "erase") == 0)
    {
      sscanf(line, "%s %x", operation, &address);
      FLASH_EraseSector(address);
      uint32_t start = address & ~(FLASH_SECTOR_SIZE - 1);
      uint32_t end = start + FLASH_SECTOR_SIZE - 1;
      printf("Erase FLASH[0x%06X-0x%06X]=0xFF\r\n", start, end);
    }
    else
    {
      printf("Invalid command. \r\n");
    }
  }
}

void aht20_main()
{
  if (AHT20_Init())
  {
    printf("AHT20 Init Success!\r\n");
  }
  else
  {
    printf("AHT20 Init Failed!\r\n");
    return;
  }

  float temperature;
  float humidity;
  while (1)
  {
    if (AHT20_Read(&temperature, &humidity))
    {
      printf("Temperature: %.2f C\r\n", temperature);
      printf("Humidity: %.2f %%RH\r\n", humidity);
      printf("----------------------\r\n");
    }
    else
    {
      printf("AHT20 Read Failed!\r\n");
    }
    HAL_Delay(1000);
  }
}

void bmp280_main()
{
  if (BMP280_Init())
  {
    printf("BMP280 Init Success!\r\n");
  }
  else
  {
    printf("BMP280 Init Failed!\r\n");
    return;
  }

  float temperature;
  float pressure;

  while (1)
  {
    if (BMP280_Read(&temperature, &pressure))
    {
      printf("Temperature: %.2f C\r\n", temperature);
      printf("Pressure: %.2f Pa\r\n", pressure);
      printf("----------------------\r\n");
    }
    else
    {
      printf("BMP280 Read Failed!\r\n");
    }

    HAL_Delay(1000);
  }
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();  /* Call init function for freertos objects (in cmsis_os2.c) */
  MX_FREERTOS_Init();

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  // I2C_Scan();
  // eeprom_main();
  // flash_main();
  // aht20_main();
  // bmp280_main();

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enables the Clock Security System
  */
  HAL_RCC_EnableCSS();
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state
   */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line
     number, ex: printf("Wrong parameters value: file %s on line %d\r\n",
     file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
