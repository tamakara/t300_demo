/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "cmsis_os2.h"
#include "main.h"
#include "task.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "event_groups.h"
#include "t300/led.h"
#include "t300/log.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define EVENT_SENSOR_READY (1U << 0)
#define EVENT_NETWORK_READY (1U << 1)
#define EVENT_STORAGE_READY (1U << 2)

#define EVENT_ALL_READY                                                        \
  (EVENT_SENSOR_READY | EVENT_NETWORK_READY | EVENT_STORAGE_READY)

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
EventGroupHandle_t systemEventGroupHandle;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
    .name = "defaultTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void SensorInitTask(void *argument);
void NetworkInitTask(void *argument);
void StorageInitTask(void *argument);
void SystemTask(void *argument);

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void)
{
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */

  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */

  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */

  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */

  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle =
      osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  const osThreadAttr_t sensorInitTask_attributes = {
      .name = "SensorInit",
      .stack_size = 1024,
      .priority = osPriorityNormal,
  };
  osThreadNew(SensorInitTask, NULL, &sensorInitTask_attributes);

  const osThreadAttr_t networkInitTask_attributes = {
      .name = "NetworkInit",
      .stack_size = 1024,
      .priority = osPriorityNormal,
  };
  osThreadNew(NetworkInitTask, NULL, &networkInitTask_attributes);

  const osThreadAttr_t storageInitTask_attributes = {
      .name = "StorageInit",
      .stack_size = 1024,
      .priority = osPriorityNormal,
  };
  osThreadNew(StorageInitTask, NULL, &storageInitTask_attributes);

  const osThreadAttr_t systemTask_attributes = {
      .name = "SystemTask",
      .stack_size = 1024,
      .priority = osPriorityNormal,
  };
  osThreadNew(SystemTask, NULL, &systemTask_attributes);

  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  systemEventGroupHandle = xEventGroupCreate();

  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for (;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void SensorInitTask(void *argument)
{
  (void)argument;

  osDelay(1000);

  printf("Sensor: Initialization Complete!\r\n");

  xEventGroupSetBits(systemEventGroupHandle, EVENT_SENSOR_READY);

  vTaskDelete(NULL);
}

void NetworkInitTask(void *argument)
{
  (void)argument;

  osDelay(2000);

  printf("Network: Initialization Complete!\r\n");

  xEventGroupSetBits(systemEventGroupHandle, EVENT_NETWORK_READY);

  vTaskDelete(NULL);
}

void StorageInitTask(void *argument)
{
  (void)argument;

  osDelay(3000);

  printf("Storage: Initialization Complete!\r\n");

  xEventGroupSetBits(systemEventGroupHandle, EVENT_STORAGE_READY);

  vTaskDelete(NULL);
}

void SystemTask(void *argument)
{
  (void)argument;

  printf("System: Waiting for initialization...\r\n");

  EventBits_t bits = xEventGroupWaitBits(
      systemEventGroupHandle,
      EVENT_ALL_READY,
      pdFALSE,
      pdTRUE,
      portMAX_DELAY);

  if ((bits & EVENT_ALL_READY) == EVENT_ALL_READY)
  {
    printf("System: All Modules Ready!\r\n");
    printf("System: Data Upload Started!\r\n");
  }

  vTaskDelete(NULL);
}
/* USER CODE END Application */
