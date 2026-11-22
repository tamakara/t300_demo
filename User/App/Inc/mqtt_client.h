#ifndef __USER_APP_MQTT_CLIENT_H
#define __USER_APP_MQTT_CLIENT_H

#include "lwip/err.h"

err_t MQTT_ClientStart(void);
err_t MQTT_ClientPublishTest(void);

#endif
