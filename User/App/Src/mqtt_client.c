#include "mqtt_client.h"

#include "lwip/apps/mqtt.h"
#include "lwip/ip_addr.h"
#include "lwip/tcpip.h"

#include <stdio.h>
#include <string.h>

/* MQTT Broker */
#define MQTT_SERVER_IP "192.168.10.1"
#define MQTT_SERVER_PORT 1883

/* MQTT Client */
#define MQTT_CLIENT_ID "stm32_t300"
#define MQTT_KEEP_ALIVE 60

static mqtt_client_t *mqtt_client = NULL;

static ip_addr_t mqtt_server_ip;

static struct mqtt_connect_client_info_t mqtt_client_info;

/* MQTT connection callback */
static void MQTT_ConnectionCallback(
    mqtt_client_t *client,
    void *argument,
    mqtt_connection_status_t status)
{
  (void)client;
  (void)argument;

  if (status == MQTT_CONNECT_ACCEPTED)
  {
    printf("MQTT: Connected successfully!\r\n");
  }
  else
  {
    printf("MQTT: Connection status = %d\r\n", status);
  }
}

/* Run in LwIP TCP/IP core thread */
static void MQTT_StartInCore(void *argument)
{
  err_t err;

  (void)argument;

  mqtt_client = mqtt_client_new();

  if (mqtt_client == NULL)
  {
    printf("MQTT: Client allocation failed!\r\n");
    return;
  }

  memset(&mqtt_client_info, 0, sizeof(mqtt_client_info));

  mqtt_client_info.client_id = MQTT_CLIENT_ID;
  mqtt_client_info.client_user = NULL;
  mqtt_client_info.client_pass = NULL;
  mqtt_client_info.keep_alive = MQTT_KEEP_ALIVE;

  mqtt_client_info.will_topic = NULL;
  mqtt_client_info.will_msg = NULL;

  IP_ADDR4(&mqtt_server_ip, 192, 168, 10, 1);

  err = mqtt_client_connect(
      mqtt_client,
      &mqtt_server_ip,
      MQTT_SERVER_PORT,
      MQTT_ConnectionCallback,
      NULL,
      &mqtt_client_info);

  if (err != ERR_OK)
  {
    printf("MQTT: Connect request failed: %d\r\n", err);
  }
  else
  {
    printf("MQTT: Connecting to Broker...\r\n");
  }
}

/* Called from an application task */
err_t MQTT_ClientStart(void)
{
  return tcpip_callback(MQTT_StartInCore, NULL);
}
