#include "tasks.h"
#include "cmsis_os2.h"
#include "lwip/netif.h"
#include "mqtt_client.h"
#include <stdint.h>

extern struct netif gnetif;

void NetworkStatusTask(void *argument)
{
  (void)argument;

  uint8_t mqtt_started = 0;

  while (1)
  {
    if (netif_is_link_up(&gnetif))
    {
      if (!mqtt_started)
      {
        if (MQTT_ClientStart() == ERR_OK)
        {
          mqtt_started = 1;
        }
      }

      if (mqtt_started)
      {
        MQTT_ClientPublishTest();
      }
    }

    osDelay(2000);
  }
}