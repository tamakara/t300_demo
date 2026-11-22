#include "mqtt_client.h"

#include "led.h"
#include "lwip/apps/mqtt.h"
#include "lwip/err.h"
#include "lwip/ip_addr.h"
#include "lwip/tcpip.h"
#include <stdio.h>
#include <string.h>

/* MQTT Broker */
#define MQTT_SERVER_IP "192.168.10.1"
#define MQTT_SERVER_PORT 1883

/* MQTT Client */
#define MQTT_CLIENT_ID "stm32"
#define MQTT_KEEP_ALIVE 60

static mqtt_client_t *mqtt_client = NULL;

static ip_addr_t mqtt_server_ip;

static struct mqtt_connect_client_info_t mqtt_client_info;

static volatile uint8_t mqtt_connected = 0;

/* MQTT connection callback */
static void MQTT_IncomingPublishCallback(
    void *argument,
    const char *topic,
    u32_t tot_len)
{
  (void)argument;

  printf("\r\nMQTT RX Topic: %s\r\n", topic);
  printf("MQTT RX Length: %lu\r\n", (unsigned long)tot_len);
}

static void MQTT_IncomingDataCallback(
    void *argument,
    const u8_t *data,
    u16_t len,
    u8_t flags)
{
  (void)argument;
  (void)flags;

  printf("MQTT RX Data: ");

  for (u16_t i = 0; i < len; i++)
  {
    printf("%c", data[i]);
  }

  if (len == 6 && memcmp((const char *)data, "LED_ON", 6) == 0)
  {
    LED0_On();
  }
  else if (len == 7 && memcmp((const char *)data, "LED_OFF", 7) == 0)
  {
    LED0_Off();
  }
  else
  {
    printf("Unknown command received.\r\n", len, data);
  }
}

static void MQTT_SubscribeCallback(void *argument, err_t err)
{
  (void)argument;

  if (err == ERR_OK)
  {
    printf("MQTT: Subscribe Success!\r\n");
  }
  else
  {
    printf("MQTT: Subscribe Failed: %d\r\n", err);
  }
}

static void MQTT_ConnectionCallback(
    mqtt_client_t *client,
    void *argument,
    mqtt_connection_status_t status)
{
  (void)client;
  (void)argument;

  if (status == MQTT_CONNECT_ACCEPTED)
  {
    mqtt_connected = 1;

    printf("MQTT: Connected successfully!\r\n");

    mqtt_set_inpub_callback(
        mqtt_client,
        MQTT_IncomingPublishCallback,
        MQTT_IncomingDataCallback,
        NULL);

    err_t err =
        mqtt_subscribe(mqtt_client, "command", 0, MQTT_SubscribeCallback, NULL);

    if (err != ERR_OK)
    {
      printf("MQTT: Subscribe Request Failed: %d\r\n", err);
    }
  }
  else
  {
    mqtt_connected = 0;
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

static void MQTT_PublishInCore(void *argument)
{
  static uint32_t count = 0;

  char payload[64];

  (void)argument;

  if (!mqtt_connected || mqtt_client == NULL)
  {
    return;
  }

  snprintf(
      payload,
      sizeof(payload),
      "Hello from STM32! Count: %lu",
      (unsigned long)count);

  err_t err = mqtt_publish(
      mqtt_client,
      "test",
      payload,
      strlen(payload),
      0, // QoS 0
      0, // Retain disabled
      NULL,
      NULL);

  if (err == ERR_OK)
  {
    printf("MQTT TX: %s\r\n", payload);
    count++;
  }
  else
  {
    printf("MQTT Publish Failed: %d\r\n", err);
  }
}

/* Called from an application task */
err_t MQTT_ClientStart(void)
{
  return tcpip_callback(MQTT_StartInCore, NULL);
}

err_t MQTT_ClientPublishTest(void)
{
  return tcpip_callback(MQTT_PublishInCore, NULL);
}
